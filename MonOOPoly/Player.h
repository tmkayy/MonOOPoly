#pragma once
#include "Consts.h"
#include <stdexcept>
#include "Property.h"
#include "Trade.h"
class Player
{
	Token username;
	double money;
	size_t id;
	bool imprisoned;
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
	Player();
	Player(size_t username, double money);
	~Player();
	Player(const Player& other);
	Player(Player&& other) noexcept;
	Player& operator=(const Player& other);
	Player& operator=(Player&& other) noexcept;

	void setImprisoned(bool status);
	
	Token getUsername() const;
	double getMoney() const;
	size_t getId();
	bool isImprisoned();
	Player(size_t username, double money);

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
};

