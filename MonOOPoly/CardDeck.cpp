#include "CardDeck.h"
#include "PaymentCard.h"
#include "GroupPaymentCard.h"
#include "MovePositionCard.h"

CardDeck::CardDeck()
{
	cards = Stack<Card*>(cardCount);
	generateDeck();
}

void CardDeck::generateDeck()
{
	static bool seeded = false;
	if (!seeded) {
		std::srand((std::time(nullptr)));
		seeded = true;
	}

	free();

	for (int i = 0; i < cardCount; ++i) {
		int type = std::rand() % 3; // 0: PaymentCard, 1: GroupPaymentCard, 2: MovePositionCard
		double value = 0;
		switch (type) {
		case 0: // PaymentCard
			value = (std::rand() % 401) - 200; // -200 to 200
			cards.push(new PaymentCard(value));
			break;
		case 1: // GroupPaymentCard
			value = (std::rand() % 101) - 50; // -50 to 50
			cards.push(new GroupPaymentCard(value));
			break;
		case 2: // MovePositionCard
			value = (std::rand() % 21) - 10; // -10 to 10
			cards.push(new MovePositionCard(static_cast<int>(value)));
			break;
		}
	}
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

Stack<Card*> CardDeck::getCards() const
{
	return cards;
}

Card* CardDeck::drawCard() {
	if (cards.isEmpty()) {
		return nullptr;
	}
	Card* card = cards.top();
	cards.pop();
	return card;
}

void CardDeck::returnCard(Card* card) {
	if (card) {
		cards.push(card);
	}
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