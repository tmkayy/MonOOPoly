#include "CardDeck.h"
#include <random>
#include "CardField.h"


CardDeck::CardDeck()
{
    cards = Stack<Card*>(cardCount);
    //24 chance cards
    cards.push(new MovePositionCard(5));   // Advance to Illinois Ave
    cards.push(new MovePositionCard(39));  // Advance to Boardwalk
    cards.push(new MovePositionCard(24));  // Advance to St. Charles Place
    cards.push(new MovePositionCard(11));  // Advance to nearest Utility
    cards.push(new MovePositionCard(-3));  // Go back 3 spaces
    cards.push(new MovePositionCard(0));   // Advance to GO
    cards.push(new MovePositionCard(10));  // Go to Jail
    cards.push(new MovePositionCard(15));  // Advance to nearest Railroad
    cards.push(new MovePositionCard(15));  // Advance to nearest Railroad (duplicate)
    cards.push(new MovePositionCard(11));  // Advance to nearest Utility (duplicate)

    cards.push(new GroupPaymentCard(50));   // Pay each player $50
    cards.push(new GroupPaymentCard(-50)); // Collect $50 from each player
    cards.push(new GroupPaymentCard(10));   // Collect $10 from each player (birthday)

    cards.push(new PaymentCard(100));       // Bank pays you dividend of $100
    cards.push(new PaymentCard(-15));       // Pay poor tax of $15
    cards.push(new PaymentCard(150));      // Your building loan matures, collect $150
    cards.push(new PaymentCard(-20));      // Speeding fine $20
    cards.push(new PaymentCard(-25));      // Pay for street repairs
    cards.push(new PaymentCard(10));       // You have won a crossword competition, collect $10
    cards.push(new PaymentCard(50));       // Grand Opera Night, collect $50 from each player
    cards.push(new PaymentCard(-100));     // Pay hospital $100
    cards.push(new PaymentCard(-150));     // Pay school tax of $150
    cards.push(new PaymentCard(200));      // Bank error in your favor, collect $200
    cards.push(new PaymentCard(-100));     // Pay for general repairs

    // Community Chest cards (24 cards)
    cards.push(new MovePositionCard(0));   // Advance to GO
    cards.push(new MovePositionCard(10));  // Go to Jail
    cards.push(new MovePositionCard(5));   // Advance to Illinois Ave (duplicate)

    cards.push(new PaymentCard(200));      // Bank error in your favor, collect $200
    cards.push(new PaymentCard(-50));      // Doctor's fee, pay $50
    cards.push(new PaymentCard(100));      // From sale of stock, you get $100
    cards.push(new PaymentCard(20));       // Income tax refund, collect $20
    cards.push(new PaymentCard(100));     // Receive for services $100
    cards.push(new PaymentCard(-100));    // Pay hospital $100
    cards.push(new PaymentCard(-150));     // Pay school tax of $150
    cards.push(new PaymentCard(25));       // You inherit $25
    cards.push(new PaymentCard(100));      // Life insurance matures, collect $100
    cards.push(new PaymentCard(45));       // You won second prize in a beauty contest, collect $45
    cards.push(new PaymentCard(100));      // Holiday fund matures, collect $100
    cards.push(new PaymentCard(50));       // Receive consultancy fee $50
    cards.push(new PaymentCard(-50));      // Pay for general repairs $50
    cards.push(new PaymentCard(10));       // Collect $10 interest on loan
    cards.push(new PaymentCard(25));       // Collect $25 from sale of stock
    cards.push(new PaymentCard(-40));      // Pay for street repairs $40
    cards.push(new PaymentCard(100));      // You have won a prize in a lottery, collect $100
    cards.push(new PaymentCard(-30));      // Pay for school fees $30
    cards.push(new PaymentCard(75));       // Collect $75 from matured insurance
    cards.push(new PaymentCard(-20));      // Pay for a fine $20

    shuffle();
}

void CardDeck::shuffle()
{
    Vector<Card*> tempVector;
    while (!cards.isEmpty()) {
        tempVector.push_back(cards.top());
        cards.pop();
    }

    // shuffle using Fisher-Yates algorithm
    std::random_device rd;
    std::mt19937 g(rd());

    for (size_t i = tempVector.getSize() - 1; i > 0; --i) {
        std::uniform_int_distribution<size_t> dist(0, i);
        size_t j = dist(g);
        std::swap(tempVector[i], tempVector[j]);
    }
    for (size_t i = 0; i < tempVector.getSize(); ++i) { //move back to stack
        cards.push(tempVector[i]);
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