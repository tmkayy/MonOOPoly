#include "PayRentCommand.h"
#include "Bank.h"

PayRentCommand::PayRentCommand(Player& payer, Property& property)
    : payer(payer), receiver(*property.getOwner()), property(property),
    amountPaid(0) {
}

void PayRentCommand::execute() {
    amountPaid = property.getPriceForRent();
    Bank::transferMoney(payer, receiver, amountPaid);
}

void PayRentCommand::undo() {
    Bank::transferMoney(receiver, payer, amountPaid);
}

GameCommand* PayRentCommand::clone() const {
    return new PayRentCommand(*this);
}