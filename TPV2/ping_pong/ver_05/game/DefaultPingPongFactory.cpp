// This file is part of the course TPV2@UCM - Samir Genaim

#include "DefaultPingPongFactory.h"

#include "../sdlutils/SDLUtils.h"
#include "AIPaddle.h"
#include "BounceOnBorder.h"
#include "Container.h"
#include "GameCtrl.h"
#include "GameManager.h"
#include "ImageRenderer.h"
#include "InfoMsgs.h"
#include "MovePaddleWithKeyBoard.h"
#include "RectangleRenderer.h"
#include "ScoreRenderer.h"
#include "SimpleMove.h"
#include "StopOnBorder.h"

DefaultPingPongFactory::DefaultPingPongFactory() {
}

DefaultPingPongFactory::~DefaultPingPongFactory() {
}

GameObject* DefaultPingPongFactory::createRightPaddle(GameObject *ball) {
	Container *paddle = new Container();

	paddle->addComponent(new SimpleMove());
	paddle->addComponent(new StopOnBorder());

	paddle->setWidth(10.0f);
	paddle->setHeight(50.0f);
	paddle->getPos().set(10, sdlutils().height() / 2 - 25);

	auto leftPaddle_ic = new MovePaddleWithKeyBoard();
	leftPaddle_ic->setKeys(SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A);
	paddle->addComponent(leftPaddle_ic);
//	leftPaddle_->addComponent(new AIPaddle(ball));

	paddle->addComponent(new RectangleRenderer(build_sdlcolor(0xff0000ff)));

	return paddle;
}

GameObject* DefaultPingPongFactory::createLeftPaddle(GameObject *ball) {
	Container *paddle = new Container();
	paddle->addComponent(new SimpleMove());
	paddle->addComponent(new StopOnBorder());

	paddle->setWidth(10.0f);
	paddle->setHeight(50.0f);
	paddle->getPos().set(sdlutils().width() - 15, sdlutils().height() / 2 - 25);

	auto leftPaddle_ic = new MovePaddleWithKeyBoard();
	leftPaddle_ic->setKeys(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN,
			SDL_SCANCODE_SPACE);
	paddle->addComponent(leftPaddle_ic);
//	paddle->addComponent(new AIPaddle(ball));

	paddle->addComponent(new RectangleRenderer(build_sdlcolor(0x0000ffff)));

	return paddle;

}

GameManager* DefaultPingPongFactory::createGameManager(GameObject *ball,
		GameObject *leftPaddle, GameObject *rightPaddle) {
	GameManager *gm = new GameManager(ball);
	gm->addComponent(new GameCtrl());
	gm->addComponent(new ScoreRenderer());
	gm->addComponent(new InfoMsgs());
	return gm;

}

GameObject* DefaultPingPongFactory::createBall() {
	Container *ball = new Container();
	ball->addComponent(new SimpleMove());
	ball->addComponent(new BounceOnBorder());
	//	ball_->addComponent(new RectangleRenderer(build_sdlcolor(0xff0000ff)));
	ball->addComponent(
			new ImageRenderer(&sdlutils().images().at("tennis_ball")));

	ball->setWidth(10.0f);
	ball->setHeight(10.0f);
	ball->getPos().set(sdlutils().width() / 2 - 5, sdlutils().height() / 2 - 5);

	return ball;
}
