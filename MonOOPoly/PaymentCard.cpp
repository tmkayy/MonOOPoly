#include "PaymentCard.h"
#include "Bank.h"

PaymentCard::PaymentCard(double money)
{
	this->moneyAmount = money;
}

Card* PaymentCard::clone() const
{
	return new PaymentCard(*this);
}

void PaymentCard::applyEffect(Player& player, const Vector<Player*>& allPlayers) const
{
	if (moneyAmount < 0) {
		Bank::subtractMoney(player, moneyAmount);
		std::cout << "You have paid " << -moneyAmount << "$." << std::endl;
	}
	else {
		Bank::addMoney(player, moneyAmount);
		std::cout << "You have received " << moneyAmount << "$." << std::endl;
	}
}


void PaymentCard::reverseEffect(Player& player, const Vector<Player*>& allPlayers) const {
	Bank::addMoney(player, moneyAmount); // Give the money back
}