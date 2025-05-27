#pragma once
#include "Field.h"
#include "Player.h"
#include "Stack.hpp"
#include "Card.h"
class CardField:public Field
{
public:
	Field* clone() const override;
	void drawCard(Player& player, Stack<Card*> cards);
};

