#include "PayTaxCommand.h"
#include "Bank.h"
#include "Monopoly.h"
#include "DeclareBankruptcyCommand.h"

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

    std::cout << player.tokenToString()
        << " couldn't pay tax of $" << amount << ".\n";

    while (!wasPaid && !player.isBankrupt()) {
        std::cout << "You must raise funds to pay the tax ($" << amount << ").\n";
        std::cout << "Use the menu to sell property, trade, or declare bankruptcy.\n";
        game->showPlayerOptions();

        // try again
        wasPaid = Bank::subtractMoney(player, amount);
        if (wasPaid) {
            return;
        }
    }

    if (!wasPaid && !player.isBankrupt()) {
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