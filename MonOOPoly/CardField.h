#pragma once
#include "Field.h"
#include "Stack.hpp"
#include "Card.h"
class CardDeck;
class Player;
class CardField:public Field
{
	bool drawCard(Player& player, CardDeck& deck);
public:
	Field* clone() const override;
	void onLand(Player& player);
};

