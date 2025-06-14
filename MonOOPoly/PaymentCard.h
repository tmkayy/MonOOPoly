#pragma once
#include "Card.h"
#include "Bank.h"
class PaymentCard : public Card
{
	double moneyAmount = 0;
public:
	PaymentCard() = default;
	PaymentCard(double money);
	Card* clone() const override;
	void applyEffect(Player& player, const Vector<Player*>& allPlayers) const override;
	void reverseEffect(Player& player, const Vector<Player*>& allPlayers) const override;
};

