// This file is part of the course TPV2@UCM - Samir Genaim


#include "operations.h"

#include <cstdlib>
#include <iostream>

#include "messages.h"
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
	BinOpMSg msg;
	msg.id = _BINOP;

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

	msg.op = opcode;

	std::cout << "Enter two integer numbers: ";
	std::cin >> msg.x >> msg.y;

	Uint8* end = msg.serialize(buff);

	return end-buff;
}

Sint32 handle_binary_operation(Uint8 *buff) {
	BinOpMSg msg;
	msg.deserialize(buff);

	Sint32 res = 0;
	switch (msg.op) {
	case 0:
		res = msg.x + msg.y;
		break;
	case 1:
		res = msg.x - msg.y;
		break;
	case 2:
		res = msg.x * msg.y;
		break;
	case 3:
		res = msg.x / msg.y;
		break;
	default:
		break;
	}

	return res;
}

int random_operation(Uint8 *buff) {
	Msg msg;
	msg.id = _RANDOM;
	Uint8* end = msg.serialize(buff);
	return end-buff;
}

Sint32 handle_random_operation(Uint8 *buff) {
	return rand();
}

int sum_operation(Uint8 *buff) {
	SumMsg msg;
	msg.id = _SUM;
	int n = 0;
	do {
		std::cout << "Enter the number (1-10) of elements you want to sum: ";
		std::cin >> n;
	} while (n < 1 || n > 10);
	msg.size = n;

	for (auto i = 0; i < n; i++) {
		std::cout << "Enter n[" << i << "]: ";
		std::cin >> msg.nums[i];
	}

	Uint8* end = msg.serialize(buff);
	return end-buff;
}

Sint32 handle_sum_operation(Uint8 *buff) {
	SumMsg msg;
	msg.deserialize(buff);
	Sint32 res = 0;
	for (auto i = 0u; i < msg.size; i++)
		res += msg.nums[i];
	return res;
}

