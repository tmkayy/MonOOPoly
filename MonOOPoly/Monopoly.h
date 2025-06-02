#pragma once
#include "Vector.hpp"
#include "Bank.h"
#include "Player.h"
#include "Board.h"
#include "CardDeck.h"

class Monopoly
{
	Vector<Player*> players;
	Board board;
	CardDeck cards;
	Bank bank;
};

