#include "PayJailFineCommand.h"
#include "Jail.h"
#include <iostream>
#include "Monopoly.h"
#include "DeclareBankruptcyCommand.h"

PayJailFineCommand::PayJailFineCommand(Player& player)
	: player(player), paid(false), wasInJail(false) {
}

void PayJailFineCommand::execute() {
	wasInJail = player.isImprisoned();
	if (!wasInJail)
		return;

	Monopoly* game = player.getGame();
	paid = Bank::subtractMoney(player, bailCost);
	if (paid) {
		player.setImprisoned(false);
		std::cout << Green << player.tokenToString()
			<< " paid $100 jail fine and was released" << Reset << std::endl;
		return;
	}

	std::cout << Yellow << player.tokenToString()
		<< " couldn't pay $100 jail fine." << Reset << std::endl;
	player.setTurnsBankrupt(player.getTurnsBankrupt() + 1);
	if (player.getTurnsBankrupt() == 2) {
		std::cout << Red << player.tokenToString()
			<< " still does not have enough money to pay bail ($" << bailCost << ")." << Reset << std::endl;
		game->executeCommand(new DeclareBankruptcyCommand(player, game->getGameBoard().getProperties()));
	}



	while (!paid && !player.isBankrupt()) {
		std::cout << Yellow << "You must raise funds to pay the jail fine ($100)." << Reset << std::endl;
		std::cout << Yellow << "Use the menu to sell property, trade, or declare bankruptcy." << Reset << std::endl;
		int choice = game->showPlayerOptions();

		switch (choice) {
		case 3: // sell property
			game->handleSellProperty();
			break;
		case 2: // trade
			game->handleTradeOptions();
			break;
		case 6: // declare bankruptcy
			game->executeCommand(new DeclareBankruptcyCommand(player,
				game->getGameBoard().getProperties()));
			return;
		case 4:
			game->handlePendingTrades();
			break;
		case 5: //next turn
			system("cls");
			return;
		default:
			std::cout << Red << "Invalid choice. Please try again." << Reset << std::endl;
			continue;
		}

		//try again
		paid = Bank::subtractMoney(player, bailCost);
		if (paid) {
			player.setImprisoned(false);
			std::cout << Green << player.tokenToString()
				<< " paid $100 jail fine and was released" << Reset << std::endl;
			player.setTurnsBankrupt(0);
			return;
		}
	}
}

void PayJailFineCommand::undo() {
	if (paid && wasInJail) {
		//only undo if payment actually occurred
		Bank::addMoney(player, bailCost);  //refund the fine
		player.setImprisoned(true);   //put back in jail
		std::cout << Cyan << player.tokenToString()
			<< " $100 jail fine was refunded and player returned to jail" << Reset << std::endl;
	}
}

GameCommand* PayJailFineCommand::clone() const {
	return new PayJailFineCommand(*this);
}

