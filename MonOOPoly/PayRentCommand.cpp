#include "PayRentCommand.h"
#include "Bank.h"
#include "Monopoly.h"
#include "DeclareBankruptcyCommand.h"
#include <iostream>

PayRentCommand::PayRentCommand(Player& payer, Property& property)
	: payer(payer), receiver(*property.getOwner()), property(property),
	amountPaid(0) {
}

void PayRentCommand::execute() {
	amountPaid = property.getPriceForRent();
	if (Bank::transferMoney(payer, receiver, amountPaid)) {
		std::cout << Green << payer.tokenToString() << " paid rent of $" << amountPaid << " to " << receiver.tokenToString() << "." << Reset << std::endl;
		return;
	}

	Monopoly* game = payer.getGame();
	std::cout << Red << payer.tokenToString() << " does not have enough money to pay rent ($" << amountPaid << ")." << Reset << std::endl;
	while (payer.getMoney() < amountPaid) {
		std::cout << Yellow << "You need to raise funds to pay rent." << Reset << std::endl;
		int choice = game->showPlayerOptions();

		if (choice == 3) { //sell property
			game->handleSellProperty();
		}
		else if (choice == 2) { //trade
			game->handleTradeOptions();
		}
		else if (choice == 6) { //declare bankruptcy
			game->executeCommand(new DeclareBankruptcyCommand(payer, game->getGameBoard().getProperties()));
			return;
		}
		else {
			std::cout << Yellow << "You must sell or trade to raise money, or declare bankruptcy." << Reset << std::endl;
		}

		if (payer.isBankrupt()) return;
	}

	//try to pay again
	if (!Bank::transferMoney(payer, receiver, amountPaid)) {
		std::cout << Red << payer.tokenToString() << " still does not have enough money to pay rent ($" << amountPaid << ")." << Reset << std::endl;
		game->executeCommand(new DeclareBankruptcyCommand(payer, game->getGameBoard().getProperties()));
	}
}

void PayRentCommand::undo() {
	Bank::transferMoney(receiver, payer, amountPaid);
}

GameCommand* PayRentCommand::clone() const {
	return new PayRentCommand(*this);
}