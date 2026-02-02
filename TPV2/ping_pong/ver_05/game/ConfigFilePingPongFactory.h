// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <string>

#include "PingPongFactory.h"

class JSONValue;
class Container;
class GameObject;

class ConfigFilePingPongFactory: public PingPongFactory {
public:
	ConfigFilePingPongFactory(std::string filename);
	virtual ~ConfigFilePingPongFactory();
	GameObject* createRightPaddle(GameObject *ball) override;
	GameObject* createLeftPaddle(GameObject *ball) override;
	GameManager* createGameManager(GameObject *ball, GameObject *leftPaddle,
			GameObject *rightPaddle) override;
	GameObject* createBall() override;
private:

	void configurePaddle(Container *paddle, std::string id, GameObject *ball);
	JSONValue *_jValueRoot;

};

