#include "DeclareBankruptcyCommand.h"
#include <iostream>

DeclareBankruptcyCommand::DeclareBankruptcyCommand(Player& player, const Vector<Property*>& allProperties)
    : bankruptPlayer(player),
    allGameProperties(allProperties),
    originalMoney(player.getMoney()),
    couldHavePaid(false) {
}


void DeclareBankruptcyCommand::execute() {
    previouslyOwnedProperties.clear();
    int totalAssets = bankruptPlayer.getMoney();

    for (size_t i = 0; i < allGameProperties.getSize(); ++i) {
        Property* prop = allGameProperties[i];
        if (prop->getOwner() == &bankruptPlayer) {
            previouslyOwnedProperties.push_back(prop);
            totalAssets += prop->getPriceToBuy() / 2;

            for (size_t j = 0; j < prop->getMortgages().getSize(); ++j) {
                Mortgage* m = prop->getMortgages()[j];
                if (dynamic_cast<Cottage*>(m)) {
                    totalAssets += prop->getPriceForCottage() / 2;
                }
                else if (dynamic_cast<Castle*>(m)) {
                    totalAssets += prop->getPriceForCastle() / 2;
                }
            }
        }
    }

    couldHavePaid = (totalAssets >= 0);

    for (size_t i = 0; i < previouslyOwnedProperties.getSize(); ++i) {
        Property* prop = previouslyOwnedProperties[i];
        for (size_t j = 0; j < prop->getMortgages().getSize(); ++j) {
            delete prop->getMortgages()[j];
        }
        prop->getMortgages().clear();
        prop->setOwner(nullptr);
    }

    Bank::subtractMoney(bankruptPlayer, bankruptPlayer.getMoney());
    std::cout << bankruptPlayer.tokenToString() << " declared bankruptcy!\n";
}

void DeclareBankruptcyCommand::undo() {
    if (!couldHavePaid) return;

    for (size_t i = 0; i < previouslyOwnedProperties.getSize(); ++i) {
        previouslyOwnedProperties[i]->setOwner(&bankruptPlayer);
    }
    Bank::addMoney(bankruptPlayer, originalMoney - bankruptPlayer.getMoney());

    std::cout << "Bankruptcy undone for " << bankruptPlayer.tokenToString() << "\n";
}

GameCommand* DeclareBankruptcyCommand::clone() const {
    return new DeclareBankruptcyCommand(*this);
}