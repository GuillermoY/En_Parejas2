// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include "game/Game.h"

int main(int, char**) {
	try {
		Game g;
		g.init();
		g.start();
	}
	catch (const std::string& e) {
		std::cerr << e << std::endl;
	}
	catch (const char* e) {
		std::cerr << e << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception" << std::endl;
	}
	return 0;
}