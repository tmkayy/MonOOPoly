#pragma once

const short maxTokens = 7;
const double cottageRent = 1.15;
const double castleRent = 1.50;

const short cardCount = 48;
const short fieldCount = 40;

const short bailCost = 50;
const short maxTurnsJail = 3;
const short jailPos = 10;

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

enum class PropertyColor {
    Brown,
    LightBlue,
    Pink,
    Orange,
    Red,
    Yellow,
    Green,
    Blue,
    Railroad,
    Utility,
    Unknown,
    Count
};