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

bool Bank::transferMoney(Player& from, Player& to, double amount) {
    if (&from == &to) return false;
    if (amount <= 0) return false;

    if (from.getMoney() < amount) {
        //handle bankrupcy
        return false;
    }

    try {
        from.money -= amount;
        to.money += amount;


        return true;
    }
    catch (...) {
        return false;
    }
}
