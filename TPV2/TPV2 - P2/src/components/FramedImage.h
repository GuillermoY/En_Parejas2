// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL.h>
#include "../ecs/Component.h"

class Texture;

struct FramedImage : public ecs::Component {

	FramedImage() :
		_tex(nullptr), _frameW(0), _frameH(0), _cols(0),
		_firstFrame(0), _numFrames(0), _currFrame(0),
		_frameTime(0), _lastFrameTime(0) {
	}

	FramedImage(const Texture* tex, int frameW, int frameH, int cols,
		int firstFrame, int numFrames, Uint32 frameTime) :
		_tex(tex), _frameW(frameW), _frameH(frameH), _cols(cols),
		_firstFrame(firstFrame), _numFrames(numFrames),
		_currFrame(firstFrame), _frameTime(frameTime), _lastFrameTime(0) {
	}

	virtual ~FramedImage() {}

	void setAnimation(int firstFrame, int numFrames) {
		_firstFrame = firstFrame;
		_numFrames = numFrames;
		_currFrame = firstFrame;
	}

	SDL_Rect currentClip() const {
		int col = _currFrame % _cols;
		int row = _currFrame / _cols;
		return SDL_Rect{ col * _frameW, row * _frameH, _frameW, _frameH };
	}

	void update(Uint32 currTime) {
		if (_frameTime == 0) return;
		if (currTime - _lastFrameTime >= _frameTime) {
			_lastFrameTime = currTime;
			_currFrame++;
			if (_currFrame >= _firstFrame + _numFrames)
				_currFrame = _firstFrame;
		}
	}

	const Texture* _tex;
	int    _frameW;
	int    _frameH;
	int    _cols;
	int    _firstFrame;
	int    _numFrames;
	int    _currFrame;
	Uint32 _frameTime;
	Uint32 _lastFrameTime;
};