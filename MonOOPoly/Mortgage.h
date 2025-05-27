#pragma once
#include "Property.h"

class Mortgage
{
public:
	virtual ~Mortgage() = default;
	virtual Mortgage* clone() const = 0;
	virtual void increaseRent(Property& property) = 0;
};

