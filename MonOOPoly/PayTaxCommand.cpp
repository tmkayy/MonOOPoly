#include "PayTaxCommand.h"
#include "Bank.h"
#include "Monopoly.h"
#include "DeclareBankruptcyCommand.h"
#include <iostream>

PayTaxCommand::PayTaxCommand(Player& player, double amount)
    : player(player), amount(amount), wasPaid(false) {
}

void PayTaxCommand::execute() {
    if (amount <= 0) return;

    wasPaid = Bank::subtractMoney(player, amount);
    if (wasPaid) {
        return;
    }

    Monopoly* game = player.getGame();

    std::cout << Red << player.tokenToString()
        << " couldn't pay tax of $" << amount << "." << Reset << std::endl;

    while (!wasPaid && !player.isBankrupt()) {
        std::cout << Yellow << "You must raise funds to pay the tax ($" << amount << ")." << Reset << std::endl;
        std::cout << Yellow << "Use the menu to sell property, trade, or declare bankruptcy." << Reset << std::endl;
        game->showPlayerOptions();

        // try again
        wasPaid = Bank::subtractMoney(player, amount);
        if (wasPaid) {
            return;
        }
    }

    if (!wasPaid && !player.isBankrupt()) {
        std::cout << Red << player.tokenToString()
            << " still does not have enough money to pay tax ($" << amount << ")." << Reset << std::endl;
        game->executeCommand(new DeclareBankruptcyCommand(player, game->getGameBoard().getProperties()));
    }
}

void PayTaxCommand::undo() {
    if (wasPaid) {
        Bank::addMoney(player, amount);
        wasPaid = false;
    }
}

GameCommand* PayTaxCommand::clone() const {
    return new PayTaxCommand(*this);
}
