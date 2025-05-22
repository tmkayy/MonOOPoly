#pragma once
#include "Field.h"
#include "Player.h"
#include "Mortgage.h"
#include "Vector.hpp"

class Property:public Field
{
	double priceToBuy;
	double priceForCottage;
	double priceForCastle;
	Player* owner = nullptr;
	Vector<Mortgage*> mortgages;

public:
	Property() :priceToBuy(0), priceForCottage(0), priceForCastle(0), owner(nullptr), mortgages(){}
	void buy(Player* owner);
	void buildCottage();//add arg cottage
	void buildCastle();	//add arg castle
	// Inherited via Field
	Field* clone() const override;
};

