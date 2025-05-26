#include "MovePositionCard.h"

Card* MovePositionCard::clone() const
{
    return new MovePositionCard(*this);
}

void MovePositionCard::applyEffect(const Player& player) const
{
}
