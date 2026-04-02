// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_pixels.h>
#include <SDL_render.h>

#include "../ecs/Component.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"

struct RectangleViewer: ecs::Component {

	RectangleViewer() :
			RectangleViewer(build_sdlcolor(0xff0000ff), false) {
	}
	RectangleViewer(SDL_Color color, bool empty = false) :
			_color(color), _empty(empty) {
	}
	virtual ~RectangleViewer() {
	}

	inline void init(SDL_Color color, bool empty = false) {
		_color = color;
		_empty = empty;
	}

	void draw(SDL_Renderer *renderer, const SDL_FRect &dest) {
		SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(_color));
		if (_empty) {
			SDL_RenderRect(renderer, &dest);
		} else {
			SDL_RenderFillRect(renderer, &dest);
		}
	}

	SDL_Color _color;
	bool _empty;

};

