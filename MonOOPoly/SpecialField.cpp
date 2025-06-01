#include "SpecialField.h"

SpecialField::SpecialField(Type type)
{
	fieldType = type;
}

Field* SpecialField::clone() const
{
	return new SpecialField(*this);
}
