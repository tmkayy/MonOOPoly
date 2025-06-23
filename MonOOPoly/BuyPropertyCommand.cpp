#include "BuyPropertyCommand.h"

BuyPropertyCommand::BuyPropertyCommand(Player& buyer, Property& property)
    : buyer(buyer), property(property), purchased(false) {
}

void BuyPropertyCommand::execute() {
    purchased = buyer.buyProperty(property);
}

void BuyPropertyCommand::undo() {
    if (purchased) {
        buyer.sellProperty(property);
    }
}

GameCommand* BuyPropertyCommand::clone() const {
    return new BuyPropertyCommand(*this);
}
