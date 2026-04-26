// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL_net.h>
#include <csignal>
#include <cassert>
#include <string>
#include <unordered_map>
#include <thread>
#include <sstream>

using namespace std;

void error() {
	cout << "Error: " << SDL_GetError() << endl;
	exit(1);
}

// The port on which the server/client is listening, to be used by method
// shutdown to send an empty message to wake up the server/client if blocking
bool done = false;
Uint16 shutDownTriggetPort = 0;

// this just sent an empty message the server/client to wake it up.
void shutdown() {
	if (done)
		return;

	cout << endl << "Shutting down  ..." << endl;

	done = true;

	if (shutDownTriggetPort != 0) {

		// A datagram socket for sending tha wake-up packaet
		NET_DatagramSocket *conn = NET_CreateDatagramSocket(nullptr, 0);

		// Resolve the hostname to an IP address
		NET_Address *addr = NET_ResolveHostname("localhost");
		if (NET_WaitUntilResolved(addr, 1000) != NET_SUCCESS) {
			error();
		}

		char buffer[1] = { 0 };

		NET_SendDatagram(conn, addr, shutDownTriggetPort, buffer, 1);

		// We have to UnReference the address, so it is freed
		NET_UnrefAddress(addr);

		// Close the datagram socket
		NET_DestroyDatagramSocket(conn);
	}
}

/* Signal Handler for SIGINT */
void handlerSIGINT(int) {
	shutdown();
}

void server(Uint16 port) {

	// trap for CTRL-C -- it will call handlerSIGINT which in turn calls shutdown
	signal(SIGINT, handlerSIGINT);


	// The datagram socket that the server uses to send/recieve messages
	NET_DatagramSocket *conn = NET_CreateDatagramSocket(nullptr, port);

	// store the port so it can be used by shutdown
	shutDownTriggetPort = port;

	cout << "The server is up and kicking ..." << endl;

	done = false;

	while (!done) {

		// in case of a shutdown wake-up
		if (done)
			continue;

		// Sleep for 60sec, or until someone connects to the server
		if (NET_WaitUntilInputAvailable(reinterpret_cast<void**>(&conn), 1,
				60000) > 0) {

			// Try to receive pending datagram packet(s)
			NET_Datagram *packet = nullptr;
			while (NET_ReceiveDatagram(conn, &packet) && packet != nullptr) {

				// Print the content
				std::cout << "Client " << NET_GetAddressString(packet->addr) << ":" << packet->port << " says : ";
				for (auto i = 0; i < packet->buflen; i++)
					std::cout << packet->buf[i];
				cout << endl;

				// Reply
				NET_SendDatagram(conn, packet->addr, packet->port, "Recieved!",9);

				// Destroy the packet
				NET_DestroyDatagram(packet);
			}
		}
	}

	// Destroy the socket
	NET_DestroyDatagramSocket(conn);
}


void client(char *host, Uint16 port) {
	// some auxiliary variables for sending/receiving information
	char buffer[256];

	NET_DatagramSocket *conn = NET_CreateDatagramSocket(NULL, 0);

	// This the server address
	NET_Address *addr = NET_ResolveHostname(host);
	if (NET_WaitUntilResolved(addr, 1000) != NET_SUCCESS) {
		error();
	}

	// Ask the user for a message
	cout << "Enter a message: ";
	cin.getline(buffer, 255);

	if ( !NET_SendDatagram(conn, addr, port, buffer, strlen(buffer)) ) {
		error();
	}

	// Sleep for 60sec, or until the reply is received
	if (NET_WaitUntilInputAvailable(reinterpret_cast<void**>(&conn), 1, 60000)
			> 0) {

		NET_Datagram *dgram = nullptr;
		while (NET_ReceiveDatagram(conn, &dgram) && dgram != nullptr) {
			std::cout << "Server Replied: ";
			for (auto i = 0; i < dgram->buflen; i++)
				std::cout << dgram->buf[i];
			cout << endl;
			NET_DestroyDatagram(dgram);
		}
	}

	// Unreference the address so it can be freed
	NET_UnrefAddress(addr);

	// Destry the socket
	NET_DestroyDatagramSocket(conn);

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
