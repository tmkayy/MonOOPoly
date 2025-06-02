#include "Player.h"
#include "Bank.h"
#include "Board.h"

void Player::setUsername(size_t n)
{
	if (n > maxTokens)
		throw std::invalid_argument("invalid name");
	username = (Token)n;
}

void Player::setImprisoned(bool status)
{
	imprisoned = status;
}

Token Player::getUsername() const
{
	return username;
}

double Player::getMoney() const
{
	return money;
}

size_t Player::getId()
{
	return id;
}

bool Player::isImprisoned()
{
	return imprisoned;
}


Player::Player(size_t username, double money)
{
	setUsername(username);
	this->money = money;
}

bool Player::buyProperty(Property& property)
{
	if (property.getOwner() != nullptr) {
		return false;
	}

	if (!Bank::subtractMoney(*this, property.getPriceToBuy())) {
		return false;
	}

	property.setOwner(this);

	const MyString& color = property.getColor();
	if (color == "Brown") brownProperties++;
	else if (color == "Light Blue") lightBlueProperties++;
	else if (color == "Pink") pinkProperties++;
	else if (color == "Orange") orangeProperties++;
	else if (color == "Red") redProperties++;
	else if (color == "Yellow") yellowProperties++;
	else if (color == "Green") greenProperties++;
	else if (color == "Blue") blueProperties++;
	else if (color == "Railroad") railroadProperties++;
	else if (color == "Utility") utilityProperties++;

	return true;
}

bool Player::sellProperty(Property& property)
{
	//trade
}

int Player::getPropertiesOfColor(const MyString& color) const
{
	if (color == "Brown") return brownProperties;
	if (color == "Light Blue") return lightBlueProperties;
	if (color == "Pink") return pinkProperties;
	if (color == "Orange") return orangeProperties;
	if (color == "Red") return redProperties;
	if (color == "Yellow") return yellowProperties;
	if (color == "Green") return greenProperties;
	if (color == "Blue") return blueProperties;
	if (color == "Railroad") return railroadProperties;
	if (color == "Utility") return utilityProperties;
	return -1;
}