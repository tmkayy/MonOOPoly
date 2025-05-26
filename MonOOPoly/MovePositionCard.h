#pragma once
#include "Card.h"
class MovePositionCard :public Card
{
public:
	Card* clone() const override;
	void applyEffect(const Player& player) const override;
};

