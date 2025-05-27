#include "CardField.h"

Field* CardField::clone() const
{
    return new CardField(*this);
}

void CardField::drawCard(Player& player, Stack<Card*> cards)
{
    cards.top()->applyEffect(player);
}
