#include "Trade.h"
#include "Player.h"
#include "Property.h"
#include "Bank.h"

Trade::Trade(Player* proposer, Player* receiver)
    : proposer(proposer), receiver(receiver),
    proposerMoney(0), receiverMoney(0),
    isAccepted(false) {
}

void Trade::addProposerProperty(Property* property) {
    proposerProperties.push_back(property);
}

void Trade::addReceiverProperty(Property* property) {
    receiverProperties.push_back(property);
}

void Trade::setProposerMoney(int amount) {
    proposerMoney = amount;
}

void Trade::setReceiverMoney(int amount) {
    receiverMoney = amount;
}

void Trade::accept() {
    isAccepted = true;
    execute();
}

void Trade::reject() {
    isAccepted = false;
}

void Trade::execute() {
    if (!isAccepted || !proposer || !receiver) return;

    if (!isValid()) {
        reject();
        return;
    }

    //transfer properties and update color counts
    for (size_t i = 0; i < proposerProperties.getSize(); ++i) {
        Property* prop = proposerProperties[i];
        if (prop) {
            //update color counts before changing owner
            proposer->decrementPropertyCount(prop->getColor());
            prop->setOwner(receiver);
            receiver->incrementPropertyCount(prop->getColor());

            checkMonopolyChange(prop->getColor(), proposer, receiver);
        }
    }

    for (size_t i = 0; i < receiverProperties.getSize(); ++i) {
        Property* prop = receiverProperties[i];
        if (prop) {
            //update color counts before changing owner
            receiver->decrementPropertyCount(prop->getColor());
            prop->setOwner(proposer);
            proposer->incrementPropertyCount(prop->getColor());

            checkMonopolyChange(prop->getColor(), receiver, proposer);
        }
    }

    //handle money transfers
    if (proposerMoney > 0) {
        Bank::transferMoney(*proposer, *receiver, proposerMoney);
    }
    if (receiverMoney > 0) {
        Bank::transferMoney(*receiver, *proposer, receiverMoney);
    }
}

void Trade::checkMonopolyChange(PropertyColor color, Player* oldOwner, Player* newOwner) {
    bool wasMonopoly = oldOwner->hasMonopoly(color);
    bool isNowMonopoly = newOwner->hasMonopoly(color);

    if (wasMonopoly && !oldOwner->hasMonopoly(color)) {
        std::cout << oldOwner->tokenToString() << " lost monopoly on "
            << Property::colorToString(color) << " properties!\n";
    }

    if (!wasMonopoly && isNowMonopoly) {
        std::cout << newOwner->tokenToString() << " gained monopoly on "
            << Property::colorToString(color) << " properties!\n";
    }
}

bool Trade::isValid() const {
    if (!proposer || !receiver) return false;

    if (proposerMoney < 0 || receiverMoney < 0) return false;

    if (proposer->getMoney() < proposerMoney) return false;
    if (receiver->getMoney() < receiverMoney) return false;

    for (size_t i = 0; i < proposerProperties.getSize(); ++i) {
        Property* prop = proposerProperties[i];
        if (!prop || prop->getOwner() != proposer) return false;
    }

    for (size_t i = 0; i < receiverProperties.getSize(); ++i) {
        Property* prop = receiverProperties[i];
        if (!prop || prop->getOwner() != receiver) return false;
    }

    return true;
}

const Vector<Property*>& Trade::getProposerProperties() const {
    return proposerProperties;
}

const Vector<Property*>& Trade::getReceiverProperties() const {
    return receiverProperties;
}

int Trade::getProposerMoney() const {
    return proposerMoney;
}

int Trade::getReceiverMoney() const {
    return receiverMoney;
}

bool Trade::getIsAccepted() const {
    return isAccepted;
}

Player* Trade::getProposer() const {
    return proposer;
}

Player* Trade::getReceiver() const {
    return receiver;
}