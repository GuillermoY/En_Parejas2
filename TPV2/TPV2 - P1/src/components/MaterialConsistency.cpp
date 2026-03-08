// This file is part of the course TPV2@UCM - Samir Genaim

#include "MaterialConsistency.h"

#include <cassert>
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"

MaterialConsistency::MaterialConsistency() :
		_consistency(sdlutils().rand().nextInt(10, 101)),
		_lastCheckTime(0) {
}

MaterialConsistency::MaterialConsistency(int consistency) :
		_consistency(consistency),
		_lastCheckTime(0) {
}

MaterialConsistency::~MaterialConsistency() {
}

void MaterialConsistency::initComponent() {
	_lastCheckTime = sdlutils().virtualTimer().currTime();
}

void MaterialConsistency::update() {
	uint64_t now = sdlutils().virtualTimer().currTime();

	if (now - _lastCheckTime >= _CHECK_INTERVAL) {
		_lastCheckTime = now;

		// 10% de probabilidad de perder consistencia
		int roll = sdlutils().rand().nextInt(0, 100);
		if (roll < static_cast<int>(_DECAY_PROB * 100)) {
			_consistency--;
			if (_consistency <= 0) {
				_ent->setAlive(false);
			}
		}
	}
}
