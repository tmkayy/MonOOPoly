#pragma once
#include "Consts.h"
#include <stdexcept>
#include "Property.h"
class Player
{
	Token username;
	double money;
	size_t id;
	bool imprisoned;

	int brownProperties = 0;
	int lightBlueProperties = 0;
	int pinkProperties = 0;
	int orangeProperties = 0;
	int redProperties = 0;
	int yellowProperties = 0;
	int greenProperties = 0;
	int blueProperties = 0;
	int railroadProperties = 0;
	int utilityProperties = 0;

	friend class Bank;
	friend class Board;

	void setUsername(size_t n);
public:
	void setImprisoned(bool status);
	
	Token getUsername() const;
	double getMoney() const;
	size_t getId();
	bool isImprisoned();

	Player() : username(Token::Unknown), money(0), id(0), imprisoned(false){}
	Player(size_t username, double money);

	bool buyProperty(Property& property);
	bool sellProperty(Property& property);//trade

	int getBrownProperties() const { return brownProperties; }
	int getLightBlueProperties() const { return lightBlueProperties; }
	int getPinkProperties() const { return pinkProperties; }
	int getOrangeProperties() const { return orangeProperties; }
	int getRedProperties() const { return redProperties; }
	int getYellowProperties() const { return yellowProperties; }
	int getGreenProperties() const { return greenProperties; }
	int getBlueProperties() const { return blueProperties; }
	int getRailroadProperties() const { return railroadProperties; }
	int getUtilityProperties() const { return utilityProperties; }

	int getPropertiesOfColor(const MyString& color) const;
};

