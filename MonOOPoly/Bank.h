#pragma once
#include "Player.h"
class Bank
{
public:
	static bool addMoney(Player& player, double cash);
	static bool subtractMoney(Player& player, double cash);
};

