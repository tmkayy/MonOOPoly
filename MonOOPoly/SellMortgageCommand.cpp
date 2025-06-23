#include "SellMortgageCommand.h"
#include <iostream>

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
		std::cout << Green << player->tokenToString() << " sold cottage on " << property.getName()
			<< colorToString(property.getColor()) << Reset << std::endl;
	}
	else {
		Bank::addMoney(*player, property.getPriceForCastle() / 2);
		std::cout << Green << player->tokenToString() << " sold castle on " << property.getName()
			<< colorToString(property.getColor()) << Reset << std::endl;
	}

	delete m;
	property.getMortgages().pop_back();
	sold = true;

	//check if monopoly was lost
	if (hadMonopoly && !player->hasMonopoly(property.getColor())) {
		monopolyLost = true;
		std::cout << Yellow << player->tokenToString() << " lost monopoly on "
			<< colorToString(property.getColor()) << " properties!" << Reset << std::endl;
	}
}

void SellMortgageCommand::undo() {
	if (!sold) return;

	if (wasCottage) {
		if (Bank::subtractMoney(*player, property.getPriceForCottage() / 2)) {
			Cottage* cottage = new Cottage();
			property.getMortgages().push_back(cottage);
			cottage->increaseRent(property);
			std::cout << Cyan << "Undo: Rebuilt cottage on "
				<< colorToString(property.getColor()) << Reset << std::endl;
		}
	}
	else {
		if (Bank::subtractMoney(*player, property.getPriceForCastle() / 2)) {
			Castle* castle = new Castle();
			property.getMortgages().push_back(castle);
			castle->increaseRent(property);
			std::cout << Cyan << "Undo: Rebuilt castle on "
				<< colorToString(property.getColor()) << Reset << std::endl;
		}
	}

	if (monopolyLost) {
		std::cout << Green << "Undo: Regained monopoly on "
			<< colorToString(property.getColor()) << Reset << std::endl;
	}
}

GameCommand* SellMortgageCommand::clone() const {
	return new SellMortgageCommand(*this);
}
