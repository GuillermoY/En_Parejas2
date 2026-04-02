// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

class FoodSystem : public ecs::System {
public:
	FoodSystem();
	virtual ~FoodSystem();

	void initSystem() override;
	void update()     override;
	void recieve(const Message& m) override;

private:
	int _totalFood;
	int _eatenFood;

	void createFoodGrid();
	void removeAllFood();
	void onFoodEaten(ecs::entity_t e, bool miraculous);
};