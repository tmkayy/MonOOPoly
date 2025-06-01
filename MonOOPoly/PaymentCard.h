#pragma once
#include "Card.h"
class PaymentCard : public Card
{
	double moneyAmount = 0;
public:
	Card* clone() const override;
	void applyEffect(Player& player) const override;
};

