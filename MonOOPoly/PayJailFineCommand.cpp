#include "PayJailFineCommand.h"
#include "Jail.h"
#include <iostream>
#include "Monopoly.h"
#include "DeclareBankruptcyCommand.h"

void PayJailFineCommand::execute() {
    wasInJail = player.isImprisoned();
    if (!wasInJail)
        return;

    Monopoly* game = player.getGame();
    if (!game) {
        std::cout << "Game context not found for player.\n";
        return;
    }

    paid = Bank::subtractMoney(player, bailCost);
    if (paid) {
        player.setImprisoned(false);
        std::cout << player.tokenToString()
            << " paid $100 jail fine and was released\n";
        return;
    }

    std::cout << player.tokenToString()
        << " couldn't pay $100 jail fine.\n";

    while (!paid && !player.isBankrupt()) {
        std::cout << "You must raise funds to pay the jail fine ($100).\n";
        std::cout << "Use the menu to sell property, trade, or declare bankruptcy.\n";
        game->showPlayerOptions();

        //try again
        paid = Bank::subtractMoney(player, bailCost);
        if (paid) {
            player.setImprisoned(false);
            std::cout << player.tokenToString()
                << " paid $100 jail fine and was released\n";
            return;
        }
    }
    if (!paid && !player.isBankrupt()) {
        std::cout << player.tokenToString() << " still does not have enough money to pay bail ($" << bailCost << ").\n";
        game->executeCommand(new DeclareBankruptcyCommand(player, game->getGameBoard().getProperties()));
    }
}

void PayJailFineCommand::undo() {
    if (paid && wasInJail) {
        //only undo if payment actually occurred
        Bank::addMoney(player, bailCost);  //refund the fine
        player.setImprisoned(true);   //put back in jail
        std::cout << player.tokenToString()
            << " $100 jail fine was refunded and player returned to jail\n";
    }
}

GameCommand* PayJailFineCommand::clone() const {
    return new PayJailFineCommand(*this);
}