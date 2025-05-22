#include "Property.h"

Field* Property::clone() const
{
    return new Property(*this);
}
