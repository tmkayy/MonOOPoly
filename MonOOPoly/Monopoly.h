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
	size_t currentPlayerIndex = 0;
	Player* currentPlayer = nullptr;

	//for trade logic
	int moneyOffered;
	int moneyRequested;
	Player* tradePartner;

	void copyFrom(const Monopoly& other);
	void moveFrom(Monopoly&& other) noexcept;
	void free();

	void addPlayerWithTokenSelection();
public:
	Monopoly();
	~Monopoly() noexcept;
	Monopoly(const Monopoly& other);
	Monopoly& operator=(const Monopoly& other);
	Monopoly(Monopoly&& other) noexcept;
	Monopoly& operator=(Monopoly&& other) noexcept;

	Board& getGameBoard();
	CardDeck& getCardDeck();

	void startGame();
	void endGame();
	void nextTurn();

	bool isGameOver() const;
	void announceWinner() const;

	bool executeCommand(GameCommand* command);
	void undoLastCommand();

	void handleJailTurn();
	void handleMovement();
	void handleCurrentField();
	void handlePropertyLanding(Property* property);
	void handlePlayerOptions();
	void handleBuildingOptions();
	void handleTradeOptions();
	void handleSellProperty();
	void handleTradeMoneyOptions();
	void handlePendingTrades();

	void advanceToNextPlayer();

	int showJailOptions();
	bool showBuyPropertyPrompt(Property& property);
	int showPlayerOptions();
	void showMessage(const std::string& message);
	int showTradeSelectionMenu();
	int selectBuildingType(Property* property);

	Vector<Property*> getPlayerProperties(Player& player);
	Vector<Property*> getBuildableProperties(Player& player);
	Vector<Player*>& getPlayers() { return players; }
	const Vector<Player*>& getPlayers() const { return players; }

	Property* selectProperty(const Vector<Property*>& properties);
	Player* selectTradePartner();

	bool validateTrade(const Vector<Property*>& offer, const Vector<Property*>& request);
	void displayTrade(const Trade* trade);

	void initializePlayers();

};

