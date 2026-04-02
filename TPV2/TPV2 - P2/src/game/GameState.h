// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

class GameState {
public:
	virtual ~GameState() {}
	virtual void enter() {}
	virtual void leave() {}
	virtual void update() = 0;
};