#pragma once

const short maxTokens = 7;

enum class Token {
	RaceCar,
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