// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

struct Transform;

class BallSystem: public ecs::System {
public:
	BallSystem();
	virtual ~BallSystem();


	void initSystem() override;
	void update() override;
	void recieve(const Message&) override;
private:
	void resetBall();
	void moveBall();
	void changeDirection();

	Transform *_ballTr;
};

