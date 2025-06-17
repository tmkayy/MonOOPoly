#include "CardDeck.h"
#include "PaymentCard.h"
#include "GroupPaymentCard.h"
#include "MovePositionCard.h"

CardDeck::CardDeck()
{
    cards = Stack<Card*>(cardCount);
    cards.push(new MovePositionCard(5));   
    cards.push(new MovePositionCard(7));  
    cards.push(new MovePositionCard(6));  
    cards.push(new MovePositionCard(11));  
    cards.push(new MovePositionCard(-3));  
    cards.push(new MovePositionCard(-2));   
    cards.push(new MovePositionCard(10));  
    cards.push(new MovePositionCard(8));  
    cards.push(new MovePositionCard(1));  
    cards.push(new MovePositionCard(-1));  

    cards.push(new GroupPaymentCard(50)); 
    cards.push(new GroupPaymentCard(-50));
    cards.push(new GroupPaymentCard(10)); 

    cards.push(new PaymentCard(100));  
    cards.push(new PaymentCard(-15));  
    cards.push(new PaymentCard(150));  
    cards.push(new PaymentCard(-20));  
    cards.push(new PaymentCard(-25));  
    cards.push(new PaymentCard(10));   
    cards.push(new PaymentCard(50));   
    cards.push(new PaymentCard(-100)); 
    cards.push(new PaymentCard(-150)); 
    cards.push(new PaymentCard(200));  
    cards.push(new PaymentCard(-100)); 

    cards.push(new MovePositionCard(0)); 
    cards.push(new MovePositionCard(10));
    cards.push(new MovePositionCard(5)); 

    cards.push(new PaymentCard(200));    
    cards.push(new PaymentCard(-50));    
    cards.push(new PaymentCard(100));    
    cards.push(new PaymentCard(20));     
    cards.push(new PaymentCard(100));    
    cards.push(new PaymentCard(-100));   
    cards.push(new PaymentCard(-150));   
    cards.push(new PaymentCard(25));     
    cards.push(new PaymentCard(100));    
    cards.push(new PaymentCard(45));     
    cards.push(new PaymentCard(100));    
    cards.push(new PaymentCard(50));     
    cards.push(new PaymentCard(-50));    
    cards.push(new PaymentCard(10));     
    cards.push(new PaymentCard(25));     
    cards.push(new PaymentCard(-40));    
    cards.push(new PaymentCard(100));    
    cards.push(new PaymentCard(-30));    
    cards.push(new PaymentCard(75));     
    cards.push(new PaymentCard(-20));    

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