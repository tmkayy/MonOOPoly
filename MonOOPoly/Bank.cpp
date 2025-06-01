#include "Bank.h"

bool Bank::addMoney(Player& player, double cash)
{
	if (cash < 0) {
		return false;
	}
	player.money += cash;
	return true;
}

bool Bank::subtractMoney(Player& player, double cash)
{
	if (cash < 0 || player.money < cash)
		return false;
	player.money -= cash;
	return true;
}
