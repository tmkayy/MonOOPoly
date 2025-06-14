#pragma once
#include "Field.h"
class Jail:public Field
{
public:
	Field* clone() const override;
    void onLand(Player& player) override;

    static void sendToJail(Player& player);
    static bool attemptRelease(Player& player, bool payBail, bool rolledDoubles);
};

