#include "Jail.h"
#include "Bank.h"
#include "Player.h"
#include <iostream>

Field* Jail::clone() const {
	return new Jail(*this);
}

void Jail::sendToJail(Player& player) {
	player.setImprisoned(true);
	player.setTurnsInJail(0);
	std::cout << player.tokenToString() << " was sent to jail!" << std::endl;
}