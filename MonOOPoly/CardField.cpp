#include "CardField.h"
#include "CardDeck.h"
#include "Player.h"

Field* CardField::clone() const
{
	return new CardField(*this);
}

void CardField::onLand(Player& player, CardDeck& deck, const Vector<Player*>& allPlayers)
{
	drawCard(player, deck, allPlayers);
}

Card* CardField::drawCard(Player& player, CardDeck& deck, const Vector<Player*>& allPlayers)
{
    Card* card = deck.drawCard();
    if (!card) {
        return nullptr;
    }
    card->applyEffect(player, allPlayers);
    return card;
}
