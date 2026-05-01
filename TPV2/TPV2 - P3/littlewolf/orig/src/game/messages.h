// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

/*
 * We put the definition of Message in a separated file just to
 * avoid circular dependencies with ecs.h (maybe there is a better
 * solution!).
 *
 * We do not put them in the ecs namspace to avoid the boredom
 * of writing the prefix ecs:: in the user classes.
 *
 * By default the struct Message is
 *
 *  struct Message {
 *	   uint8_t id;
 *  }
 *
 * and the user can redefine it in ../gemae/messages_defs.h (using
 * the same name Message).
 *
 * There are many ways how to define several messages, but keep it
 * simple! You only one struct called Message that has a field
 *
 *    uint8_t id
 *
 * to indicated the type of the message, and other fields to carry all
 * necessary information. If space matters for you, you can use
 * union types to save some space, but if you use union types then use
 * fields of primite types only, otherwise you will endup defining
 * constructors everywhere!
 *
 * For an example, see messages_defs_example.h
 *
 *
 * Never include your messages_defs.h directly in your program, always
 * use ecs/messages.h
 *
 */
#include <SDL_stdinc.h>
#include "SDLNetUtils.h"

enum MsgId : Uint8 {
	_CONN_REQUEST,
	_CONN_REQUEST_ACCEPTED,
	_CONN_REQUEST_REJECTED,
	_CLIENT_CONNECTED,
	_CLIENT_DISCONNECTED,
	_SERVER_SHUTDOWN,
	_PLAYER_STATE,        // jugador envía posición/vel/rot cada frame
	_PLAYER_INFO,         // info completa al conectar
	_SHOOT,               // jugador dispara, master decide
	_DEAD,                // master avisa quién murió
	_CORRECTION,          // master corrige posición inválida
	_RESTART,             // master envía posición de reset para un jugador
	_RESTART_COUNTDOWN,   // master envía cuenta atrás
	_DAMAGE				  // master avisa de daño recibido
};

struct Msg {
	Uint8 type;
	_IMPL_SERIALIZATION_(type)
};

struct MsgWithClientId : Msg {
	Uint8 clientId;
	_IMPL_SERIALIZATION_(*static_cast<Msg*>(this), clientId)
};

struct MsgWithMasterId : MsgWithClientId {
	Uint8 masterId;
	_IMPL_SERIALIZATION_(*static_cast<MsgWithClientId*>(this), masterId)
};

// Posición, velocidad y rotación cada frame + posición anterior
struct PlayerStateMsg : MsgWithClientId {
	float x, y;
	float vx, vy;
	float theta;
	float prev_x, prev_y;
	_IMPL_SERIALIZATION_(*static_cast<MsgWithClientId*>(this),
		x, y, vx, vy, theta, prev_x, prev_y)
};

// Info completa (estado incluido) — al conectar o corrección de posición
struct PlayerInfoMsg : MsgWithClientId {
	float x, y;
	float vx, vy;
	float theta;
	Uint8 state;
	_IMPL_SERIALIZATION_(*static_cast<MsgWithClientId*>(this),
		x, y, vx, vy, theta, state)
};

// Jugador dispara — master valida
struct ShootMsg : MsgWithClientId {
	float x, y;
	float theta;
	_IMPL_SERIALIZATION_(*static_cast<MsgWithClientId*>(this), x, y, theta)
};

struct DamageMsg : MsgWithClientId {
	float damage;
	_IMPL_SERIALIZATION_(*static_cast<MsgWithClientId*>(this), damage)
};

// Master avisa quién murió
struct DeadMsg : MsgWithClientId {
	_IMPL_SERIALIZATION_(*static_cast<MsgWithClientId*>(this))
};

// Master envía posición de reset para un jugador
struct RestartPlayerMsg : MsgWithClientId {
	float x, y, theta;
	_IMPL_SERIALIZATION_(*static_cast<MsgWithClientId*>(this), x, y, theta)
};

// Master envía segundos restantes de cuenta atrás
struct CountdownMsg : Msg {
	Uint8 seconds;
	_IMPL_SERIALIZATION_(*static_cast<Msg*>(this), seconds)
};

