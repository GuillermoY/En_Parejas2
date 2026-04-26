// This file is part of the course TPV2@UCM - Samir Genaim


#include "operations.h"

#include <cstdlib>
#include <iostream>

Uint8 menu() {
	int option = 255;

	do {
		std::cout << "0. Binary operation." << std::endl;
		std::cout << "1. Random number." << std::endl;
		std::cout << "2. Sum." << std::endl;
		std::cout << "3. Exit." << std::endl;
		std::cout << "--------------------" << std::endl;
		std::cout << "Choose an option: ";
		std::cin >> option;
	} while (option < 0 || option > 3);

	return option;
}

int binary_operation(Uint8 *buff) {
	buff[0] = 0; // binary operation

	char op = ' ';
	Uint8 opcode = 255;
	do {
		std::cout << "Enter an operation (+ - * /): ";
		std::cin >> op;
		switch (op) {
		case '+':
			opcode = 0;
			break;
		case '-':
			opcode = 1;
			break;
		case '*':
			opcode = 2;
			break;
		case '/':
			opcode = 3;
			break;
		default:
			break;
		}
	} while (opcode > 3);

	Sint32 x, y;

	std::cout << "Enter two integer numbers: ";
	std::cin >> x >> y;

	buff[1] = opcode;

	// THIS IS NOT SAFE
	// *reinterpret_cast<Sint32*>(buff + 2) = x;
	// *reinterpret_cast<Sint32*>(buff + 2 + sizeof(Sint32)) = y;

	memcpy(buff+2,&x,sizeof(Sint32));
	memcpy(buff+2+sizeof(Sint32),&y,sizeof(Sint32));

	return 2 + 2 * sizeof(Sint32);
}

Sint32 handle_binary_operation(Uint8 *buff) {

	// THIS IS NOT SAFE
	// Sint32 x = *reinterpret_cast<Sint32*>(buff + 1);
	// Sint32 y = *reinterpret_cast<Sint32*>(buff + 1 + sizeof(Sint32));

	Sint32 x, y;
	memcpy(&x,buff+1,sizeof(Sint32));
	memcpy(&y,buff+1+sizeof(Sint32),sizeof(Sint32));


	Sint32 res = 0;
	switch (buff[0]) {
	case 0:
		res = x + y;
		break;
	case 1:
		res = x - y;
		break;
	case 2:
		res = x * y;
		break;
	case 3:
		res = x / y;
		break;
	default:
		break;
	}

	return res;
}

int random_operation(Uint8 *buff) {
	buff[0] = 1;
	return 1;
}

Sint32 handle_random_operation(Uint8 *buff) {
	return rand();
}

int sum_operation(Uint8 *buff) {
	buff[0] = 2;

	int n = 0;
	do {
		std::cout << "Enter the number (1-255) of elements you want to sum: ";
		std::cin >> n;
	} while (n < 1 || n > 10);
	buff[1] = n;

	for (auto i = 0; i < n; i++) {
		Sint32 x;
		std::cout << "Enter n[" << i << "]: ";
		std::cin >> x;
		// THIS IS NOT SAFE
		// *reinterpret_cast<Sint32*>(buff + 2 + i * sizeof(Sint32)) = x;
		memcpy(buff+2+i*sizeof(Sint32),&x,sizeof(Sint32));
	}

	return 2 + n * sizeof(Sint32);
}

Sint32 handle_sum_operation(Uint8 *buff) {
	Uint8 n = buff[0];
	Sint32 res = 0;
	for (auto i = 0u; i < n; i++) {
		Sint32 x;
		// THIS IS NOT SAFE
		//x = *reinterpret_cast<Sint32*>(buff + 1 + i * sizeof(Sint32));
		memcpy(&x,buff+1+i*sizeof(Sint32),sizeof(Sint32));
		res += x;
	}
	return res;
}

