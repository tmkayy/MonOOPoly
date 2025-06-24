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

    Monopoly* game = player.getGame();
    wasPaid = Bank::subtractMoney(player, amount);

    if (wasPaid) {
        return;
    }

    //immediate bankruptcy if can't pay
    std::cout << Red << player.tokenToString()
        << " can't pay tax of $" << amount << " and is forced to declare bankruptcy!" << Reset << std::endl;
    game->executeCommand(new DeclareBankruptcyCommand(player, game->getGameBoard().getProperties()));
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