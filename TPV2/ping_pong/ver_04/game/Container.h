// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>
#include "GameObject.h"

class InputComponent;
class RenderComponent;
class PhysicsComponent;

class Container: public GameObject {
public:
	Container();
	virtual ~Container();

	void handleInput() override;
	void update() override;
	void render() override;

	inline void addComponent(InputComponent *ic) {
		_ic.push_back(ic);
	}

	inline void addComponent(RenderComponent *rc) {
		_rc.push_back(rc);
	}

	inline void addComponent(PhysicsComponent *pc) {
		_pc.push_back(pc);
	}

private:
	std::vector<InputComponent*> _ic;
	std::vector<RenderComponent*> _rc;
	std::vector<PhysicsComponent*> _pc;
};

