#include "PassGoCommand.h"
#include "Bank.h"

PassGoCommand::PassGoCommand(Player& player, bool landed)
    : player(player), landed(landed), moneyCollected(false) {
}

void PassGoCommand::execute() {
    Bank::addMoney(player, goMoney);
    moneyCollected = true;
    std::cout << Green << "Passed GO! Collected " << Green << "$" << goMoney << Reset << std::endl;
}

void PassGoCommand::undo() {
    if (moneyCollected) {
        Bank::subtractMoney(player, goMoney);
        moneyCollected = false;
    }
}

GameCommand* PassGoCommand::clone() const {
    return new PassGoCommand(*this);
}
