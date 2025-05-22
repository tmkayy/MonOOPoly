#include "CardField.h"

Field* CardField::clone() const
{
    return new CardField(*this);
}
