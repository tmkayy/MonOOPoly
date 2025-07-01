#pragma once
#include "Field.h"
#include "MyString.h"
#include "Bank.h"

class Player;
class SpecialField : public Field {
public:
    enum class Type {
        GO,
        FREE_PARKING,
        GO_TO_JAIL,
        INCOME_TAX,
        LUXURY_TAX
    };

private:
    Type fieldType;
    MyString name;
    double value;

public:
    explicit SpecialField(Type type, const MyString& name = "", double value = 0);
    Field* clone() const override;

    Type getType() const;
    const MyString& getName() const;
    double getValue() const;
};