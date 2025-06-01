#pragma once
#include "Field.h"
#include "Player.h"
#include "Stack.hpp"
#include "Card.h"
#include "CardDeck.h"
class CardField:public Field
{
public:
	Field* clone() const override;
	bool drawCard(Player& player, CardDeck& deck);
};

