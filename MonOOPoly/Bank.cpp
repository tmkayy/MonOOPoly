#include "Bank.h"

void Bank::addMoney(Player& player, double cash)
{
	if (cash < 0) {
		throw std::invalid_argument("negative money");
	}
	player.money += cash;
}

void Bank::subtractMoney(Player& player, double cash)
{
	if (cash > 0) {
		throw std::invalid_argument("positive money");
	}
	player.money -= cash;
	if (player.money < 0) {
		//implement
	}
}
