#pragma once
#include "Field.h"
#include "MyString.h"

class SpecialField : public Field {
public:
    enum class Type {
        GO,
        FREE_PARKING,
        JAIL,
        GO_TO_JAIL,
        INCOME_TAX,
        LUXURY_TAX,
        UTILITY,
        RAILROAD
    };

private:
    Type fieldType;
    MyString name;
    double value;

public:
    explicit SpecialField(Type type, const MyString& name = "", double value = 0);
    Field* clone() const override;

    Type getType() const { return fieldType; }
    const MyString& getName() const { return name; }
    double getValue() const { return value; }
};