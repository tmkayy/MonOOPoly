#include "CardField.h"
#include "CardDeck.h"
#include "Player.h"

Field* CardField::clone() const
{
	return new CardField(*this);
}

void CardField::onLand(Player& player)
{
	//drawCard(player, ...); //TODO
}

bool CardField::drawCard(Player& player, CardDeck& deck)
{
	if (deck.getCards().isEmpty()) {
		return false;
	}
	/*deck.getCards().top()->applyEffect(player);*/ //TODO
	deck.getCards().pop();
	return true;
}
