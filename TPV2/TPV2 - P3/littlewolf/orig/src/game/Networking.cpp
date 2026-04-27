// This file is part of the course TPV2@UCM - Samir Genaim

#include "Networking.h"

#include <cassert>
#include <iostream>

#include "../sdlutils/SDLUtils.h"
#include "Game.h"
#include "LittleWolf.h"

Networking::Networking() : _sock(nullptr), _clientId(0), _masterId(0) {}

Networking::~Networking() {
	if (_sock != nullptr)
		NET_DestroyStreamSocket(_sock);
}

bool Networking::init(const char* host, Uint16 port) {
	bool success = false;

	NET_Address* addr = NET_ResolveHostname(host);
	if (NET_WaitUntilResolved(addr, 1000) != NET_SUCCESS) {
		NET_UnrefAddress(addr);
		return false;
	}

	_sock = NET_CreateClient(addr, port);
	if (NET_WaitUntilConnected(_sock, 1000) != NET_SUCCESS) {
		NET_DestroyStreamSocket(_sock);
		NET_UnrefAddress(addr);
		_sock = nullptr;
		return false;
	}

	NET_UnrefAddress(addr);

	void* conn[1];
	conn[0] = _sock;

	Msg m0;
	MsgWithMasterId m1;

	if (NET_WaitUntilInputAvailable(conn, 1, 1000) > 0) {
		SDLNetUtils::buff_t buf = SDLNetUtils::receive(_sock);
		if (!buf.error && buf.size > 0) {
			m0.deserialize(buf.data);
			if (m0.type == _CONN_REQUEST_ACCEPTED) {
				m1.deserialize(buf.data);
				_clientId = m1.clientId;
				_masterId = m1.masterId;
				success = true;
				std::cout << "Connected with id " << (int)_clientId
					<< " (master is " << (int)_masterId << ")" << std::endl;
			}
			else {
				std::cout << "Connection rejected!" << std::endl;
			}
		}
	}
	else {
		std::cout << "No reply from server." << std::endl;
	}

	return success;
}

void Networking::disconnect() {
	if (_sock == nullptr) return;
	NET_DestroyStreamSocket(_sock);
	_sock = nullptr;
}

void Networking::update() {
	assert(_sock != nullptr);

	Msg              m0;
	MsgWithMasterId  m1;
	PlayerStateMsg   m2;
	PlayerInfoMsg    m3;
	ShootMsg         m4;
	DeadMsg          m5;
	RestartPlayerMsg m6;
	CountdownMsg     m7;

	while (true) {
		SDLNetUtils::buff_t buf = SDLNetUtils::receive(_sock);

		if (buf.error) {
			std::cout << "Connection to server lost!" << std::endl;
			SDLNetUtils::print_SDLNet_error_and_exit(1);
		}

		if (buf.size == 0) break;

		m0.deserialize(buf.data);

		switch (m0.type) {
		case _CLIENT_CONNECTED:
			m1.deserialize(buf.data);
			_masterId = m1.masterId;
			handle_new_client(m1.clientId);
			break;
		case _CLIENT_DISCONNECTED:
			m1.deserialize(buf.data);
			_masterId = m1.masterId;
			handle_disconnect(m1.clientId);
			break;
		case _PLAYER_STATE:
			m2.deserialize(buf.data);
			handle_player_state(m2);
			break;
		case _PLAYER_INFO:
			m3.deserialize(buf.data);
			handle_player_info(m3);
			break;
		case _SHOOT:
			m4.deserialize(buf.data);
			handle_shoot(m4);
			break;
		case _DEAD:
			m5.deserialize(buf.data);
			handle_dead(m5);
			break;
		case _CORRECTION:
			m3.deserialize(buf.data);
			handle_correction(m3);
			break;
		case _RESTART:
			m6.deserialize(buf.data);
			handle_restart_player(m6);
			break;
		case _RESTART_COUNTDOWN:
			m7.deserialize(buf.data);
			handle_countdown(m7);
			break;
		default:
			break;
		}
	}
}

// ----------------------------------------------------------------
// Envío
// ----------------------------------------------------------------

void Networking::send_state(float x, float y, float vx, float vy,
	float theta, float prev_x, float prev_y) {
	PlayerStateMsg m;
	m.type = _PLAYER_STATE;
	m.clientId = _clientId;
	m.x = x; m.y = y;
	m.vx = vx; m.vy = vy;
	m.theta = theta;
	m.prev_x = prev_x; m.prev_y = prev_y;
	SDLNetUtils::serialized_send(m, _sock);
}

void Networking::send_my_info(float x, float y, float vx, float vy,
	float theta, Uint8 state) {
	PlayerInfoMsg m;
	m.type = _PLAYER_INFO;
	m.clientId = _clientId;
	m.x = x; m.y = y;
	m.vx = vx; m.vy = vy;
	m.theta = theta;
	m.state = state;
	SDLNetUtils::serialized_send(m, _sock);
}

void Networking::send_shoot(float x, float y, float theta) {
	ShootMsg m;
	m.type = _SHOOT;
	m.clientId = _clientId;
	m.x = x; m.y = y;
	m.theta = theta;
	SDLNetUtils::serialized_send(m, _sock);
}

void Networking::send_dead(Uint8 deadId) {
	DeadMsg m;
	m.type = _DEAD;
	m.clientId = deadId;
	SDLNetUtils::serialized_send(m, _sock);
}

void Networking::send_correction(Uint8 id, float x, float y,
	float vx, float vy, float theta) {
	PlayerInfoMsg m;
	m.type = _CORRECTION;
	m.clientId = id;
	m.x = x; m.y = y;
	m.vx = vx; m.vy = vy;
	m.theta = theta;
	m.state = LittleWolf::ALIVE;
	SDLNetUtils::serialized_send(m, _sock);
}

void Networking::send_restart_player(Uint8 id, float x, float y, float theta) {
	RestartPlayerMsg m;
	m.type = _RESTART;
	m.clientId = id;
	m.x = x; m.y = y;
	m.theta = theta;
	SDLNetUtils::serialized_send(m, _sock);
}

void Networking::send_countdown(Uint8 seconds) {
	CountdownMsg m;
	m.type = _RESTART_COUNTDOWN;
	m.seconds = seconds;
	SDLNetUtils::serialized_send(m, _sock);
}

// ----------------------------------------------------------------
// Recepción
// ----------------------------------------------------------------

void Networking::handle_new_client(Uint8 id) {
	if (id != _clientId)
		Game::Instance()->get_littlewolf().send_my_info();
}

void Networking::handle_disconnect(Uint8 id) {
	Game::Instance()->get_littlewolf().remove_player(id);
}

void Networking::handle_player_state(const PlayerStateMsg& m) {
	if (m.clientId == _clientId) return;
	Game::Instance()->get_littlewolf().update_player_state(
		m.clientId, m.x, m.y, m.vx, m.vy, m.theta,
		m.prev_x, m.prev_y, is_master());
}

void Networking::handle_player_info(const PlayerInfoMsg& m) {
	if (m.clientId == _clientId) return;
	Game::Instance()->get_littlewolf().update_player_info(
		m.clientId, m.x, m.y, m.vx, m.vy, m.theta,
		static_cast<LittleWolf::PlayerState>(m.state));
}

void Networking::handle_shoot(const ShootMsg& m) {
	if (!is_master()) return;
	Game::Instance()->get_littlewolf().process_shoot(
		m.clientId, m.x, m.y, m.theta);
}

void Networking::handle_dead(const DeadMsg& m) {
	std::cout << "handle_dead called for id=" << (int)m.clientId << std::endl;
	Game::Instance()->get_littlewolf().kill_player(m.clientId);
}

void Networking::handle_correction(const PlayerInfoMsg& m) {
	// Corrección de posición enviada por el master
	Game::Instance()->get_littlewolf().update_player_info(
		m.clientId, m.x, m.y, m.vx, m.vy, m.theta,
		static_cast<LittleWolf::PlayerState>(m.state));
}

void Networking::handle_restart_player(const RestartPlayerMsg& m) {
	Game::Instance()->get_littlewolf().reset_player(
		m.clientId, m.x, m.y, m.theta);
}

void Networking::handle_countdown(const CountdownMsg& m) {
	Game::Instance()->get_littlewolf().set_countdown(m.seconds);
}