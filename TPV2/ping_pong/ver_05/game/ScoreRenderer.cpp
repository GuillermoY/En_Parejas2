// This file is part of the course TPV2@UCM - Samir Genaim

#include "ScoreRenderer.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "GameManager.h"

ScoreRenderer::ScoreRenderer() :
		_left_score(0), //
		_right_score(0), //
		_scoreMsg() {
	updateScoreMsg();
}

ScoreRenderer::~ScoreRenderer() {
}

void ScoreRenderer::render(Container *o) {
	auto gm = static_cast<GameManager*>(o);

	int curr_left_score = gm->getScore(0);
	int curr_right_score = gm->getScore(1);
	if (curr_left_score != _left_score || curr_right_score != _right_score) {
		_left_score = curr_left_score;
		_right_score = curr_right_score;
		updateScoreMsg();
	}

	_scoreMsg.render((sdlutils().width() - _scoreMsg.width()) / 2, 10);
}

void ScoreRenderer::updateScoreMsg() {
	_scoreMsg = Texture(
			sdlutils().renderer(), //
			std::to_string(_left_score) + " - " + std::to_string(_right_score),
			sdlutils().fonts().at("ARIAL16"), build_sdlcolor(0xffffffff));
}
