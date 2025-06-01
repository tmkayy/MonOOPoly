#pragma once
#include "Vector.hpp"
#include "Field.h"
#include "Property.h"
#include "Jail.h"
#include "CardField.h"
#include "SpecialField.h"

class Board
{
    Vector<Field*> board;

    void free();
    void copyFrom(const Board& other);
    void moveFrom(Board&& other) noexcept;

public:
    Board();
    ~Board();
    Board(const Board& other);
    Board(Board&& other) noexcept;
    Board& operator=(const Board& other);
    Board& operator=(Board&& other) noexcept;

    Vector<Field*> getBoard() const;
};
