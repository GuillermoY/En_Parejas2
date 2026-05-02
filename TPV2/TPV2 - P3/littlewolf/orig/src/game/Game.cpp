// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "LittleWolf.h"
#include "Networking.h"
#include "SDLNetUtils.h"

Game::Game() : _lw(nullptr), _net(nullptr) {}

Game::~Game() {
	delete _lw;
	delete _net;

	SDLNetUtils::close_SDLNet();

	if (InputHandler::HasInstance())
		InputHandler::Release();
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

bool Game::init() {
	return true; // requerido por Singleton
}

bool Game::init_game(const char* host, Uint16 port, const char* map, const char* name) {

	// Crear LittleWolf y cargar mapa primero (necesitamos la resolución)
	_lw = new LittleWolf();
	_lw->load(map);

	// Inicializar SDL
	if (!SDLUtils::Init(
		"[LittleWolf Net: " + std::string(map) + "]",
		_lw->get_xres(),
		_lw->get_yres(),
		"resources/config/littlewolf.resources.json")) {
		std::cerr << "Error initializing SDLUtils" << std::endl;
		return false;
	}

	if (!InputHandler::Init()) {
		std::cerr << "Error initializing InputHandler" << std::endl;
		return false;
	}

	// Inicializar red y conectar
	SDLNetUtils::init_SDLNet();
	_net = new Networking();
	if (!_net->init(host, port)) {
		std::cerr << "Could not connect to server" << std::endl;
		return false;
	}

	// Inicializar renderer y añadir jugador local
	_lw->init(sdlutils().window(), sdlutils().renderer());
	_lw->addPlayer(_net->get_client_id(), name);

	// Enviar info al resto
	_lw->send_my_info();

	return true;
}

void Game::start() {
	bool exit = false;
	auto& ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		ihdlr.refresh();

		if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		// Procesar mensajes de red entrantes
		_net->update();

		// Actualizar juego
		_lw->update();

		// Renderizar
		_lw->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

	_net->disconnect();
}