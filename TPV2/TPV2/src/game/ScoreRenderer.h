// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../sdlutils/Texture.h"
#include "RenderComponent.h"

class ScoreRenderer: public RenderComponent {
public:
	ScoreRenderer();
	virtual ~ScoreRenderer();
	void render(Container *o) override;
private:
	void updateScoreMsg();

	int _left_score;
	int _right_score;
	Texture _scoreMsg;
};

