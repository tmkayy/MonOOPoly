#pragma once
#include "Consts.h"
#include <stdexcept>
#include "Property.h"
#include "Trade.h"
#include "SellMortgageCommand.h"

class Monopoly;

class Player
{
	Monopoly* game;
	Token username;
	double money;
	size_t id;
	size_t turnsInJail = 0;
	size_t pairsThrown = 0;
	bool imprisoned;
	Vector<PropertyColor> monopolies;
	Vector<Trade*> pendingTrades;

	int propertyCounts[static_cast<size_t>(PropertyColor::Count)]{};

	friend class Bank;
	friend class Board;

	void setUsername(size_t n);

	void copyFrom(const Player& other);
	void moveFrom(Player&& other) noexcept;
	void free();
	PropertyColor colorStringToEnum(const MyString& colorStr) const;
public:
	Player(Monopoly* game = nullptr);
	Player(Monopoly* game, size_t username, double money);
	~Player();
	Player(const Player& other);
	Player(Player&& other) noexcept;
	Player& operator=(const Player& other);
	Player& operator=(Player&& other) noexcept;

	void setImprisoned(bool status);
	void setTurnsInJail(size_t n);
	void setPairsThrown(size_t n);
	
	Token getUsername() const;
	double getMoney() const;
	size_t getTurnsInJail() const;
	size_t getId() const;
	size_t getPairsThrown() const;
	bool isImprisoned() const;

	bool buyProperty(Property& property);
	bool sellProperty(Property& property);//trade

		//trade
		bool proposeTrade(Player& receiver,
			const Vector<Property*>& propertiesOffered,
			const Vector<Property*>& propertiesRequested,
			double moneyOffered = 0,
			double moneyRequested = 0);

		bool acceptTrade(Trade& trade);
		void rejectTrade(Trade& trade);
		const Vector<Trade*>& getPendingTrades() const;

	int getPropertyCount(PropertyColor color) const;
	int getPropertiesOfColor(const MyString& color) const;
	void incrementPropertyCount(PropertyColor color);
	void decrementPropertyCount(PropertyColor color);

	MyString tokenToString() const;

	bool hasMonopoly(PropertyColor color) const;
};

