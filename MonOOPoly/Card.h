#pragma once
#include "Player.h"
class Card {
public:
	virtual Card* clone() const = 0;
	virtual void applyEffect(Player& player) const = 0;
};