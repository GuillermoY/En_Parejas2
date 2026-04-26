// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>
#include "SDLNetUtils.h"

enum MsgId : Uint8 {
	_CONN_REQ, _REQ_ACC, _REQ_REJ, _SEND_STATE, _CLIENT_DISCONNECTED, _STATE
};

struct Msg {
	Uint8 id;

	_IMPL_SERIALIZATION_(id)
};

struct MsgWithClientId: Msg {
	Uint8 clientId;

	_IMPL_SERIALIZATION_(*static_cast<Msg*>(this),clientId)
};

struct StateMsg: MsgWithClientId {
	Uint32 state;
	_IMPL_SERIALIZATION_(*static_cast<MsgWithClientId*>(this),state)
};
