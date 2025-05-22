#pragma once
class Field
{
public:
	virtual Field* clone() const = 0;
	virtual ~Field() = default;
	Field() = default;
};

