// This file is part of the course TPV2@UCM - Samir Genaim

#include "ImageWithFrames.h"

#include <cassert>
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/macros.h"
#include "Transform.h"

ImageWithFrames::ImageWithFrames() :
	_tr(nullptr),
	_tex(nullptr),
	_numFrames(1),
	_frameW(0),
	_frameH(0),
	_framesPerRow(1),
	_currFrame(0),
	_lastFrameTime(0) {
}

ImageWithFrames::ImageWithFrames(const Texture* tex, int numFrames, int frameW, int frameH, int framesPerRow) :
	_tr(nullptr),
	_tex(tex),
	_numFrames(numFrames),
	_frameW(frameW),
	_frameH(frameH),
	_framesPerRow(framesPerRow),
	_currFrame(0),
	_lastFrameTime(0) {
}

ImageWithFrames::~ImageWithFrames() {
}

void ImageWithFrames::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
	assert(_tex != nullptr);
	_lastFrameTime = sdlutils().virtualTimer().currTime();
}

void ImageWithFrames::update() {
	uint64_t now = sdlutils().virtualTimer().currTime();
	if (now - _lastFrameTime >= _FRAME_INTERVAL) {
		_currFrame = (_currFrame + 1) % _numFrames;
		_lastFrameTime = now;
	}
}

void ImageWithFrames::render() {
	assert(_tex != nullptr);

	int col = _currFrame % _framesPerRow;
	int row = _currFrame / _framesPerRow;

	SDL_FRect src = {
		static_cast<float>(col * _frameW),
		static_cast<float>(row * _frameH),
		static_cast<float>(_frameW),
		static_cast<float>(_frameH)
	};

	SDL_FRect dest = build_sdlfrect(_tr->getPos(), _tr->getWidth(), _tr->getHeight());

	_tex->render(src, dest, _tr->getRot());
}