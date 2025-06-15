#include "Jail.h"
#include "Bank.h"
#include "Player.h"
#include <iostream>

Field* Jail::clone() const {
    return new Jail(*this);
}

void Jail::onLand(Player& player) {
    std::cout << player.tokenToString()
        << (player.isImprisoned() ? " is visiting jail (imprisoned)"
            : " is just visiting jail")
        << std::endl;
}

void Jail::sendToJail(Player& player) {
    player.setImprisoned(true);
    player.setTurnsInJail(0);
    std::cout << player.tokenToString()<< " was sent to jail!" << std::endl;
}

bool Jail::attemptRelease(Player& player, bool payBail, bool rolledDoubles) {
    if (!player.isImprisoned()) return true;

    if (payBail) {
        if (Bank::subtractMoney(player, 100)) {
            player.setImprisoned(false);
            std::cout << player.tokenToString()
                << " paid $100 bail and was released" << std::endl;
            return true;
        }
        std::cout << player.tokenToString()
            << " couldn't pay $100 bail" << std::endl;
        return false;
    }

    if (rolledDoubles) {
        player.setImprisoned(false);
        std::cout << player.tokenToString()
            << " rolled doubles and was released" << std::endl;
        return true;
    }

    player.setTurnsInJail(player.getTurnsInJail() + 1);
    std::cout << player.tokenToString()
        << " failed to roll doubles (turns in jail: "
        << player.getTurnsInJail() << ")" << std::endl;
    return false;
}