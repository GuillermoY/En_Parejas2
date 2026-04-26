// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL_net.h>
#include <SDL_stdinc.h>
#include <cassert>
#include <csignal>
#include <cstring>

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
	cout << "Starting server at port " << port << endl;

	// Trap for CTRL-C -- it will call handlerSIGINT which in turn calls shutdown
	signal(SIGINT, handlerSIGINT);

	// Some auxiliary variables for sending/receiving information
	char buffer[256];
	int result = 0;

	NET_Server *serv = NET_CreateServer(nullptr, port);
	if (!serv) {
		error();
	}

	// Store the port so it can be used by shutdown
	shutDownTriggetPort = port;

	cout << "The server is up and kicking ..." << endl;

	done = false;
	while (!done) {

		// Sleep for 60sec, or until someone connects to the server
		if (NET_WaitUntilInputAvailable(reinterpret_cast<void**>(&serv), 1,
				60000) > 0) {

			// in case of shutdown wake-up
			if (done)
				continue;

			NET_StreamSocket *client = nullptr;

			// Since we have activity in 'serv', we accept the connection
			if (NET_AcceptClient(serv, &client) && client != nullptr) {

				cout << "A client has connected, waiting for a message ..."
						<< endl;

				// Wait for the client to send a message
				if (NET_WaitUntilInputAvailable(
						reinterpret_cast<void**>(&client), 1, 60000) > 0) {

					// Since there was activity in 'client', we read the data. We do it
					// in a loop just in case it is more that 254 chars
					std::cout << "Client says : ";
					while ((result = NET_ReadFromStreamSocket(client, buffer,
							254)) > 0) {
						buffer[result] = 0; // end of char*
						// print the user message
						cout << buffer;
					}
					cout << endl;

					// - send back the 9 chars c-string "Received!", plus one for \0
					// - normally we should check errors afterward sending, but we skip it here
					//   for simplicity
					NET_WriteToStreamSocket(client, "Received!", 9);
				}
				// close the socket, in a later example we will see how to keep it open, etc.
				NET_DestroyStreamSocket(client);
			}
		}
	}

	// close the server's master socket
	NET_DestroyServer(serv);
}

void client(const char *host, Uint16 port) {

	// some auxiliary variables for sending/receiving information
	char buffer[256];
	int result = 0;

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

	// - ask the user for a message
	cout << "Enter a message: ";
	cin.getline(buffer, 255);

	// - Send the message to the server
	if (!NET_WriteToStreamSocket(conn, buffer, strlen(buffer))) {
		error();
	}

	// - wait for the server's response
	if (NET_WaitUntilInputAvailable(reinterpret_cast<void**>(&conn), 1, 1000)
			> 0) {
		while ((result = NET_ReadFromStreamSocket(conn, buffer, 254)) > 0) {
			buffer[result] = 0;
			cout << buffer;
		}
		cout << endl;
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
