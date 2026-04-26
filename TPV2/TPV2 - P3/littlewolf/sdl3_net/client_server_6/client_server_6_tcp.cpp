// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL_net.h>
#include <SDL_stdinc.h>
#include <cassert>
#include <csignal>
#include <cstdlib>
#include <cstring>

#include "messages.h"

using namespace std;

void error() {
	cout << "Error: " << SDL_GetError() << endl;
	exit(1);
}

// the port on which the server is listening, to be used by method
// shutdown to connect in order to wake up the server if blocking
bool done = false;
Uint16 shutDownTriggetPort = 0;

// this just sent an empty message to the server/client to wake it up.
void shutdown() {

	if (done)
		return;

	cout << endl << "Shutting down  ..." << endl;

	done = true;

	if (shutDownTriggetPort != 0) {

		// Resolve the hostname to an IP address
		NET_Address *addr = NET_ResolveHostname("localhost");
		if (NET_WaitUntilResolved(addr, 1000) != NET_SUCCESS) {
			error();
		}

		// Connect to the server, just to wake it up if it is sleeping
		NET_StreamSocket *conn = NET_CreateClient(addr, shutDownTriggetPort);
		if (NET_WaitUntilConnected(conn, 1000) != NET_SUCCESS) {
			error();
		}

		// UnReference the address so it can be freed
		NET_UnrefAddress(addr);

		// Destroy the socket
		NET_DestroyStreamSocket(conn);
	}
}

/* Signal Handler for SIGINT */
void handlerSIGINT(int) {
	shutdown();
}

constexpr std::size_t MAX_CLIENT = 10;

void server(Uint16 port) {
	cout << "Starting server at port " << port << endl;

	// Trap for CTRL-C -- it will call handlerSIGINT which in turn calls shutdown
	signal(SIGINT, handlerSIGINT);

	NET_Server *serv = NET_CreateServer(nullptr, port);
	if (!serv) {
		error();
	}

	// Store the port so it can be used by shutdown
	shutDownTriggetPort = port;

	cout << "The server is up and kicking ..." << endl;

	// An array of all connections. We will use it to check if there
	// is activity in any of them.
	void *conn[MAX_CLIENT+1];
	std::size_t used_conn = 1;

	// We store the server in the first position
	conn[0] = serv;

	// This array will store the socket in the position of the client ID.
	NET_StreamSocket *clients[MAX_CLIENT] = { }; // initialize to nullptr

	done = false;
	while (!done) {

		// Sleep for 60sec, or until someone connects to the server
		if (NET_WaitUntilInputAvailable(conn, used_conn, 60000) > 0) {

			// in case of shutdown wake-up
			if (done)
				continue;

			// We don't know exactly in which socket we had activity, so we check
			// them all.

			// If we have activity in 'serv', we accept the connection
			NET_StreamSocket *client = nullptr;
			if (NET_AcceptClient(serv, &client) && client != nullptr) {

				if (used_conn-1 < MAX_CLIENT) {
					Uint8 j = 0;
					while (j < MAX_CLIENT && clients[j] != nullptr)
						j++;

					// we must have found one, because used_conn < MAX_CLIENT
					assert(j < MAX_CLIENT);

					conn[used_conn] = clients[j] = client;
					std::cout << "New client assigned id " << (int) j
							<< std::endl;
					++used_conn;

					// Send acceptance message
					MsgWithClientId m;
					m.id = _REQ_ACC;
					m.clientId = j;
					SDLNetUtils::serialized_send(m, client);

					// We tell all to send their states so the new client get it
					Msg m_send_state;
					m_send_state.id = _SEND_STATE;
					for (auto j = 1u; j < used_conn; j++) {
						SDLNetUtils::serialized_send(m_send_state,
								(NET_StreamSocket*) conn[j]);
					}
				} else {
					Msg m;
					m.id = _REQ_REJ;
					SDLNetUtils::serialized_send(m, client);
				}
			}

			// Check activity in the clients sockets
			for (auto i = 1u; i < used_conn; i++) {

				// Read a message
				SDLNetUtils::buff_t buf = SDLNetUtils::receive(
						(NET_StreamSocket*) conn[i]);

				// forward it
				if (!buf.error) {
					if (buf.size > 0) {
						for (auto j = 1u; j < used_conn; j++) {
							if (i != j) {
								SDLNetUtils::send((NET_StreamSocket*) conn[j],
										buf.data, buf.size);
							}
						}
					}
				} else {

					// delete socket form the clients array
					Uint8 j = 0;
					while (j < MAX_CLIENT && (void*) clients[j] != conn[i])
						j++;

					std::cout << "Client " << (int)j << " disconnected!"
							<< std::endl;

					// We must have found it, so we remove it
					assert(j < MAX_CLIENT);
					clients[j] = nullptr;
					conn[i] = conn[used_conn - 1];
					conn[used_conn - 1] = nullptr;
					--used_conn;

					// Tell all clients, that client 'j' has disconnected
					MsgWithClientId m;
					m.id = _CLIENT_DISCONNECTED;
					m.clientId = j;
					for (auto j = 1u; j < used_conn; j++) {
						SDLNetUtils::serialized_send(m,
								(NET_StreamSocket*) conn[j]);
					}
				}

			}

		}
	}

	// Dispose all client sockets
	for (auto i = 1u; i < used_conn; i++)
		NET_DestroyStreamSocket((NET_StreamSocket*) conn[i]);

	// Close the server's master socket
	NET_DestroyServer(serv);
}

void client(const char *host, Uint16 port) {

	Uint8 myId = 0;
	Uint32 client_states[MAX_CLIENT];
	bool available_clients[MAX_CLIENT] = { };

	// The address of the server we want to connect to
	NET_Address *addr = NET_ResolveHostname(host);
	if (NET_WaitUntilResolved(addr, 1000) != NET_SUCCESS) {
		error();
	}

	// Connect to the server
	NET_StreamSocket *conn = NET_CreateClient(addr, port);
	if (NET_WaitUntilConnected(conn, 1000) != NET_SUCCESS) {
		error();
	}

	// - wait for the server to send acceptance message
	if (NET_WaitUntilInputAvailable(reinterpret_cast<void**>(&conn), 1, 1000)
			> 0) {
		SDLNetUtils::buff_t buf = SDLNetUtils::receive(conn);
		// forward it
		if (!buf.error) {
			if (buf.size > 0) {
				Msg m;
				m.deserialize(buf.data);

				if (m.id == _REQ_ACC) {
					MsgWithClientId m;
					m.deserialize(buf.data);
					myId = m.clientId;
					client_states[myId] = rand() % 1000;
					available_clients[myId] = true;
				} else {
					done = true;
					std::cout << "Rejected!" << std::endl;
				}
			} else {
				done = true;
				std::cout << "Something went wrong" << std::endl;
			}
		} else {
			done = true;
			std::cout << "Something went wrong" << std::endl;
		}
	} else {
		done = true;
		std::cout << "Did not get acceptance message" << std::endl;
	}

	bool send_my_state = true;
	bool some_state_changed = true;
	while (!done) {

		if (done)
			continue;

		// send my state if needed
		if (send_my_state) {
			StateMsg m;
			m.id = _STATE;
			m.clientId = myId;
			m.state = client_states[myId];
			SDLNetUtils::serialized_send(m, conn);
			send_my_state = false;
		}

		if (some_state_changed) {
			// for Linux use clear, for window use CLS
			system("CLS"); // system("clear");
			cout << "myId=" << (int)myId << endl;
			for (auto i = 0u; i < MAX_CLIENT; i++) {
				if (available_clients[i]) {
					cout << (int)i << ":" << client_states[i] << " ";
				}
			}
			cout << endl;
			some_state_changed = false;
		}

		// - wait for the server to send acceptance message
		if (NET_WaitUntilInputAvailable(reinterpret_cast<void**>(&conn), 1,
				1000) > 0) {

			if (done)
				continue;

			SDLNetUtils::buff_t buf = SDLNetUtils::receive(conn);
			if (!buf.error) {
				if (buf.size > 0) {
					Msg m;
					m.deserialize(buf.data);
					switch (m.id) {
					case _SEND_STATE:
						send_my_state = true;
						break;
					case _STATE: {
						StateMsg m;
						m.deserialize(buf.data);
						client_states[m.clientId] = m.state;
						available_clients[m.clientId] = true;
						some_state_changed = true;
						break;
					}
					case _CLIENT_DISCONNECTED: {
						MsgWithClientId m;
						m.deserialize(buf.data);
						available_clients[m.clientId] = false;
						some_state_changed = true;
						break;
					}
					}
				}
			} else {
				done = true;
				std::cout << "Looks like server closed connection" << std::endl;
				continue;
			}
		}

		// change state every now and then
		if (rand() % 100 < 10) {
			// change state
			client_states[myId] = rand() % 1000;
			send_my_state = true;
			some_state_changed = true;
		}


	}

// UnReference the address so it can be freed
	NET_UnrefAddress(addr);

// close the connection
	NET_DestroyStreamSocket(conn);
}

int main(int argc, char **argv) {

	srand(time(0));

	// initialize SDLNet
	if (!NET_Init()) {
		error();
	}

	if (argc == 3 && strcmp(argv[1], "server") == 0) {
		server(static_cast<Uint16>(atoi(argv[2]))); // start in server mode
	} else if (argc == 4 && strcmp(argv[1], "client") == 0) {
		client(argv[2], static_cast<Uint16>(atoi(argv[3]))); // start in client mode
	} else {
		cout << "Usage: " << endl;
		cout << "  " << argv[0] << " client host port " << endl;
		cout << "  " << argv[0] << " server port " << endl;
		cout << endl;
		cout << "Example:" << endl;
		cout << "  " << argv[0] << " server 2000" << endl;
		cout << "  " << argv[0] << " client localhost 2000" << endl;
	}

	// finalize SDLNet
	NET_Quit();

	return 0;
}
