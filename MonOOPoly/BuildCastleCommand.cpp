#include "BuildCastleCommand.h"

BuildCastleCommand::BuildCastleCommand(Player* player, Property& property)
	: player(player), property(property), built(false) {
}

void BuildCastleCommand::execute() {
	if (!player || property.getOwner() != player) return;
	if (!player->hasMonopoly(property.getColor())) return;

	if (Bank::subtractMoney(*player, property.getPriceForCastle())) {
		Castle* castle = new Castle();
		property.getMortgages().push_back(castle);
		castle->increaseRent(property);
		built = true;
		std::cout << Yellow << "Castle built on " << property.getName() << Reset << std::endl;
	}
	else
		std::cout << Red << "Not enough money to build a castle on " << property.getName() << Reset << std::endl;
}

void BuildCastleCommand::undo() {
	if (built && !property.getMortgages().isEmpty()) {
		Mortgage* m = property.getMortgages().peek();
		if (dynamic_cast<Castle*>(m)) {
			Bank::addMoney(*player, property.getPriceForCastle() / 2);
			delete m;
			property.getMortgages().pop_back();
		}
	}
}

GameCommand* BuildCastleCommand::clone() const {
	return new BuildCastleCommand(*this);
}

bool BuildCastleCommand::wasBuilt() const {
	return built;
}
