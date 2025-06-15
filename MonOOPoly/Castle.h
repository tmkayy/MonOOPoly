#pragma once
#include "Mortgage.h"
#include "Consts.h"
class Castle : public Mortgage
{
public:
	Mortgage* clone() const override;

	void increaseRent(Property& property) override;

};

