#include "Castle.h"
#include "Consts.h"

void Castle::increaseRent(Property& property)
{
	property.setPriceForRent(property.getPriceForRent() * cottageRent);
}

Mortgage* Castle::clone() const
{
	return new Castle(*this);
}
