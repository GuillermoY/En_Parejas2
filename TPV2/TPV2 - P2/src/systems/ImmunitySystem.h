// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

class ImmunitySystem : public ecs::System {
public:
	ImmunitySystem();
	virtual ~ImmunitySystem();

	void initSystem() override;
	void update()     override;
	void recieve(const Message& m) override;
};