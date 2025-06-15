#pragma once
#include "Mortgage.h"
#include "Consts.h"
class Cottage : public Mortgage
{
public:
	Mortgage* clone() const override;
	void increaseRent(Property& property) override;
};

