#pragma once
#include "Field.h"
#include "Stack.hpp"
#include "Card.h"
class CardDeck;
class Player;
class CardField:public Field
{
public:
	static Card* drawCard(Player& player, CardDeck& deck, const Vector<Player*>& allPlayers);
	Field* clone() const override;
};

