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
	if (paymentAmount >= 0) {
		for (size_t i = 0; i < allPlayers.getSize(); ++i) {
			if (allPlayers[i] != &player) {
				Bank::transferMoney(*allPlayers[i], player, paymentAmount);
			}
		}
		std::cout << "All players pay " << Green << paymentAmount << "$" << Reset << " to " << Tokens[(size_t)player.getUsername()] << "." << std::endl;
	}
	else {
		for (size_t i = 0; i < allPlayers.getSize(); ++i) {
			if (allPlayers[i] != &player) {
				Bank::transferMoney(player, *allPlayers[i], -paymentAmount);
			}
		}
		std::cout << Tokens[(size_t)player.getUsername()] << " pays " << Green << -paymentAmount << "$" << Reset << " to all players." << std::endl;
	}
}

void GroupPaymentCard::reverseEffect(Player& player, const Vector<Player*>& allPlayers) const {
	if (paymentAmount >= 0) {
		for (size_t i = 0; i < allPlayers.getSize(); i++) {
			if (allPlayers[i] != &player) {
				Bank::transferMoney(player, *allPlayers[i], paymentAmount);
			}
		}
	}
	else {
		for (size_t i = 0; i < allPlayers.getSize(); i++) {
			if (allPlayers[i] != &player) {
				Bank::transferMoney(*allPlayers[i], player, -paymentAmount);
			}
		}
	}
}

double GroupPaymentCard::getPaymentAmount() const
{
	return paymentAmount;
}
