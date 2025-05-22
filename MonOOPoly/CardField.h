#pragma once
#include "Field.h"
#include "Player.h"
#include "Vector.hpp"
class CardField:public Field
{
public:
	Field* clone() const override;
	void drawCard(Player& player);//vector card
};

