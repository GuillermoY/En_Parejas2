// This file is part of the course TPV2@UCM - Samir Genaim

#include "Container.h"

#include "../sdlutils/macros.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"

Container::Container() {
}

Container::~Container() {
	for (auto pc : _pc)
		delete pc;

	for (auto rc : _rc)
		delete rc;

	for (auto ic : _ic)
		delete ic;
}

void Container::update() {
	for (auto pc : _pc)
		pc->update(this);

}

void Container::render() {
	for (auto rc : _rc)
		rc->render(this);
}

void Container::handleInput() {
	for (auto ic : _ic)
		ic->handleInput(this);
}
