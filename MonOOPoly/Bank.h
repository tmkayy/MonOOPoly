#pragma once
#include "Player.h"
class Bank
{
public:
	void addMoney(Player& player, double cash);
	void subtractMoney(Player& player, double cash);
};

