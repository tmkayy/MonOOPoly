#include "GroupPaymentCard.h"
#include "Bank.h"

GroupPaymentCard::GroupPaymentCard(double money)
{
	this->paymentAmount = money;
}

Card* GroupPaymentCard::clone() const
{
	return new GroupPaymentCard(*this);
}

void GroupPaymentCard::applyEffect(Player& player) const
{
	//TODO
}
