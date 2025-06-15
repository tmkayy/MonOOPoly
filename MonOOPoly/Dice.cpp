#include "Dice.h"
#include <iostream>

int Dice::roll() const {
    int result = (std::rand() % 6) + 1;
    std::cout << "Rolled: " << result << std::endl;
    return result;
}