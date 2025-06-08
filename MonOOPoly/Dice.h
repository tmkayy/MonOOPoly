#pragma once
#include <cstdlib>
#include <ctime>

class Dice {
public:
    Dice() { std::srand(std::time(nullptr)); }

    int roll() const;
};
