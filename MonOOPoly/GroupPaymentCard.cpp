#include "GroupPaymentCard.h"
#include "Bank.h"

GroupPaymentCard::GroupPaymentCard(double money)
{
	this->paymentAmount = money;
}

Card* GroupPaymentCard::clone() const
{
	return new GroupPaymentCard(*this);
}

void GroupPaymentCard::applyEffect(Player& player, const Vector<Player*>& allPlayers) const
{
}

void GroupPaymentCard::applyEffect(Player& player, const Vector<Player*>& allPlayers) const
{
	for (size_t i = 0; i < allPlayers.getSize(); ++i) {
		if (allPlayers[i] != &player) {
			Bank::transferMoney(*allPlayers[i], player, paymentAmount);
		}
	}
}

void GroupPaymentCard::reverseEffect(Player& player, const Vector<Player*>& allPlayers) const {
	for (size_t i = 0; i < allPlayers.getSize(); i++) {
		if (allPlayers[i] != &player) {
			Bank::transferMoney(*allPlayers[i], player, paymentAmount);
		}
	}
}
