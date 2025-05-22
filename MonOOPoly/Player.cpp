#include "Player.h"

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


