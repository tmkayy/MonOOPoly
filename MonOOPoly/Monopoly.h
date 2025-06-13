#pragma once
#include "Vector.hpp"
#include "Bank.h"
#include "Player.h"
#include "Board.h"
#include "CardDeck.h"
#include "Dice.h"
#include "GameCommand.h"

class Monopoly
{
	Vector<Player*> players;
    Vector<GameCommand*> commandHistory;
	Board board;
	CardDeck cards;
	Bank bank;
	Dice d1;
	Dice d2;

	void copyFrom(const Monopoly& other);
	void moveFrom(Monopoly&& other) noexcept;
	void free();

public:
	Monopoly();
	~Monopoly() noexcept;
	Monopoly(const Monopoly& other);
	Monopoly& operator=(const Monopoly& other);
	Monopoly(Monopoly&& other) noexcept;
	Monopoly& operator=(Monopoly&& other) noexcept;

	void startGame();
	void endGame();
	void nextTurn();

	void executeCommand(GameCommand* command);
	void undoLastCommand();
};

