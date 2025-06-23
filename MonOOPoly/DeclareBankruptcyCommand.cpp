#include "DeclareBankruptcyCommand.h"
#include <iostream>

DeclareBankruptcyCommand::DeclareBankruptcyCommand(Player& player, const Vector<Property*>& allProperties)
    : bankruptPlayer(player),
    allGameProperties(allProperties) {
}

void DeclareBankruptcyCommand::execute() {
    previouslyOwnedProperties.clear();

    // transfer all properties to the bank and clear mortgages
    for (size_t i = 0; i < allGameProperties.getSize(); ++i) {
        Property* prop = allGameProperties[i];
        if (prop->getOwner() == &bankruptPlayer) {
            previouslyOwnedProperties.push_back(prop);
            for (size_t j = 0; j < prop->getMortgages().getSize(); ++j) {
                delete prop->getMortgages()[j];
            }
            prop->getMortgages().clear();
            prop->setOwner(nullptr);
        }
    }

    Bank::subtractMoney(bankruptPlayer, bankruptPlayer.getMoney());
    std::cout << Red << bankruptPlayer.tokenToString() << " declared bankruptcy!" << Reset << std::endl;
}

void DeclareBankruptcyCommand::undo() {
    // can't undo
}

GameCommand* DeclareBankruptcyCommand::clone() const {
    return new DeclareBankruptcyCommand(*this);
}
