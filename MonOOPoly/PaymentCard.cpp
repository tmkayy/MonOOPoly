#include "PaymentCard.h"

Card* PaymentCard::clone() const
{
    return new PaymentCard(*this);
}

void PaymentCard::applyEffect(const Player& player) const
{
}
