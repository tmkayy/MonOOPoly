#pragma once
#include "Field.h"
#include "Player.h"
#include "Stack.hpp"
#include "Card.h"
#include "CardDeck.h"
class CardField:public Field
{
	bool drawCard(Player& player, CardDeck& deck);
public:
	Field* clone() const override;
	void onLand(Player& player) override;
};

