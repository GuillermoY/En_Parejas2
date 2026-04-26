// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL_net.h>
#include <SDL_stdinc.h>
#include <cassert>
#include <csignal>
#include <cstdlib>
#include <cstring>

#include "messages.h"
#include "TCPServer.h"

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


void server(Uint16 port) {
	TCPServer server(10);

	if (server.connect(port)) {
		server.listen();
	}
}

void client(const char *host, Uint16 port) {

	Msg m0;
	MsgWithMasterId m1;

	Uint8 myId = 0;

	// The address of the server we want to connect to
	NET_Address *addr = NET_ResolveHostname(host);
	if (NET_WaitUntilResolved(addr, 1000) != NET_SUCCESS) {
		error();
	}

	// Connect to the server
	NET_StreamSocket *sock = NET_CreateClient(addr, port);
	if (NET_WaitUntilConnected(sock, 1000) != NET_SUCCESS) {
		error();
	}

	void *conn[1];
	conn[0] = sock;

	// - wait for the server to send acceptance message
	if (NET_WaitUntilInputAvailable(conn, 1, 1000) > 0) {
		SDLNetUtils::buff_t buf = SDLNetUtils::receive(sock);
		// forward it
		if (!buf.error) {
			if (buf.size > 0) {
				m0.deserialize(buf.data);

				if (m0.type == _CONN_REQUEST_ACCEPTED) {
					m1.deserialize(buf.data);
					myId = m1.clientId;
					std::cout << "Connected with id " << (int) m1.clientId
							<< " (master is " << (int) m1.masterId << ")"
							<< std::endl;
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

	while (!done) {

		if (done)
			continue;

		// - wait for the server to send acceptance message
		if (NET_WaitUntilInputAvailable(reinterpret_cast<void**>(&sock), 1,
				1000) > 0) {

			if (done)
				continue;

			SDLNetUtils::buff_t buf = SDLNetUtils::receive(sock);
			if (!buf.error) {
				if (buf.size > 0) {
					Msg m;
					m.deserialize(buf.data);
					switch (m.type) {
					case _CLIENT_CONNECTED: {
						m1.deserialize(buf.data);
						std::cout << "New client " << (int) m1.clientId
								<< " connected (master is " << (int) m1.masterId << ")"
								<< std::endl;
					}
						break;
					case _CLIENT_DISCONNECTED: {
						m1.deserialize(buf.data);
						std::cout << "Client " << (int) m1.clientId
								<< " disconnected (master is " << (int) m1.masterId << ")"
								<< std::endl;
					}
						break;
					default:
						break;
					}
				}
			} else {
				done = true;
				std::cout << "Looks like server closed connection" << std::endl;
				continue;
			}
		}

		// change state every now and then
		if (rand() % 1000 < 10) {
			done = true;
		}

	}

	// UnReference the address so it can be freed
	NET_UnrefAddress(addr);

	// close the connection
	NET_DestroyStreamSocket(sock);
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
