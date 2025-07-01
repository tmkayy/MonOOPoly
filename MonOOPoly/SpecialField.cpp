#include "SpecialField.h"
#include "Player.h"

SpecialField::SpecialField(Type type, const MyString& name, double value)
	: fieldType(type), name(name), value(value)
{
}

Field* SpecialField::clone() const
{
	return new SpecialField(*this);
}

SpecialField::Type SpecialField::getType() const {
    return fieldType;
}

const MyString& SpecialField::getName() const {
    return name;
}

double SpecialField::getValue() const {
    return value;
}