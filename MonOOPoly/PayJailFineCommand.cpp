#include "PayJailFineCommand.h"
#include "Jail.h"
#include <iostream>

PayJailFineCommand::PayJailFineCommand(Player& player)
    : player(player), paid(false), wasInJail(player.isImprisoned()) {
}

void PayJailFineCommand::execute() {
    wasInJail = player.isImprisoned();
    if (wasInJail) {
        paid = Bank::subtractMoney(player, bailCost);  //
        if (paid) {
            player.setImprisoned(false);
            std::cout << player.tokenToString()
                << " paid $100 jail fine and was released\n";
        }
        else {
            std::cout << player.tokenToString()
                << " couldn't pay $100 jail fine\n";
        }
    }
}

void PayJailFineCommand::undo() {
    if (paid && wasInJail) {
        //only undo if payment actually occurred
        Bank::addMoney(player, 100);  //refund the fine
        player.setImprisoned(true);   //put back in jail
        std::cout << player.tokenToString()
            << " $100 jail fine was refunded and player returned to jail\n";
    }
}

GameCommand* PayJailFineCommand::clone() const {
    return new PayJailFineCommand(*this);
}