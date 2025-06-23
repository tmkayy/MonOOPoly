#include "PayRentCommand.h"
#include "Bank.h"
#include "Monopoly.h"
#include "DeclareBankruptcyCommand.h"

PayRentCommand::PayRentCommand(Player& payer, Property& property)
	: payer(payer), receiver(*property.getOwner()), property(property),
	amountPaid(0) {
}

void PayRentCommand::execute() {
	amountPaid = property.getPriceForRent();
	if (Bank::transferMoney(payer, receiver, amountPaid))
		return;

	Monopoly* game = payer.getGame();
	std::cout << payer.tokenToString() << " does not have enough money to pay rent ($" << amountPaid << ").\n";
	while (payer.getMoney() < amountPaid) {
		std::cout << "You need to raise funds to pay rent.\n";
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
			std::cout << "You must sell or trade to raise money, or declare bankruptcy.\n";
		}

		if (payer.isBankrupt()) return;
	}

	//try to pay again
	if (!Bank::transferMoney(payer, receiver, amountPaid)) {
		std::cout << payer.tokenToString() << " still does not have enough money to pay rent ($" << amountPaid << ").\n";
		game->executeCommand(new DeclareBankruptcyCommand(payer, game->getGameBoard().getProperties()));
	}
}

void PayRentCommand::undo() {
	Bank::transferMoney(receiver, payer, amountPaid);
}

GameCommand* PayRentCommand::clone() const {
	return new PayRentCommand(*this);
}