#include "SpecialField.h"
#include "Bank.h"

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

void SpecialField::onLand(Player& player) {
    switch (fieldType) {
    case Type::GO:
        //nothing happens unless you pass
        break;
    case Type::FREE_PARKING:
        //nothing ever happens...
        break;
    case Type::JAIL:
        //just visiting
        break;
    case Type::GO_TO_JAIL:
        player.setImprisoned(true);
        player.setTurnsInJail(0);
        //move to jail in the monopoly class
        break;
    case Type::INCOME_TAX:
        Bank::subtractMoney(player, value);
        break;
    case Type::LUXURY_TAX:
        Bank::subtractMoney(player, value);
        break;
    }
}

void SpecialField::onPass(Player& player) {
    if (fieldType == Type::GO) {
        Bank::addMoney(player, value > 0 ? value : goMoney);
    }
}