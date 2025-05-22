#pragma once
#include "Consts.h"
#include <stdexcept>
class Player
{
	Token username;
	double money;
	size_t id;
	bool imprisoned;

	friend class Bank;

	void setUsername(size_t n);
public:
	void setImprisoned(bool status);
	
	Token getUsername() const;
	double getMoney() const;
	size_t getId();
	bool isImprisoned();

	Player() : username(Token::Unknown), money(0), id(0), imprisoned(false){}
	Player(Token username, double money);
};

