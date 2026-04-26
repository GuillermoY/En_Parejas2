// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include "SDLNetUtils.h"


struct Message {
	Uint8 id;

	_IMPL_SERIALIZATION_(id)
};

struct InfoMsg : Message {
	Uint8 x[10];
	Uint32 y;

	_IMPL_SERIALIZATION_(*static_cast<Message*>(this),x,y)
};

int main(int argc, char **argv) {
	Uint8 buffer[100];

	// serialize
	InfoMsg a;
	a.id = 25;
	for (auto i = 0u; i < 10; i++)
		a.x[i] = 2*i;
	a.y = 1234321;
	a.serialize(buffer);

	// deserialize
	InfoMsg b;
	b.deserialize(buffer);
	std::cout << "b.id = " << (int)b.id << std::endl;
	for (auto i = 0u; i < 10; i++)
		std::cout << "b.x[" << i << "] = " << (int)b.x[i] << std::endl;
	std::cout << "b.y = " << b.y << std::endl;

	return 0;
}
