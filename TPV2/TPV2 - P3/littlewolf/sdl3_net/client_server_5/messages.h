// This file is part of the course TPV2@UCM - Samir Genaim


#pragma once
#include <SDL_stdinc.h>
#include "SDLNetUtils.h"

enum MsgId : Uint8 { _BINOP, _RANDOM, _SUM };

struct Msg {
	Uint8 id;

	_IMPL_SERIALIZATION_(id)
};

struct BinOpMSg : Msg {
	Uint8 op;
	Sint32 x;
	Sint32 y;

	_IMPL_SERIALIZATION_(*static_cast<Msg*>(this),op,x,y)
};

struct SumMsg : Msg {
	Uint8 size ;
	Sint32 nums[10];
	_IMPL_SERIALIZATION_(*static_cast<Msg*>(this),size,nums)
};

