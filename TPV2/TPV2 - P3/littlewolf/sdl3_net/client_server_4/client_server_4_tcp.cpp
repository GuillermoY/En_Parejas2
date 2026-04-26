// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL_net.h>
#include <SDL_stdinc.h>
#include <cassert>
#include <csignal>
#include <cstring>
#include "operations.h"
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

using msg_size_t = Uint16;

// Send a TCP message, adding a size header
bool send(NET_StreamSocket *conn, Uint8 *buffer, msg_size_t size) {

	bool result = false;

	if (NET_WriteToStreamSocket(conn, reinterpret_cast<Uint8*>(&size),
			sizeof(msg_size_t))) {
		result = NET_WriteToStreamSocket(conn, buffer, size);
	}

	return result;
}

// Reading a message from TCP, assuming there is a size header.
//
int receive(NET_StreamSocket *conn, Uint8 *buffer) {
	msg_size_t size = 0;
	int result = 0;

	// read the size of the message, the first sizeof(msgSize_t) bytes, into buffer
	result = NET_ReadFromStreamSocket(conn, &size, sizeof(msg_size_t));
	if (result != 0 && result != sizeof(msg_size_t))
		return -1;

	Uint16 bytes_read = 0;

	// receive size bytes
	while (bytes_read < size) {
		result = NET_ReadFromStreamSocket(conn, buffer + bytes_read,
				size - bytes_read);
		bytes_read += result;
	}

	return bytes_read;
}

void server(Uint16 port) {
	cout << "Starting server at port " << port << endl;

	// Trap for CTRL-C -- it will call handlerSIGINT which in turn calls shutdown
	signal(SIGINT, handlerSIGINT);

	// Some auxiliary variables for sending/receiving information
	Uint8 buffer[256];
	Msg *msg = reinterpret_cast<Msg*>(buffer);

	NET_Server *serv = NET_CreateServer(nullptr, port);
	if (!serv) {
		error();
	}

	// Store the port so it can be used by shutdown
	shutDownTriggetPort = port;

	cout << "The server is up and kicking ..." << endl;

	// An array of all connections. We will use it to check if there
	// is activity in any of them.
	constexpr std::size_t MAX_CLIENT = 10;
	void *conn[MAX_CLIENT];
	std::size_t used_conn = 1;

	// We store the server in the first position
	conn[0] = serv;

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

				if (used_conn < MAX_CLIENT) {
					conn[used_conn] = client;
					std::cout << "New client assigned id " << used_conn
							<< std::endl;
					++used_conn;
					send(client, (Uint8*) "ACCEPTED", 8);
				} else {
					send(client, (Uint8*) "FULLY BOOKED", 12);
				}
			}

			// Check activity in the clients sockets
			for (auto i = 1u; i < used_conn; i++) {

				// Read a message
				int n = receive((NET_StreamSocket*) conn[i], buffer);

				// if client disconnected
				if (n == -1) {
					std::cout << "Client " << i << " disconnected!"
							<< std::endl;
					conn[i] = conn[used_conn - 1];
					conn[used_conn - 1] = nullptr;
					--used_conn;
				} else if (n > 0) {
					Sint32 res = 0;
					std::cout << "Client '" << i << " requested operation "
							<< (int) msg->id << std::endl;
					switch (buffer[0]) {
					case 0:
						res = handle_binary_operation(buffer);
						break;
					case 1:
						res = handle_random_operation(buffer);
						break;
					case 2:
						res = handle_sum_operation(buffer);
						break;
					default:
						break;
					}
					send((NET_StreamSocket*) conn[i],
							reinterpret_cast<Uint8*>(&res), sizeof(Sint32));
				}

			}

		}
	}

	// Dispose all client sockets
	for (auto i = 1; i < used_conn; i++)
		NET_DestroyStreamSocket((NET_StreamSocket*) conn[i]);

	// Close the server's master socket
	NET_DestroyServer(serv);
}

void client(const char *host, Uint16 port) {

	// Some auxiliary variables for sending/receiving information
	Uint8 buffer[256];
	int size = 0;

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
		int n = receive(conn, buffer);
		if (n > 0) {
			buffer[n] = 0;
			if (strcmp((char*) buffer, "ACCEPTED") != 0) {
				done = true;
				std::cout << "Rejected: " << buffer << std::endl;

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

		Uint8 option = menu();

		switch (option) {
		case 0:
			size = binary_operation(buffer);
			break;
		case 1:
			size = random_operation(buffer);
			break;
		case 2:
			size = sum_operation(buffer);
			break;
		case 3:
			done = true;
			continue;
			break;
		default:
			assert(false);
		}

		// - Send the message to the server
		if (!send(conn, buffer, size)) {
			error();
		}

		// - wait for the server's response
		if (NET_WaitUntilInputAvailable(reinterpret_cast<void**>(&conn), 1,
				1000) > 0) {
			int n = receive(conn, buffer);
			if (n > 0)
				cout << "Result = " << *reinterpret_cast<Sint32*>(buffer) << endl;
		}

	}

// UnReference the address so it can be freed
	NET_UnrefAddress(addr);

// close the connection
	NET_DestroyStreamSocket(conn);
}

int main(int argc, char **argv) {

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
