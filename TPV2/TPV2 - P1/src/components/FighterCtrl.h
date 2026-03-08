// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class Transform;

class FighterCtrl : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::FIGHTERCTRL)

		FighterCtrl();
	virtual ~FighterCtrl();
	void initComponent() override;
	void update() override;

private:
	Transform* _tr;

	static constexpr float _ROT_STEP = 5.0f;  // grados por frame al girar
	static constexpr float _THRUST = 0.2f;  // factor de empuje
	static constexpr float _SPEED_LIMIT = 3.0f;  // velocidad máxima
};
