#include "Player.h"
#include "Bank.h"

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

void Player::setId(int n)
{
	id = (n % fieldCount + fieldCount) % fieldCount;
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
	return true;
}

bool Player::sellProperty(Property& property)
{
	//trade
}


