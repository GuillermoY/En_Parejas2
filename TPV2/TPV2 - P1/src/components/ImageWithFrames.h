// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include <SDL.h>
#include <cstdint>

class Transform;
class Texture;

class ImageWithFrames : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::IMAGEWITHFRAMES)

		ImageWithFrames();
	ImageWithFrames(const Texture* tex, int numFrames, int frameW, int frameH, int framesPerRow);
	virtual ~ImageWithFrames();

	void initComponent() override;
	void update() override;
	void render() override;

private:
	Transform* _tr;
	const Texture* _tex;
	int             _numFrames;
	int             _frameW;
	int             _frameH;
	int             _framesPerRow;
	int             _currFrame;
	uint64_t        _lastFrameTime;

	static constexpr uint64_t _FRAME_INTERVAL = 50;
};