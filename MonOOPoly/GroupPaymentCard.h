#pragma once
#include "Card.h"
class GroupPaymentCard:public Card
{
public:
	Card* clone() const override;
	void applyEffect(const Player& player) const override;
};

