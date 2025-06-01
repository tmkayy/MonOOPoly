#pragma once
#include "Field.h"

class SpecialField : public Field {
public:
    enum class Type { GO, FREE_PARKING, GO_TO_JAIL };

private:
    Type fieldType;

public:
    explicit SpecialField(Type type);
    Field* clone() const override;
    Type getType() const { return fieldType; }
};

