#pragma once
#include "Card.h"
#include "Stack.hpp"
#include "Consts.h"
class CardDeck
{
	Stack<Card*> cards;

	void free();
	void copyFrom(const CardDeck& other);
	void moveFrom(CardDeck&& other) noexcept;

public:
    CardDeck();
    ~CardDeck();
    CardDeck(const CardDeck& other);
    CardDeck(CardDeck&& other) noexcept;
    CardDeck& operator=(const CardDeck& other);
    CardDeck& operator=(CardDeck&& other) noexcept;

    void clear();
};

