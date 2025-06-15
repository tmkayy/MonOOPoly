#include "Cottage.h"
#include "Property.h"

Mortgage* Cottage::clone() const
{
	return new Cottage(*this);
}

void Cottage::increaseRent(Property& property)
{
	property.setPriceForRent(property.getPriceForRent() * cottageRent);
}
