#include "SpecialField.h"

SpecialField::SpecialField(Type type, const MyString& name, double value)
	: fieldType(type), name(name), value(value)
{
}

Field* SpecialField::clone() const
{
	return new SpecialField(*this);
}
