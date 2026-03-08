// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class Transform;

class TeleportOnExit : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::TELEPORTONEXIT)

	TeleportOnExit();
	virtual ~TeleportOnExit();
	void initComponent() override;
	void update() override;

private:
	Transform *_tr;
};
