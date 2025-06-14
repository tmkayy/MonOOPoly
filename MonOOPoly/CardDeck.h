#pragma once
#include "Card.h"
#include "PaymentCard.h"
#include "GroupPaymentCard.h"
#include "MovePositionCard.h"
#include "Stack.hpp"
#include "Vector.hpp"
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

    Stack<Card*> getCards() const;
    Card* drawCard();
    void returnCard(Card* card);

    void shuffle();
};

