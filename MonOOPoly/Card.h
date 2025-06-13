#pragma once
#include "Player.h"
class Card {
public:
	virtual Card* clone() const = 0;
	virtual void applyEffect(Player& player, const Vector<Player*>& allPlayers) const = 0;
};