#pragma once
#include "Card.h"
class PaymentCard : public Card
{
public:
	Card* clone() const override;
	void applyEffect(const Player& player) const override;
};

