#include "SellMortgageCommand.h"

SellMortgageCommand::SellMortgageCommand(Player* player, Property& property)
    : player(player), property(property), wasCottage(false),
    sold(false), monopolyLost(false) {
}

void SellMortgageCommand::execute() {
    if (!player || property.getOwner() != player) return;
    if (property.getMortgages().isEmpty()) return;

    //check monopoly status before selling
    bool hadMonopoly = player->hasMonopoly(property.getColor());

    Mortgage* m = property.getMortgages().peek();
    wasCottage = dynamic_cast<Cottage*>(m) != nullptr;

    if (wasCottage) {
        Bank::addMoney(*player, property.getPriceForCottage() / 2);
        std::cout << player->tokenToString() << " sold cottage on "
            << Property::colorToString(property.getColor()) << "\n";
    }
    else {
        Bank::addMoney(*player, property.getPriceForCastle() / 2);
        std::cout << player->tokenToString() << " sold castle on "
            << Property::colorToString(property.getColor()) << "\n";
    }

    delete m;
    property.getMortgages().pop_back();
    sold = true;

    //check if monopoly was lost
    if (hadMonopoly && !player->hasMonopoly(property.getColor())) {
        monopolyLost = true;
        std::cout << player->tokenToString() << " lost monopoly on "
            << Property::colorToString(property.getColor()) << " properties!\n";
    }
}

void SellMortgageCommand::undo() {
    if (!sold) return;

    if (wasCottage) {
        if (Bank::subtractMoney(*player, property.getPriceForCottage() / 2)) {
            Cottage* cottage = new Cottage();
            property.getMortgages().push_back(cottage);
            cottage->increaseRent(property);
            std::cout << "Undo: Rebuilt cottage on "
                << Property::colorToString(property.getColor()) << "\n";
        }
    }
    else {
        if (Bank::subtractMoney(*player, property.getPriceForCastle() / 2)) {
            Castle* castle = new Castle();
            property.getMortgages().push_back(castle);
            castle->increaseRent(property);
            std::cout << "Undo: Rebuilt castle on "
                << Property::colorToString(property.getColor()) << "\n";
        }
    }

    if (monopolyLost) {
        std::cout << "Undo: Regained monopoly on "
            << Property::colorToString(property.getColor())<< "\n";
    }
}

GameCommand* SellMortgageCommand::clone() const {
    return new SellMortgageCommand(*this);
}