#include "GroupPaymentCard.h"

Card* GroupPaymentCard::clone() const
{
    return new GroupPaymentCard(*this);
}

void GroupPaymentCard::applyEffect(const Player& player) const
{
}
