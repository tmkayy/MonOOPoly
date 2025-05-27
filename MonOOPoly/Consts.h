#pragma once

const short maxTokens = 7;
const double cottageRent = 1.15;
const double castleRent = 1.50;

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