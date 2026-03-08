// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class MaterialConsistency : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::MATERIALCONSISTENCY)

	MaterialConsistency();
	MaterialConsistency(int consistency);
	virtual ~MaterialConsistency();

	void initComponent() override;
	void update() override;

	int getConsistency() const { return _consistency; }

private:
	int      _consistency;
	uint64_t _lastCheckTime;

	static constexpr uint64_t _CHECK_INTERVAL = 5000; // 5 segundos en ms
	static constexpr float    _DECAY_PROB      = 0.1f; // 10% de probabilidad
};
