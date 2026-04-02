// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <array>
#include <cstdint>

#include "../ecs/System.h"

struct GameState;

class GameCtrlSystem: public ecs::System {
public:

	enum Side : uint8_t {
		LEFT = 0, //
		RIGHT
	};

	enum State {
		NEWGAME = 0, // just before starting a new game
		PAUSED, // between rounds
		RUNNING, // playing
		GAMEOVER // game over
	};

	GameCtrlSystem();
	virtual ~GameCtrlSystem();

	inline State getState() {
		return _state;
	}

	inline unsigned int getScore(std::size_t player) {
		return _score[player];
	}

	void update() override;
	void recieve(const Message&) override;



private:

	void onBallExit(Side side);
	void startRound();
	void startGame();
	void killPacMan();
	void gameOver();

	std::array<unsigned int, 2> _score;
	State _state;
	const unsigned int _maxScore;

};

