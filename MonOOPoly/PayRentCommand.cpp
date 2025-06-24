#include "PayRentCommand.h"
#include "Bank.h"
#include "Monopoly.h"
#include "DeclareBankruptcyCommand.h"
#include <iostream>

PayRentCommand::PayRentCommand(Player& payer, Property& property)
    : payer(payer), receiver(*property.getOwner()), property(property), amountPaid(0), wasPaid(false) {
}

void PayRentCommand::execute() {
    amountPaid = property.getPriceForRent();
    if (amountPaid <= 0) return;

    Monopoly* game = payer.getGame();
    wasPaid = Bank::transferMoney(payer, receiver, amountPaid);

    if (wasPaid) {
        std::cout << Green << payer.tokenToString()
            << " paid rent of $" << amountPaid << " to "
            << receiver.tokenToString() << "." << Reset << std::endl;
        return;
    }

    //immediate bankruptcy if can't pay
    std::cout << Red << payer.tokenToString()
        << " can't pay rent of $" << amountPaid << " and is forced to declare bankruptcy!" << Reset << std::endl;
    game->executeCommand(new DeclareBankruptcyCommand(payer, game->getGameBoard().getProperties()));
}

void PayRentCommand::undo() {
    if (wasPaid) {
        Bank::transferMoney(receiver, payer, amountPaid);
        wasPaid = false;
    }
}

GameCommand* PayRentCommand::clone() const {
    return new PayRentCommand(*this);
}