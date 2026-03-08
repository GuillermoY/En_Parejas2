// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class Generations : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::GENERATIONS)

	Generations();
	Generations(int gen);
	virtual ~Generations();

	void initComponent() override;

	int getGenerations() const { return _generations; }
	void setGenerations(int g) { _generations = g; }

private:
	int _generations;
};
