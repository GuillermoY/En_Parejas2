// This file is part of the course TPV2@UCM - Samir Genaim

/*
 * The content of this class is based on https://glouw.com/2018/03/11/littlewolf.html
 */

#pragma once

#include <SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "../sdlutils/InputHandler.h"

class LittleWolf {
public:

	// a point in a 2D-plane
	struct Point {
		float x;
		float y;
	};

	// Used to represent a position where a ray hits, and which tile
	struct Hit {
		int tile;
		Point where;
	};

	// a line between two points
	struct Line {
		Point a;
		Point b;
	};

	// The information on the window/renderer, the size of the window, and a texture
	// that we use to draw the walls, etc.
	struct Gpu {
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;
		int xres;
		int yres;
	};

	// when we lock a texture, we get an array of pixels where we can draw
	struct Display {
		Uint32* pixels;
		int width;
		int pitch;
	};

	// representing a (project) wall
	struct Wall {
		int top;
		int bot;
		float size;
	};

	// the status of a player
	enum PlayerState : Uint8 {
		NOT_USED, ALIVE, DEAD
	};

	// player information
	struct Player {
		Uint8 id;            // the id
		Line fov;            // focal view
		Point where;         // current position (wrt. to the grid)
		Point velocity;      // current velocity
		float speed;         // maximum speed
		float acceleration;  // acceleration
		float theta;         // rotation (in rad)
		PlayerState state;   // the state
	};

	// Representing a map, the user_walling is the walling provided by the user, and
	// walling is a scaled up version
	struct Map {
		Uint8** user_walling;
		Uint8** walling;

		Uint16 user_walling_width;
		Uint16 user_walling_height;
		Uint16 walling_width;
		Uint16 walling_height;

		Map() {
			user_walling = walling = nullptr;
			user_walling_width = user_walling_height = walling_width =
				walling_height = 0u;
		}

		~Map() {
			if (user_walling != nullptr) {
				for (auto i = 0u; i < user_walling_height; i++) {
					delete[] user_walling[i];
				}
				delete[] user_walling;
			}
			if (walling != nullptr) {
				for (auto i = 0u; i < walling_height; i++) {
					delete[] walling[i];
				}
				delete[] walling;
			}
		}
	};

	LittleWolf();
	virtual ~LittleWolf();

	// load a map from a file
	void load(std::string filename);

	// add a new player with identifier <id>, returns false if the id is already occupied
	bool addPlayer(Uint8 id);

	// initialize the SDL window information
	void init(SDL_Window* window, SDL_Renderer* render);

	// render the walls, etc
	void render();

	// update the world, etc
	void update();

	int get_xres() { return _xres; }
	int get_yres() { return _yres; }

	// ---- Métodos de red ----

	// Envía el estado del jugador local
	void send_my_info();

	// Actualiza la posición de un jugador remoto; si somos master validamos
	void update_player_state(Uint8 id, float x, float y, float vx, float vy,
		float theta, float prev_x, float prev_y, bool i_am_master);

	// Recibe info completa de un jugador remoto (al conectar)
	void update_player_info(Uint8 id, float x, float y, float vx, float vy,
		float theta, PlayerState state);

	// Master: procesa un disparo remoto y decide si da
	void process_shoot(Uint8 shooter_id, float x, float y, float theta);

	// Aplica la muerte de un jugador (todos lo reciben del master)
	void kill_player(Uint8 id);

	// Elimina un jugador desconectado
	void remove_player(Uint8 id);

	// Master: resetea un jugador a nueva posición
	void reset_player(Uint8 id, float x, float y, float theta);

	// Recibe la cuenta atrás del master
	void set_countdown(Uint8 seconds);

private:

	// mark all (used) player alive
	void bringAllToLife();

	// switch to the view of the next player
	void switchToNextPlayer();

	// mute/unmute sound
	void muteSound();

	// Master: comprueba si quedan menos de 2 vivos
	void check_restart_condition();

	// Master: lanza el restart enviando posiciones nuevas
	void launch_restart();

	// Renderiza la cuenta atrás en pantalla
	void render_countdown();

	// Calculates wall size using the <corrected> ray to the wall.
	Wall project(const int xres, const int yres, const float focal,
		const Point corrected);

	// Casts a ray from <where> in unit <direction> until a <walling> tile is hit.
	Hit cast(const Point where, Point direction, Uint8** walling,
		bool ignore_players, bool ignore_deads);

	// Shoot handler — local player shoots, sends to server
	bool shoot(Player& p);

	// Spins the player
	inline void spin(Player& p);

	// Moves the player
	void move(Player& p);

	// Renders the entire scene from the current player perspective
	void render_map(Player& p);

	// Renders upper view (when dead)
	void render_upper_view();

	// Render player list
	void render_players_info();

	// ---- Vector helpers (sin cambios del original) ----

	inline Line viewport(float focal) {
		Line fov = { { focal, -1.0f }, { focal, +1.0f }, };
		return fov;
	}

	inline Point turn(const Point a, const float t) {
		const Point b = { a.x * cosf(t) - a.y * sinf(t), a.x * sinf(t)
				+ a.y * cosf(t) };
		return b;
	}

	inline Point rag(const Point a) {
		const Point b = { -a.y, a.x };
		return b;
	}

	inline Point sub(const Point a, const Point b) {
		const Point c = { a.x - b.x, a.y - b.y };
		return c;
	}

	inline Point add(const Point a, const Point b) {
		const Point c = { a.x + b.x, a.y + b.y };
		return c;
	}

	inline Point mul(const Point a, const float n) {
		const Point b = { a.x * n, a.y * n };
		return b;
	}

	inline float mag(const Point a) {
		return sqrtf(a.x * a.x + a.y * a.y);
	}

	inline Point unit(const Point a) {
		return mul(a, 1.0f / mag(a));
	}

	inline float slope(const Point a) {
		return a.y / a.x;
	}

	inline Line rotate(const Line l, const float t) {
		const Line line = { turn(l.a, t), turn(l.b, t) };
		return line;
	}

	inline Point lerp(const Line l, const float n) {
		return add(l.a, mul(sub(l.b, l.a), n));
	}

	inline int fl(const float x) {
		return (int)x - (x < (int)x);
	}

	inline int cl(const float x) {
		return (int)x + (x > (int) x);
	}

	inline Uint8 tile(const Point a, Uint8** tiles) {
		const int x = a.x;
		const int y = a.y;
		return tiles[y][x];
	}

	inline Display lock(const Gpu gpu) {
		void* screen;
		int pitch;
		SDL_LockTexture(gpu.texture, NULL, &screen, &pitch);
		const Display display = { (Uint32*)screen, pitch
				/ (int)sizeof(Uint32), pitch };
		return display;
	}

	inline void put(const Display display, const int x, const int y,
		const Uint32 pixel) {
		display.pixels[y + x * display.width] = pixel;
	}

	inline void unlock(const Gpu gpu) {
		SDL_UnlockTexture(gpu.texture);
	}

	inline Point sh(const Point a, const Point b) {
		const float x = b.x > 0.0f ? fl(a.x + 1.0f) : cl(a.x - 1.0f);
		const float y = slope(b) * (x - a.x) + a.y;
		const Point c = { x, y };
		return c;
	}

	inline Point sv(const Point a, const Point b) {
		const float y = b.y > 0.0f ? fl(a.y + 1.0f) : cl(a.y - 1.0f);
		const float x = (y - a.y) / slope(b) + a.x;
		const Point c = { x, y };
		return c;
	}

	inline float dec(const float x) {
		return x - (int)x;
	}

	inline Uint32 color_rgba(const Uint8 tile) {
		Uint32 c = color(tile);
		return (c << 8) | (c >> 24 & 0xff);
	}

	inline Uint32 color(const Uint8 tile) {
		switch (tile) {
		case 1:  return 0xAAAA0000;
		case 2:  return 0x0000AA00;
		case 3:  return 0x000000AA;
		case 4:  return 0x00FFFF00;
		case 5:  return 0x0000FFFF;
		case 6:  return 0x00FF00FF;
		case 7:  return 0x00C0C0C0;
		case 8:  return 0x00808080;
		case 9:  return 0x00800000;
		case 10: return 0x00808000;
		case 11: return 0x00008000;
		case 12: return 0x00800080;
		case 13: return 0x00008080;
		case 14: return 0x00000080;
		default: return 0x00f260b0;
		}
	}

	inline Uint8 player_to_tile(Uint8 id) {
		return id + 10;
	}

	inline Uint8 tile_to_player(Uint8 tile) {
		assert(tile >= 10);
		return tile - 10;
	}

	// ---- Campos (igual que el original + campos de red) ----

	bool _show_help;

	static constexpr Uint8 _max_player = 10;

	int _xres;
	int _yres;

	static constexpr Uint8 _walling_size_factor = 2;

	Uint16 _walling_width;
	Uint16 _walling_height;

	float _shoot_distace;

	Map _map;

	Player _players[_max_player];

	Uint8 _curr_player_id;  // jugador cuya vista se muestra
	Uint8 _local_id;        // id asignado por el servidor

	Gpu _gpu;

	bool _mute;

	// Campos para cuenta atrás
	bool   _in_countdown;
	Uint8  _countdown;
	Uint32 _last_countdown_t;
};