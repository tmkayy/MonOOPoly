#include "Jail.h"

Field* Jail::clone() const
{
    return new Jail(*this);
}
