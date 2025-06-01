#include "CardDeck.h"

CardDeck::CardDeck()
{
	
}

CardDeck::~CardDeck()
{
	free();
}

CardDeck::CardDeck(const CardDeck& other)
{
	copyFrom(other);
}

CardDeck::CardDeck(CardDeck&& other) noexcept
{
	moveFrom(std::move(other));
}

CardDeck& CardDeck::operator=(const CardDeck& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

CardDeck& CardDeck::operator=(CardDeck&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}


void CardDeck::clear()
{
	free();
}

void CardDeck::free()
{
	while (cards.getSize() > 0)
	{
		delete cards.top();
		cards.pop();
	}
}

void CardDeck::copyFrom(const CardDeck& other)
{
	Stack<Card*> temp;
	Stack<Card*> source = other.cards;
	while (source.getSize() > 0)
	{
		Card* original = source.top();
		Card* newCard = (*original).clone();
		temp.push(newCard);
		source.pop();
	}
	while (temp.getSize() > 0)
	{
		cards.push(temp.top());
		temp.pop();
	}
}

void CardDeck::moveFrom(CardDeck&& other) noexcept
{
	cards = std::move(other.cards);
	other.cards = Stack<Card*>();
}