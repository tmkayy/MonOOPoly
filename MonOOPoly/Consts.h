#pragma once

const short maxTokens = 7;
const double cottageRent = 1.15;
const double castleRent = 1.50;

const short cardCount = 48;
const short fieldCount = 40;

enum class Token {
	RaceCar,
	Thimble,
	ScottishTerrier,
	SackOfMoney,
	Cat,
	Penguin,
	RubberDuck,
	Unknown
};

static constexpr const char* Tokens[] = {
		"RaceCar", "RaceCar", "Thimble", "ScottishTerrier",
		"SackOfMoney", "Cat", "Penguin", "RubberDuck", "Unknown"
};