#pragma once
#include "Field.h"
class Player;
class Jail:public Field
{
public:
	Field* clone() const override;

    static void sendToJail(Player& player);
};

