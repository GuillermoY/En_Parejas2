// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class Transform;

class DeAcceleration : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::DEACCELERATION)

		DeAcceleration();
	virtual ~DeAcceleration();
	void initComponent() override;
	void update() override;

private:
	Transform* _tr;

	// Factor de desaceleración por frame
	static constexpr float _DEACCEL_FACTOR = 0.995f;
	// Si la magnitud baja de este límite, frenamos del todo
	static constexpr float _MIN_SPEED = 0.05f;
};
