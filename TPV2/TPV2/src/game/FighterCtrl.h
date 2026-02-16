// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "InputComponent.h"
#include "../utils/Vector2D.h"
#include <SDL.h>

class FighterCtrl : public InputComponent {
public:
	FighterCtrl();
	virtual ~FighterCtrl();


	void handleInput(Container* o) override;

private:
	Vector2D vel; // Vel actual
	Vector2D newVel; // Nueva vel
	float r=90; // Rotación, empieza en 90 
	float thrust = 1.0f; // Factor empuje
	float speedLimit = 3.0f; // Vel maximo
};



