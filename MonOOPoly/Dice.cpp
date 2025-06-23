#include "Dice.h"
#include <iostream>
#include "Consts.h"

int Dice::roll() const {
	int result = (std::rand() % 6) + 1;
	std::cout << "Rolled: " << Cyan << result << Reset << std::endl;
	return result;
}