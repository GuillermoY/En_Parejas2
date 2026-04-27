// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_stdinc.h>
#include "../utils/Singleton.h"

class LittleWolf;
class Networking;

class Game : public Singleton<Game> {
	friend Singleton<Game>;

public:
	bool init_game(const char* host, Uint16 port, const char* map);
	void start();

	LittleWolf& get_littlewolf() { return *_lw; }
	Networking& get_networking() { return *_net; }

private:
	Game();
	virtual ~Game();
	bool init(); // requerido por Singleton

	LittleWolf* _lw;
	Networking* _net;
};