#pragma once
#include "Vector.hpp"
#include "Field.h"
#include "Jail.h"
#include "CardField.h"
#include "SpecialField.h"

class Property;
class Board
{
    Vector<Field*> board;
    Vector<Property*> properties;

    void free();
    void copyFrom(const Board& other);
    void moveFrom(Board&& other) noexcept;
    void initializeProperties();

public:
    Board();
    ~Board();
    Board(const Board& other);
    Board(Board&& other) noexcept;
    Board& operator=(const Board& other);
    Board& operator=(Board&& other) noexcept;

    static void setId(Player& player, int n);

    Vector<Field*> getBoard() const;
    Vector<Property*> getProperties() const;
    Vector<Property*> getPropertiesByColor(PropertyColor color) const;
};