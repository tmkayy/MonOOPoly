#include "PayTaxCommand.h"
#include "Bank.h"

PayTaxCommand::PayTaxCommand(Player& player, double amount)
    : player(player), amount(amount), wasPaid(false) {
}

void PayTaxCommand::execute() {
    if (amount > 0 && Bank::subtractMoney(player, amount)) {
        wasPaid = true;
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