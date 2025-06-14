#include "SellPropertyCommand.h"

SellPropertyCommand::SellPropertyCommand(Player& seller, Property& property)
    : seller(seller), property(property), sold(false) {
}

void SellPropertyCommand::execute() {
    sold = seller.sellProperty(property);
}

void SellPropertyCommand::undo() {
    if (sold) {
        seller.buyProperty(property);
    }
}

GameCommand* SellPropertyCommand::clone() const {
    return new SellPropertyCommand(*this);
}