// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"

class Transform;

class TowardDestination : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::TOWARDDESTINATION)

	TowardDestination();
	virtual ~TowardDestination();
	void initComponent() override;
	void update() override;

private:
	void pickNewDestination();

	Transform *_tr;
	Vector2D   _dest;

	static constexpr float _ARRIVAL_DIST = 10.0f;
};
