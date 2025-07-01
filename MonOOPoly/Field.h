#pragma once

class Player;
class Field
{

public:
	virtual Field* clone() const = 0;
	virtual ~Field() = default;
	Field() = default;
};

