// This file is part of the course TPV2@UCM - Samir Genaim

#include "ConfigFilePingPongFactory.h"

#include <array>
#include "../json/JSON.h"
#include "../json/JSONValue.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "AIPaddle.h"

#include "BounceOnBorder.h"
#include "Container.h"
#include "EmptyRectangleRenderer.h"
#include "GameCtrl.h"
#include "GameManager.h"
#include "ImageRenderer.h"
#include "InfoMsgs.h"
#include "MovePaddleWithKeyBoard.h"
#include "MovePaddleWithMouse.h"
#include "RectangleRenderer.h"
#include "RenderComponent.h"
#include "ScoreRenderer.h"
#include "SimpleMove.h"
#include "StopOnBorder.h"

ConfigFilePingPongFactory::ConfigFilePingPongFactory(std::string filename) {

	_jValueRoot = JSON::ParseFromFile(filename);

	if (_jValueRoot == nullptr || !_jValueRoot->IsObject()) {
		throw "Something went wrong while load/parsing '" + filename + "'";
	}
}

ConfigFilePingPongFactory::~ConfigFilePingPongFactory() {
	delete _jValueRoot;
}

GameObject* ConfigFilePingPongFactory::createRightPaddle(GameObject *ball) {
	Container *paddle = new Container();
	paddle->addComponent(new SimpleMove());
	paddle->addComponent(new StopOnBorder());

	paddle->setWidth(10.0f);
	paddle->setHeight(50.0f);
	paddle->getPos().set(10, sdlutils().height() / 2 - 25);

	configurePaddle(paddle, "rightpaddle", ball);

	return paddle;

}

GameObject* ConfigFilePingPongFactory::createLeftPaddle(GameObject *ball) {
	Container *paddle = new Container();
	paddle->addComponent(new SimpleMove());
	paddle->addComponent(new StopOnBorder());

	paddle->setWidth(10.0f);
	paddle->setHeight(50.0f);
	paddle->getPos().set(sdlutils().width() - 15, sdlutils().height() / 2 - 25);

	configurePaddle(paddle, "leftpaddle", ball);

	return paddle;
}

GameManager* ConfigFilePingPongFactory::createGameManager(GameObject *ball,
		GameObject *leftPaddle, GameObject *rightPaddle) {
	GameManager *gm = new GameManager(ball);
	gm->addComponent(new GameCtrl());
	gm->addComponent(new ScoreRenderer());
	gm->addComponent(new InfoMsgs());
	return gm;

}

GameObject* ConfigFilePingPongFactory::createBall() {

	Container *ball = new Container();
	ball->setWidth(10.0f);
	ball->setHeight(10.0f);
	ball->getPos().set(sdlutils().width() / 2 - 5, sdlutils().height() / 2 - 5);

	// physics components
	ball->addComponent(new SimpleMove());
	ball->addComponent(new BounceOnBorder());

	auto jRoot = _jValueRoot->AsObject();
	assert(jRoot["ball"] != nullptr && jRoot["ball"]->IsObject());
	auto jBall = jRoot["ball"]->AsObject();

	// configure render component
	RenderComponent *rc;
	if (jBall["type"] != nullptr) {
		if (jBall["type"]->AsString() == "image") {
			rc = new ImageRenderer(
					&sdlutils().images().at(jBall["image_file"]->AsString()));
		} else if (jBall["type"]->AsString() == "rect") {
			rc = new RectangleRenderer(
					build_sdlcolor(jBall["color"]->AsString()));
		} else if (jBall["type"]->AsString() == "erect") {
			rc = new EmptyRectangleRenderer(
					build_sdlcolor(jBall["color"]->AsString()));
		} else {
			throw "ball type is invalid";
		}
	} else {
		rc = new RectangleRenderer(build_sdlcolor(0xffffffff));
	}
	ball->addComponent(rc);

	return ball;

}

void ConfigFilePingPongFactory::configurePaddle(Container *paddle,
		std::string id, GameObject *ball) {

	// we know the root is JSONObject
	auto jRoot = _jValueRoot->AsObject();
	assert(jRoot[id] != nullptr && jRoot[id]->IsObject());
	auto jPaddle = jRoot[id]->AsObject();

	// render component
	RenderComponent *rc = nullptr;
	if (jPaddle["type"] != nullptr) {
		if (jPaddle["type"]->AsString() == "rect") {
			rc = new RectangleRenderer(
					build_sdlcolor(jPaddle["color"]->AsString()));
		} else if (jPaddle["type"]->AsString() == "erect") {
			rc = new EmptyRectangleRenderer(
					build_sdlcolor(jPaddle["color"]->AsString()));
		} else {
			throw "type of " + id + "  is invalid";
		}
	} else {
		rc = new RectangleRenderer(build_sdlcolor(0xffffffff));
	}
	paddle->addComponent(rc);

	// input component
	InputComponent *ic = nullptr;
	if (jPaddle["ctrl"] != nullptr) {
		if (jPaddle["ctrl"]->AsString() == "auto") {
			ic = new AIPaddle(ball);
		} else if (jPaddle["ctrl"]->AsString() == "mouse") {
			ic = new MovePaddleWithMouse();
		} else if (jPaddle["ctrl"]->AsString() == "keys") {
			std::array<SDL_Scancode, 3> keys = { SDL_SCANCODE_UP,
					SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT };

			for (auto i = 0u; i < keys.size(); i++) {
				keys[i] =
						static_cast<SDL_Scancode>(jPaddle["keys"]->AsArray()[i]->AsNumber());
			}

			auto kbic = new MovePaddleWithKeyBoard();
			kbic->setKeys(keys[0], keys[1], keys[2]);
			ic = kbic;
		} else {
			throw "ctrl of " + id + " is invalid";
		}
	} else {
		ic = new MovePaddleWithMouse();
	}
	paddle->addComponent(ic);

}
