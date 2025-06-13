#include "PaymentCard.h"

PaymentCard::PaymentCard(double money)
{
    this->moneyAmount = money;
}

Card* PaymentCard::clone() const
{
    return new PaymentCard(*this);
}

void PaymentCard::applyEffect(Player& player, const Vector<Player*>& allPlayers) const
{
    if (moneyAmount < 0)
        Bank::subtractMoney(player, moneyAmount);
    else
        Bank::addMoney(player, moneyAmount);
}
