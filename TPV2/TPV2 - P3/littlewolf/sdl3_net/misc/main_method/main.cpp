// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL_net.h>

using namespace std;

void error() {
	cout << "Error: " << SDL_GetError() << endl;
	exit(1);
}


void server(Uint16 port) {
	cout << "Starting server at port " << port << endl;

}

void client(const char *host, Uint16 port) {
}

int main(int argc, char **argv) {

	// initialize SDLNet
	if (NET_Init() < 0) {
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
