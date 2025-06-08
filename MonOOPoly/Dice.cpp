#include "Dice.h"

int Dice::roll() const {
    return (std::rand() % 6) + 1;
}
