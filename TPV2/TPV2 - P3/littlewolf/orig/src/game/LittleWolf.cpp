// This file is part of the course TPV2@UCM - Samir Genaim

#include "LittleWolf.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <cstring>

#include "../json/JSONValue.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "Game.h"
#include "Networking.h"

LittleWolf::LittleWolf() :
	_show_help(true),
	_xres(), _yres(),
	_walling_width(), _walling_height(),
	_shoot_distace(),
	_map(), _players(),
	_curr_player_id(0), _local_id(0),
	_mute(false),
	_countdown(0), _in_countdown(false),
	_last_countdown_t(0), _countdown_start(0) {
}

LittleWolf::~LittleWolf() {}

// ================================================================
// init / load  (sin cambios respecto al original)
// ================================================================

void LittleWolf::init(SDL_Window* window, SDL_Renderer* render) {
	SDL_Texture* const texture = SDL_CreateTexture(
		sdlutils().renderer(), SDL_PIXELFORMAT_XRGB8888,
		SDL_TEXTUREACCESS_STREAMING, _yres, _xres);
	_gpu = { window, render, texture, _xres, _yres };
}

void LittleWolf::load(std::string filename) {
	std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile(filename));
	if (jValueRoot == nullptr || !jValueRoot->IsObject())
		throw "Something went wrong while loading '" + filename + "'";

	JSONObject root = jValueRoot->AsObject();
	JSONValue* jValue = nullptr;

	Uint16 uh = 0, uw = 0, sf = 0;
	jValue = root["scaling_factor"];
	sf = (jValue != nullptr && jValue->IsNumber()) ?
		static_cast<Uint16>(jValue->AsNumber()) : 2;

	if (sf % _walling_size_factor != 0)
		throw std::string("Scaling factor must be divisible by 2");

	jValue = root["walling"];
	JSONArray json_walling;
	if (jValue != nullptr && jValue->IsArray()) {
		json_walling = jValue->AsArray();
		uh = json_walling.size();
		assert(uh > 0);
		uw = json_walling[0]->AsString().size();
	}

	_xres = uw * sf;
	_yres = uh * sf;
	_walling_width = _xres / _walling_size_factor;
	_walling_height = _yres / _walling_size_factor;
	_shoot_distace = std::min(_walling_width, _walling_height) / 2;

	Uint8** walling = new Uint8 * [uh];
	for (auto i = 0u; i < uh; i++) {
		std::string row = json_walling[i]->AsString();
		const char* buf = row.c_str();
		walling[i] = new Uint8[uw];
		for (auto j = 0u; j < uw; j++)
			walling[i][j] = buf[j] - '0';
	}

	_map.user_walling = walling;
	_map.user_walling_width = uw;
	_map.user_walling_height = uh;

	_map.walling = new Uint8 * [_walling_height];
	_map.walling_height = _walling_height;
	_map.walling_width = _walling_width;
	for (auto i = 0u; i < _walling_height; i++) {
		_map.walling[i] = new Uint8[_walling_width];
		for (auto j = 0u; j < _walling_width; j++)
			_map.walling[i][j] = 1;
	}

	Uint16 cell_h = _walling_height / _map.user_walling_height;
	Uint16 cell_w = _walling_width / _map.user_walling_width;
	for (int i = 0; i < _map.user_walling_height; i++)
		for (int j = 0; j < _map.user_walling_width; j++)
			for (int k = 0; k < cell_h; k++)
				for (int l = 0; l < cell_w; l++)
					_map.walling[i * cell_h + k][j * cell_w + l] = _map.user_walling[i][j];
}

// ================================================================
// addPlayer — busca celda vacía y coloca al jugador
// ================================================================

bool LittleWolf::addPlayer(Uint8 id) {
	assert(id < _max_player);
	if (_players[id].state != NOT_USED) return false;

	auto& rand = sdlutils().rand();
	Uint16 orow = rand.nextInt(0, _map.walling_height);
	Uint16 ocol = rand.nextInt(0, _map.walling_width);
	Uint16 row = orow;
	Uint16 col = (ocol + 1) % _map.walling_width;

	while (!((orow == row) && (ocol == col)) && _map.walling[row][col] != 0) {
		col = (col + 1) % _map.user_walling_width;
		if (col == 0) row = (row + 1) % _map.walling_height;
	}
	if (row >= _map.walling_height) return false;

	Player p = { id, viewport(0.8f), {col + 0.5f, row + 0.5f},
				 {0.f,0.f}, 2.f, 0.9f, 0.f, ALIVE };
	_map.walling[(int)p.where.y][(int)p.where.x] = player_to_tile(id);
	_players[id] = p;
	_curr_player_id = id;
	_local_id = id;
	return true;
}

// ================================================================
// update — bucle principal local
// ================================================================

void LittleWolf::update() {
	auto& ihdlr = ih();
	auto  currTime = sdlutils().currRealTime();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDL_SCANCODE_T))
			_show_help = !_show_help;
		if (ihdlr.isKeyDown(SDL_SCANCODE_M))
			muteSound();
	}

	// Si estamos en cuenta atrás nadie puede moverse
	if (_in_countdown) {
		// El master actualiza la cuenta atrás cada segundo
		if (Game::Instance()->get_networking().is_master()) {
			if (currTime - _last_countdown_t >= 1000) {
				_last_countdown_t = currTime;
				if (_countdown > 0) {
					_countdown--;
					Game::Instance()->get_networking().send_countdown(_countdown);
				}
				if (_countdown == 0) {
					_in_countdown = false;
					launch_restart();
				}
			}
		}
		return;
	}

	Player& p = _players[_local_id];
	if (p.state != ALIVE) return;

	Point prev = p.where;
	spin(p);
	move(p);

	// Envía el estado del jugador local al servidor cada frame
	Game::Instance()->get_networking().send_state(
		p.where.x, p.where.y, p.velocity.x, p.velocity.y,
		p.theta, prev.x, prev.y);

	// Disparo — se envía al servidor, el master decide
	shoot_local(p);

	// El master comprueba si quedan menos de 2 vivos
	if (Game::Instance()->get_networking().is_master())
		check_restart_condition();
}

// ================================================================
// render
// ================================================================

void LittleWolf::render() {
	Player& p = _players[_curr_player_id];

	if (p.state == ALIVE)
		render_map(p);
	else
		render_upper_view();

	render_players_info();

	if (_in_countdown)
		render_countdown();

	if (_show_help) {
		int y = sdlutils().height();
		for (const char* s : { "usage_1","usage_2","usage_3",
								"usage_4","usage_5","usage_7","usage_8" }) {
			auto& t = sdlutils().msgs().at(s);
			y -= t.height();
			t.render(0, y);
		}
	}
}

// ================================================================
// Métodos de red
// ================================================================

void LittleWolf::send_my_info() {
	Player& p = _players[_local_id];
	Game::Instance()->get_networking().send_my_info(
		p.where.x, p.where.y, p.velocity.x, p.velocity.y,
		p.theta, static_cast<Uint8>(p.state));
}

void LittleWolf::update_player_state(Uint8 id, float x, float y,
	float vx, float vy, float theta,
	float prev_x, float prev_y, bool i_am_master) {

	Player& p = _players[id];
	if (p.state == NOT_USED) return;

	if (i_am_master) {
		// Validar: la celda anterior coincide y la nueva está libre
		int py0 = (int)prev_y, px0 = (int)prev_x;
		int py1 = (int)y, px1 = (int)x;
		bool prev_ok = (_map.walling[py0][px0] == player_to_tile(id)
			|| (_map.walling[py0][px0] == 0 && px0 == px1 && py0 == py1));
		bool new_ok = (_map.walling[py1][px1] == 0
			|| _map.walling[py1][px1] == player_to_tile(id));

		if (!prev_ok || !new_ok) {
			// Corrección: enviamos la posición que tenemos nosotros
			Game::Instance()->get_networking().send_my_info(
				p.where.x, p.where.y, p.velocity.x, p.velocity.y,
				p.theta, static_cast<Uint8>(p.state));
			return;
		}

		// Movimiento válido: actualizar walling
		if (px0 != px1 || py0 != py1) {
			_map.walling[py1][px1] = _map.walling[py0][px0];
			_map.walling[py0][px0] = 0;
		}
	}
	else {
		// No somos master: actualizamos walling directamente
		int py0 = (int)p.where.y, px0 = (int)p.where.x;
		int py1 = (int)y, px1 = (int)x;
		if (px0 != px1 || py0 != py1) {
			if (_map.walling[py0][px0] == player_to_tile(id))
				_map.walling[py0][px0] = 0;
			_map.walling[py1][px1] = player_to_tile(id);
		}
	}

	p.where = { x, y };
	p.velocity = { vx, vy };
	p.theta = theta;
}

void LittleWolf::update_player_info(Uint8 id, float x, float y,
	float vx, float vy, float theta, PlayerState state) {

	if (_players[id].state == NOT_USED && state != NOT_USED) {
		// Nuevo jugador remoto — lo añadimos
		Player p = { id, viewport(0.8f), {x,y}, {vx,vy}, 2.f, 0.9f, theta, state };
		_map.walling[(int)y][(int)x] = player_to_tile(id);
		_players[id] = p;
	}
	else {
		Player& p = _players[id];
		int py0 = (int)p.where.y, px0 = (int)p.where.x;
		int py1 = (int)y, px1 = (int)x;
		if ((px0 != px1 || py0 != py1) && _map.walling[py0][px0] == player_to_tile(id))
			_map.walling[py0][px0] = 0;
		_map.walling[py1][px1] = player_to_tile(id);
		p.where = { x,y }; p.velocity = { vx,vy };
		p.theta = theta; p.state = state;
	}
}

void LittleWolf::process_shoot(Uint8 shooter_id, float x, float y, float theta) {
	// El master simula el disparo y decide si da a alguien
	Player fake;
	fake.where = { x, y };
	fake.theta = theta;
	fake.fov = viewport(0.8f);
	fake.id = shooter_id;

	for (float d = -0.05f; d <= 0.05f; d += 0.005f) {
		const Line camera = rotate(fake.fov, fake.theta + d);
		Point direction = lerp(camera, 0.5f);
		float m = mag(direction);
		direction.x /= m; direction.y /= m;
		const Hit hit = cast(fake.where, direction, _map.walling, false, true);

		if (hit.tile > 9 && mag(sub(fake.where, hit.where)) < _shoot_distace) {
			Uint8 victim = tile_to_player(hit.tile);
			if (victim != shooter_id) {
				Game::Instance()->get_networking().send_dead(victim, shooter_id);
				return;
			}
		}
	}
}

void LittleWolf::kill_player(Uint8 id, Uint8 shooter) {
	(void)shooter;
	if (_players[id].state == ALIVE) {
		_players[id].state = DEAD;
		sdlutils().soundEffects().at("pain").play("se");
		// Si el muerto somos nosotros, cambiamos a vista aérea
		if (id == _local_id)
			_curr_player_id = _local_id;
	}
}

void LittleWolf::remove_player(Uint8 id) {
	if (_players[id].state == NOT_USED) return;
	int py = (int)_players[id].where.y;
	int px = (int)_players[id].where.x;
	if (_map.walling[py][px] == player_to_tile(id))
		_map.walling[py][px] = 0;
	_players[id].state = NOT_USED;
}

void LittleWolf::reset_player(Uint8 id, float x, float y, float theta) {
	Player& p = _players[id];
	if (p.state == NOT_USED) return;

	// Quitar del walling antiguo
	int py0 = (int)p.where.y, px0 = (int)p.where.x;
	if (_map.walling[py0][px0] == player_to_tile(id))
		_map.walling[py0][px0] = 0;

	// Colocar en nueva posición
	p.where = { x, y };
	p.velocity = { 0.f, 0.f };
	p.theta = theta;
	p.state = ALIVE;
	_map.walling[(int)y][(int)x] = player_to_tile(id);

	// Si es nuestro jugador, actualizamos curr_player_id
	if (id == _local_id)
		_curr_player_id = _local_id;
}

void LittleWolf::set_countdown(Uint8 seconds) {
	_countdown = seconds;
	_in_countdown = (seconds > 0);
	_last_countdown_t = sdlutils().currRealTime();
}

// ================================================================
// Private helpers
// ================================================================

void LittleWolf::check_restart_condition() {
	if (_in_countdown) return;

	int alive = 0;
	for (auto i = 0u; i < _max_player; i++)
		if (_players[i].state == ALIVE) alive++;

	if (alive < 2) {
		_in_countdown = true;
		_countdown = 5;
		_last_countdown_t = sdlutils().currRealTime();
		// Avisar a todos
		Game::Instance()->get_networking().send_countdown(_countdown);
	}
}

void LittleWolf::launch_restart() {
	// El master elige posiciones aleatorias para todos los jugadores activos
	for (auto i = 0u; i < _max_player; i++) {
		if (_players[i].state == NOT_USED) continue;

		// Buscar celda vacía
		auto& rand = sdlutils().rand();
		Uint16 orow = rand.nextInt(0, _map.walling_height);
		Uint16 ocol = rand.nextInt(0, _map.walling_width);
		Uint16 row = orow, col = (ocol + 1) % _map.walling_width;

		while (!((orow == row) && (ocol == col)) && _map.walling[row][col] != 0) {
			col = (col + 1) % _map.user_walling_width;
			if (col == 0) row = (row + 1) % _map.walling_height;
		}

		float x = col + 0.5f, y = row + 0.5f;
		float theta = 0.f;

		// Aplicar localmente
		reset_player(i, x, y, theta);

		// Enviar al resto
		Game::Instance()->get_networking().send_restart_player(i, x, y, theta);
	}
}

void LittleWolf::bringAllToLife() {
	for (auto i = 0u; i < _max_player; i++)
		if (_players[i].state == DEAD)
			_players[i].state = ALIVE;
}

void LittleWolf::switchToNextPlayer() {
	int j = (_curr_player_id + 1) % _max_player;
	while (j != _curr_player_id && _players[j].state == NOT_USED)
		j = (j + 1) % _max_player;
	_curr_player_id = j;
}

void LittleWolf::muteSound() {
	_mute = !_mute;
	SoundManager::Instance()->set_master_volume(_mute ? 0.f : 1.f);
}

// ================================================================
// shoot_local — disparo del jugador local, envía al servidor
// ================================================================

bool LittleWolf::shoot_local(Player& p) {
	auto& ihdlr = ih();
	if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_SPACE)) {
		sdlutils().soundEffects().at("gunshot").play("se");
		// El jugador avisa que disparó — el master decide si da
		Game::Instance()->get_networking().send_shoot(
			p.where.x, p.where.y, p.theta);
		return true;
	}
	return false;
}

// ================================================================
// spin / move (igual que el original)
// ================================================================

void LittleWolf::spin(Player& p) {
	auto& ihdlr = ih();
	float d = ihdlr.isKeyDown(SDL_SCANCODE_LSHIFT) ? 0.005f : 0.05f;
	if (ihdlr.isKeyDown(SDL_SCANCODE_H)) p.theta -= d;
	if (ihdlr.isKeyDown(SDL_SCANCODE_L)) p.theta += d;
}

void LittleWolf::move(Player& p) {
	auto& ihdlr = ih();
	const Point last = p.where, zero = { 0.f, 0.f };

	if (ihdlr.isKeyDown(SDL_SCANCODE_W) || ihdlr.isKeyDown(SDL_SCANCODE_S)
		|| ihdlr.isKeyDown(SDL_SCANCODE_D) || ihdlr.isKeyDown(SDL_SCANCODE_A)) {
		const Point ref = { 1.f, 0.f };
		const Point dir = turn(ref, p.theta);
		const Point acc = mul(dir, ihdlr.isKeyDown(SDL_SCANCODE_LSHIFT) ?
			p.acceleration / 100.f : p.acceleration);
		if (ihdlr.isKeyDown(SDL_SCANCODE_W)) p.velocity = add(p.velocity, acc);
		if (ihdlr.isKeyDown(SDL_SCANCODE_S)) p.velocity = sub(p.velocity, acc);
		if (ihdlr.isKeyDown(SDL_SCANCODE_D)) p.velocity = add(p.velocity, rag(acc));
		if (ihdlr.isKeyDown(SDL_SCANCODE_A)) p.velocity = sub(p.velocity, rag(acc));
	}
	else {
		p.velocity = mul(p.velocity, 1.f - p.acceleration / p.speed);
	}

	if (mag(p.velocity) > p.speed)
		p.velocity = mul(unit(p.velocity), p.speed);

	p.where = add(p.where, p.velocity);

	if (tile(p.where, _map.walling) != 10 + _local_id
		&& tile(p.where, _map.walling) != 0) {
		p.velocity = zero;
		p.where = last;
	}
	else {
		int y0 = (int)last.y, x0 = (int)last.x;
		int y1 = (int)p.where.y, x1 = (int)p.where.x;
		if (x0 != x1 || y0 != y1) {
			_map.walling[y1][x1] = _map.walling[y0][x0];
			_map.walling[y0][x0] = 0;
		}
	}
}

// ================================================================
// cast / project (igual que el original)
// ================================================================

LittleWolf::Hit LittleWolf::cast(const Point where, Point direction,
	Uint8** walling, bool ignore_players, bool ignore_deads) {

	const Point hor = sh(where, direction);
	const Point ver = sv(where, direction);
	const Point ray = mag(sub(hor, where)) < mag(sub(ver, where)) ? hor : ver;
	const Point dc = mul(direction, 0.01f);
	const Point dx = { dc.x, 0.f };
	const Point dy = { 0.f, dc.y };
	const Point test = add(ray,
		mag(sub(hor, ver)) < 1e-3f ? dc :
		dec(ray.x) == 0.f ? dx : dy);

	Uint8 t = tile(test, walling);

	if (ignore_players && t >= 10) {
		return cast(ray, direction, walling, ignore_players, ignore_deads);
	}
	if (ignore_deads && t >= 10) {
		Uint8 pid = tile_to_player(t);
		if (_players[pid].state == DEAD)
			return cast(ray, direction, walling, ignore_players, ignore_deads);
	}

	const Hit hit = { t, ray };
	return hit.tile ? hit : cast(ray, direction, walling, ignore_players, ignore_deads);
}

LittleWolf::Wall LittleWolf::project(const int xres, const int yres,
	const float focal, const Point corrected) {
	const float normal = corrected.x < 1e-2f ? 1e-2f : corrected.x;
	const float size = 0.5f * focal * xres / normal;
	const int   top = (yres + size) / 2.f;
	const int   bot = (yres - size) / 2.f;
	return { top > yres ? yres : top, bot < 0 ? 0 : bot, size };
}

// ================================================================
// render_map (igual que el original)
// ================================================================

void LittleWolf::render_map(Player& p) {
	const Display display = lock(_gpu);
	const Line    camera = rotate(p.fov, p.theta);

	for (int x = 0; x < _gpu.xres; x++) {
		const Point dir = lerp(camera, x / (float)_gpu.xres);
		const Hit   hit = cast(p.where, dir, _map.walling, true, false);
		const Point ray = sub(hit.where, p.where);
		const Point corr = turn(ray, -p.theta);
		const Wall  wall = project(_gpu.xres, _gpu.yres, p.fov.a.x, corr);

		for (int y = 0; y < wall.bot; y++) put(display, x, y, 0x00000000);
		for (int y = wall.bot; y < wall.top; y++) put(display, x, y, color(hit.tile));
		for (int y = wall.top; y < _gpu.yres;y++) put(display, x, y, 0x00000000);

		const Hit   hit_p = cast(p.where, dir, _map.walling, false, false);
		const Point ray_p = sub(hit_p.where, p.where);
		const Point corr1 = turn(ray_p, -p.theta);
		const Wall  wall_p = project(_gpu.xres, _gpu.yres, p.fov.a.x, corr1);

		if (hit_p.tile >= 10) {
			Uint8 pid = hit_p.tile - 10;
			int   size_p = (_players[pid].state == ALIVE) ?
				wall_p.top - (wall_p.top - wall_p.bot) / 3 :
				wall_p.bot + (wall_p.top - wall_p.bot) / 10;
			for (int y = wall_p.bot; y < size_p; y++)
				put(display, x, y, color(hit_p.tile));
		}
	}

	// Punto de mira
	for (int i = -10; i < 10; i++) {
		put(display, _gpu.xres / 2, _gpu.yres / 2 + i, 0xAAAAAAAA);
		put(display, _gpu.xres / 2 + i, _gpu.yres / 2, 0xAAAAAAAA);
	}

	unlock(_gpu);

	const SDL_FRect dst = build_sdlfrect(
		(_gpu.xres - _gpu.yres) / 2.f, (_gpu.yres - _gpu.xres) / 2.f, _gpu.yres, _gpu.xres);
	SDL_RenderTextureRotated(_gpu.renderer, _gpu.texture, NULL, &dst, -90, NULL, SDL_FLIP_NONE);
}

// ================================================================
// render_upper_view (igual que el original)
// ================================================================

void LittleWolf::render_upper_view() {
	const Display display = lock(_gpu);
	std::memset(display.pixels, 0, display.pitch * _xres);

	for (auto x = 0u; x < _map.walling_height; x++)
		for (auto y = 0u; y < _map.walling_width; y++)
			if (_map.walling[x][y] != 0)
				for (int i = 0; i < _walling_size_factor; i++)
					for (int j = 0; j < _walling_size_factor; j++)
						put(display, y * _walling_size_factor + i,
							_gpu.yres - 1 - x * _walling_size_factor + j,
							color(_map.walling[x][y]));

	unlock(_gpu);

	const SDL_FRect dst = build_sdlfrect(
		(_gpu.xres - _gpu.yres) / 2.f, (_gpu.yres - _gpu.xres) / 2.f, _gpu.yres, _gpu.xres);
	SDL_RenderTextureRotated(_gpu.renderer, _gpu.texture, NULL, &dst, -90, NULL, SDL_FLIP_NONE);

	for (int i = 0u; i < _max_player; i++) {
		Player& p = _players[i];
		if (p.state == NOT_USED) continue;
		Texture info(sdlutils().renderer(), "P" + std::to_string(i),
			sdlutils().fonts().at("MFR12"),
			build_sdlcolor(color_rgba(i + 10)));
		int w = info.width(), h = info.height();
		SDL_FRect src = build_sdlfrect(0.f, 0.f, w, h);
		SDL_FRect dest = build_sdlfrect(p.where.x * 2 - w / 2.f, p.where.y * 2 - h / 2.f, w, h);
		info.render(src, dest, p.theta * 180 / 3.14159265f);
	}
}

// ================================================================
// render_players_info
// ================================================================

void LittleWolf::render_players_info() {
	uint_fast16_t y = 0;
	for (auto i = 0u; i < _max_player; i++) {
		PlayerState s = _players[i].state;
		if (s == NOT_USED) continue;
		std::string msg = (i == _curr_player_id ? "*P" : " P")
			+ std::to_string(i)
			+ (i == _local_id ? " (you)" : "")
			+ (s == DEAD ? " (dead)" : "");
		Texture info(sdlutils().renderer(), msg,
			sdlutils().fonts().at("MFR24"),
			build_sdlcolor(color_rgba(i + 10)));
		SDL_FRect dest = build_sdlfrect(0, y, info.width(), info.height());
		info.render(dest);
		y += info.height() + 5;
	}
}

// ================================================================
// render_countdown
// ================================================================

void LittleWolf::render_countdown() {
	std::string msg = "The game will restart in "
		+ std::to_string(_countdown) + " seconds";
	Texture t(sdlutils().renderer(), msg,
		sdlutils().fonts().at("MFR24"),
		build_sdlcolor(0xFFFF00FF));
	int x = (sdlutils().width() - t.width()) / 2;
	int y = (sdlutils().height() - t.height()) / 2;
	t.render(x, y);
}