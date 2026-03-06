// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../sdlutils/InputHandler.h"
#include "../utils/Vector2D.h"	
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"
#include <SDL.h>
class Transform;
class FighterCtrl : public InputHandler {
public:
	FighterCtrl();
	virtual ~FighterCtrl();


	void handleInput();

private:
	Vector2D vel = Vector2D(0.0,0.0); // Vel actual
	Vector2D newVel; // Nueva vel
	float r = 90; // Rotación, empieza en 90 
	float thrust = 1.0f; // Factor empuje
	float speedLimit = 3.0f; // Vel maximo
	Transform* o = new Transform(Vector2D(0.0, 0.0), vel,r,r,r);
};