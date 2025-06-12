#include "CardField.h"

Field* CardField::clone() const
{
	return new CardField(*this);
}

void CardField::onLand(Player& player)
{
	drawCard(player, ...); //TODO
}

bool CardField::drawCard(Player& player, CardDeck& deck)
{
	if (deck.getCards().isEmpty()) {
		return false;
	}
	deck.getCards().top()->applyEffect(player);
	deck.getCards().pop();
	return true;
}
