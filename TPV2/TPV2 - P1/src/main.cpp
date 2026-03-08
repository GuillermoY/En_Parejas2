// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include "game/Game.h"

int main(int, char**) {
	try {
		// Inicializar el Singleton de Game
		if (!Game::Init()) {
			std::cerr << "Error creating Game instance" << std::endl;
			return 1;
		}

		auto* g = Game::Instance();

		// Inicializar SDL
		if (!g->initSDL()) {
			Game::Release();
			return 1;
		}

		// Inicializar el juego (entidades, estados, etc.)
		g->initGame();

		// Bucle principal
		g->start();

		// Liberar
		Game::Release();

	}
	catch (const std::string& e) {
		std::cerr << e << std::endl;
	}
	catch (const char* e) {
		std::cerr << e << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception" << std::endl;
	}

	return 0;
}