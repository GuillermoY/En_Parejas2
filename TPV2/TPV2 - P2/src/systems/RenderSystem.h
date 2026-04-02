// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

struct Transform;
struct FramedImage;

class RenderSystem : public ecs::System {
public:
	RenderSystem();
	virtual ~RenderSystem();

	void initSystem() override;
	void update()     override;

private:
	void drawFood();
	void drawGhosts();
	void drawPacMan();
	void drawLives();
	void drawFramed(Transform* tr, FramedImage* fi);
};