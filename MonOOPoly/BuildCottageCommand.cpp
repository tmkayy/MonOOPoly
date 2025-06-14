#include "BuildCottageCommand.h"

BuildCottageCommand::BuildCottageCommand(Player* player, Property& property)
    : player(player), property(property), built(false) {
}

void BuildCottageCommand::execute() {
    if (!player || property.getOwner() != player) return;
    if (!player->hasMonopoly(property.getColor())) return;

    if (Bank::subtractMoney(*player, property.getPriceForCottage())) {
        Cottage* cottage = new Cottage();
        property.getMortgages().push_back(cottage);
        cottage->increaseRent(property);
        built = true;
    }
}

void BuildCottageCommand::undo() {
    if (built && !property.getMortgages().isEmpty()) {
        Mortgage* m = property.getMortgages().peek();
        if (dynamic_cast<Cottage*>(m)) {
            Bank::addMoney(*player, property.getPriceForCottage() / 2);
            delete m;
            property.getMortgages().pop_back();
        }
    }
}

GameCommand* BuildCottageCommand::clone() const {
    return new BuildCottageCommand(*this);
}

bool BuildCottageCommand::wasBuilt() const
{
    return built;
}
