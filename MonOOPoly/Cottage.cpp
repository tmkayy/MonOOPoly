#include "Cottage.h"
#include "Consts.h"

Mortgage* Cottage::clone() const
{
	return new Cottage(*this);
}

void Cottage::increaseRent(Property& property)
{
	property.setPriceForRent(property.getPriceForRent() * cottageRent);
}
