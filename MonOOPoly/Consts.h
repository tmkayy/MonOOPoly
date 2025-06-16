#pragma once
#include "MyString.h"

const short maxTokens = 7;
const double cottageRent = 1.15;
const double castleRent = 1.50;

const short cardCount = 48;
const short fieldCount = 40;

const short bailCost = 50;
const short maxTurnsJail = 3;
const short jailPos = 10;

const short goMoney = 200;

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
		"RaceCar", "Thimble", "ScottishTerrier",
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

static constexpr const char* PropertyColorStrings[] = {
	"Brown",
	"Light Blue",
	"Pink",
	"Orange",
	"Red",
	"Yellow",
	"Green",
	"Blue",
	"Railroad",
	"Utility",
	"Unknown"
};

inline const char* colorToString(PropertyColor color) {
	const size_t index = (size_t)(color);
	return (index < (size_t)(PropertyColor::Count)) ? PropertyColorStrings[index] : "Unknown";
}

inline PropertyColor stringToColor(const MyString& str) {
	for (size_t i = 0; i < (size_t)(PropertyColor::Count); ++i) {
		if (str == PropertyColorStrings[i]) {
			return (PropertyColor)(i);
		}
	}
	return PropertyColor::Unknown;
}