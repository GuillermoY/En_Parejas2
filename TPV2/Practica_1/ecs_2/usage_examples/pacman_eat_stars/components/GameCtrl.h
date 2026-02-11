// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"

class GameCtrl: public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::GAMECTRL)

	GameCtrl();
	virtual ~GameCtrl();

	inline void onStarDeath() {
		_currNumOfStars--;
	}

	inline void onStarEaten() {
		_currNumOfStars--;
		_score++;
	}

	void initComponent() override;
	void update() override;
	void render() override;

private:
	void createStart(unsigned int n);

	unsigned int _currNumOfStars;
	unsigned int _score;
	unsigned int _starsLimit;
};

