// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "FighterFacade.h"

namespace ecs {
	class EntityManager;
}

class FighterUtils : public FighterFacade {
public:
	FighterUtils();
	virtual ~FighterUtils();

	void create_fighter() override;
	void reset_fighter() override;
	void reset_lives() override;
	int  update_lives(int n) override;
};
