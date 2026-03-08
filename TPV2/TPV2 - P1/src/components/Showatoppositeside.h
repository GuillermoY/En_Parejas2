// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class Transform;

class ShowAtOppositeSide : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::SHOWATOPPOSITESIDE)

		ShowAtOppositeSide();
	virtual ~ShowAtOppositeSide();
	void initComponent() override;
	void update() override;

private:
	Transform* _tr;
};
