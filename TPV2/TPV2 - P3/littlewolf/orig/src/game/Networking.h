// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>
#include "messages.h"

class Networking {
public:
	Networking();
	virtual ~Networking();

	bool init(const char* host, Uint16 port);
	void disconnect();
	void update();

	Uint8 get_client_id() const { return _clientId; }
	bool  is_master()     const { return _clientId == _masterId; }

	// Envía posición/vel/rot del jugador local cada frame
	void send_state(float x, float y, float vx, float vy,
		float theta, float prev_x, float prev_y);

	// Envía info completa del jugador local
	void send_my_info(float x, float y, float vx, float vy,
		float theta, Uint8 state);

	// Jugador dispara
	void send_shoot(float x, float y, float theta);

	// Master avisa de muerte
	void send_dead(Uint8 deadId);

	// Master corrige posición de un jugador
	void send_correction(Uint8 id, float x, float y,
		float vx, float vy, float theta);

	// Master envía posición de reset para un jugador
	void send_restart_player(Uint8 id, float x, float y, float theta);

	// Master envía cuenta atrás
	void send_countdown(Uint8 seconds);

	void send_damage(Uint8 id, float damage);

private:
	void handle_new_client(Uint8 id);
	void handle_disconnect(Uint8 id);
	void handle_player_state(const PlayerStateMsg& m);
	void handle_player_info(const PlayerInfoMsg& m);
	void handle_shoot(const ShootMsg& m);
	void handle_dead(const DeadMsg& m);
	void handle_correction(const PlayerInfoMsg& m);
	void handle_restart_player(const RestartPlayerMsg& m);
	void handle_countdown(const CountdownMsg& m);

	NET_StreamSocket* _sock;
	Uint8 _clientId;
	Uint8 _masterId;
};