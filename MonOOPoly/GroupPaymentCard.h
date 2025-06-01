#pragma once
#include "Card.h"
class GroupPaymentCard:public Card
{
	double paymentAmount = 0;
public:
	GroupPaymentCard() = default;
	GroupPaymentCard(double money);
	Card* clone() const override;
	void applyEffect(Player& player) const override;
};

