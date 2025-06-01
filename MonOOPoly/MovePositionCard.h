#pragma once
#include "Card.h"
class MovePositionCard :public Card
{
	int tiles = 0;
public:
	MovePositionCard() = default;
	MovePositionCard(int tiles);
	Card* clone() const override;
	void applyEffect(Player& player) const override;
};

