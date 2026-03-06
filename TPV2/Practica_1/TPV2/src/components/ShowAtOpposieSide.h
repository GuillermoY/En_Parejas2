// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "Transform.h"
#include <SDL.h>

class ShowAtOpposieSide : public Transform {
public:
	ShowAtOpposieSide(){}
	virtual ~ShowAtOpposieSide(){}

	void update() override;

private:

};

#pragma once
