// This file is part of the course TPV2@UCM - Samir Genaim

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

	struct Point { float x; float y; };
	struct Hit { int tile; Point where; };
	struct Line { Point a; Point b; };

	struct Gpu {
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;
		int xres;
		int yres;
	};

	struct Display {
		Uint32* pixels;
		int width;
		int pitch;
	};

	struct Wall { int top; int bot; float size; };

	enum PlayerState : Uint8 { NOT_USED, ALIVE, DEAD };

	struct Player {
		Uint8       id;
		Line        fov;
		Point       where;
		Point       velocity;
		float       speed;
		float       acceleration;
		float       theta;
		PlayerState state;
	};

	struct Map {
		Uint8** user_walling;
		Uint8** walling;
		Uint16 user_walling_width;
		Uint16 user_walling_height;
		Uint16 walling_width;
		Uint16 walling_height;

		Map() {
			user_walling = walling = nullptr;
			user_walling_width = user_walling_height = walling_width = walling_height = 0u;
		}

		~Map() {
			if (user_walling != nullptr) {
				for (auto i = 0u; i < user_walling_height; i++) delete[] user_walling[i];
				delete[] user_walling;
			}
			if (walling != nullptr) {
				for (auto i = 0u; i < walling_height; i++) delete[] walling[i];
				delete[] walling;
			}
		}
	};

	LittleWolf();
	virtual ~LittleWolf();

	void load(std::string filename);
	bool addPlayer(Uint8 id);
	void init(SDL_Window* window, SDL_Renderer* render);
	void render();
	void update();

	int get_xres() { return _xres; }
	int get_yres() { return _yres; }

	// ---- Métodos de red ----

	// Envía el estado del jugador local al servidor
	void send_my_info();

	// Recibe la posición de otro jugador; si es master valida el movimiento
	void update_player_state(Uint8 id, float x, float y, float vx, float vy,
		float theta, float prev_x, float prev_y, bool i_am_master);

	// Recibe info completa de otro jugador (al conectar)
	void update_player_info(Uint8 id, float x, float y, float vx, float vy,
		float theta, PlayerState state);

	// Master: procesa un disparo de un jugador remoto
	void process_shoot(Uint8 shooter_id, float x, float y, float theta);

	// Todos: aplica la muerte de un jugador
	void kill_player(Uint8 id, Uint8 shooter);

	// Elimina un jugador que se desconectó
	void remove_player(Uint8 id);

	// Master: resetea un jugador a una posición nueva
	void reset_player(Uint8 id, float x, float y, float theta);

	// Recibe la cuenta atrás del master
	void set_countdown(Uint8 seconds);

private:
	// Sube a todos los jugadores vivos (reset)
	void bringAllToLife();

	// Cambia al siguiente jugador vivo
	void switchToNextPlayer();

	// Silencia/activa sonido
	void muteSound();

	// Master: comprueba si quedan menos de 2 vivos y gestiona el restart
	void check_restart_condition();

	// Master: lanza el restart enviando posiciones a todos
	void launch_restart();

	// Raycasting auxiliares (sin cambios respecto al original)
	Wall project(const int xres, const int yres, const float focal, const Point corrected);
	Hit  cast(const Point where, Point direction, Uint8** walling, bool ignore_players, bool ignore_deads);
	bool shoot_local(Player& p);
	inline void spin(Player& p);
	void move(Player& p);
	void render_map(Player& p);
	void render_upper_view();
	void render_players_info();
	void render_countdown();

	// --- Helpers de vectores (igual que en el original) ---
	inline Line  viewport(float focal) { return { {focal,-1.f},{focal,+1.f} }; }
	inline Point turn(Point a, float t) { return { a.x * cosf(t) - a.y * sinf(t), a.x * sinf(t) + a.y * cosf(t) }; }
	inline Point rag(Point a) { return { -a.y, a.x }; }
	inline Point sub(Point a, Point b) { return { a.x - b.x, a.y - b.y }; }
	inline Point add(Point a, Point b) { return { a.x + b.x, a.y + b.y }; }
	inline Point mul(Point a, float n) { return { a.x * n, a.y * n }; }
	inline float mag(Point a) { return sqrtf(a.x * a.x + a.y * a.y); }
	inline Point unit(Point a) { return mul(a, 1.f / mag(a)); }
	inline float slope(Point a) { return a.y / a.x; }
	inline Line  rotate(Line l, float t) { return { turn(l.a,t), turn(l.b,t) }; }
	inline Point lerp(Line l, float n) { return add(l.a, mul(sub(l.b, l.a), n)); }
	inline int   fl(float x) { return (int)x - (x < (int)x); }
	inline int   cl(float x) { return (int)x + (x > (int)x); }
	inline Uint8 tile(Point a, Uint8** tiles) { return tiles[(int)a.y][(int)a.x]; }
	inline float dec(float x) { return x - (int)x; }
	inline Point sh(Point a, Point b) {
		float x = b.x > 0.f ? fl(a.x + 1.f) : cl(a.x - 1.f);
		return { x, slope(b) * (x - a.x) + a.y };
	}
	inline Point sv(Point a, Point b) {
		float y = b.y > 0.f ? fl(a.y + 1.f) : cl(a.y - 1.f);
		return { (y - a.y) / slope(b) + a.x, y };
	}
	inline Display lock(Gpu gpu) {
		void* screen; int pitch;
		SDL_LockTexture(gpu.texture, NULL, &screen, &pitch);
		return { (Uint32*)screen, pitch / (int)sizeof(Uint32), pitch };
	}
	inline void put(Display d, int x, int y, Uint32 px) { d.pixels[y + x * d.width] = px; }
	inline void unlock(Gpu gpu) { SDL_UnlockTexture(gpu.texture); }

	inline Uint32 color(Uint8 t) {
		switch (t) {
		case 1: return 0xAAAA0000;
		case 2: return 0x0000AA00;
		case 3: return 0x000000AA;
		case 4: return 0x00FFFF00;
		case 5: return 0x0000FFFF;
		case 6: return 0x00FF00FF;
		case 7: return 0x00C0C0C0;
		case 8: return 0x00808080;
		case 9: return 0x00800000;
		case 10: return 0x00808000;
		case 11: return 0x00008000;
		case 12: return 0x00800080;
		case 13: return 0x00008080;
		case 14: return 0x00000080;
		default: return 0x00f260b0;
		}
	}
	inline Uint32 color_rgba(Uint8 t) { Uint32 c = color(t); return (c << 8) | (c >> 24 & 0xff); }
	inline Uint8  player_to_tile(Uint8 id) { return id + 10; }
	inline Uint8  tile_to_player(Uint8 t) { assert(t >= 10); return t - 10; }

	// --- Campos ---
	bool  _show_help;
	static constexpr Uint8 _max_player = 10;
	int   _xres, _yres;
	static constexpr Uint8 _walling_size_factor = 2;
	Uint16 _walling_width, _walling_height;
	float  _shoot_distace;
	Map    _map;
	Player _players[_max_player];
	Uint8  _curr_player_id;   // jugador local
	Uint8  _local_id;         // id asignado por el servidor
	Gpu    _gpu;
	bool   _mute;

	// Cuenta atrás para restart
	Uint8  _countdown;        // segundos restantes (0 = sin cuenta atrás)
	bool   _in_countdown;     // true = en cuenta atrás, nadie puede moverse
	Uint32 _last_countdown_t; // última vez que bajó 1 segundo (ms)
	Uint32 _countdown_start;  // cuando empezó la cuenta atrás (para el master)
};