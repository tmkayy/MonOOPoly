#include "MovePositionCard.h"

MovePositionCard::MovePositionCard(int tiles)
{
    this->tiles = tiles;
}

Card* MovePositionCard::clone() const
{
    return new MovePositionCard(*this);
}

void MovePositionCard::applyEffect(Player& player) const
{
    player.setId(player.getId() + tiles);
}
