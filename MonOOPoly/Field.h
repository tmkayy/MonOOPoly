#pragma once
#include "Player.h"
class Field
{

public:
	virtual Field* clone() const = 0;
	virtual ~Field() = default;
	Field() = default;

	virtual void onLand(Player& player) {}
	virtual void onPass(Player& player) {}
};

