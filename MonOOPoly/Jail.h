#pragma once
#include "Field.h"
class Jail:public Field
{
	Field* clone() const override;
};

