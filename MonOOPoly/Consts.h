#pragma once
#include "MyString.h"
#include <fstream>

constexpr const char* Reset = "\033[0m";
constexpr const char* Black = "\033[30m";
constexpr const char* Red = "\033[31m";
constexpr const char* Green = "\033[32m";
constexpr const char* Yellow = "\033[33m";
constexpr const char* Blue = "\033[34m";
constexpr const char* Magenta = "\033[35m";
constexpr const char* Cyan = "\033[36m";
constexpr const char* White = "\033[37m";
constexpr const char* BrightBlack = "\033[90m";
constexpr const char* BrightRed = "\033[91m";
constexpr const char* BrightGreen = "\033[92m";
constexpr const char* BrightYellow = "\033[93m";
constexpr const char* BrightBlue = "\033[94m";
constexpr const char* BrightMagenta = "\033[95m";
constexpr const char* BrightCyan = "\033[96m";
constexpr const char* BrightWhite = "\033[97m";

const short maxTokens = 7;
const double cottageRent = 1.15;
const double castleRent = 1.50;

const short cardCount = 48;

const short startMoney = 1500;

const short fieldCount = 40;

const short bailCost = 100;
const short maxTurnsJail = 2;
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

inline MyString colorToString(PropertyColor color) {
	const size_t index = (size_t)(color);
	const char* name = (index < (size_t)(PropertyColor::Count)) ? PropertyColorStrings[index] : "Unknown";
	const char* colorCode = nullptr;

	switch (color) {
	case PropertyColor::Brown:      colorCode = BrightBlack; break;
	case PropertyColor::LightBlue:  colorCode = Cyan;        break;
	case PropertyColor::Pink:       colorCode = Magenta;     break;
	case PropertyColor::Orange:     colorCode = Yellow;      break;
	case PropertyColor::Red:        colorCode = Red;         break;
	case PropertyColor::Yellow:     colorCode = BrightYellow; break;
	case PropertyColor::Green:      colorCode = Green;       break;
	case PropertyColor::Blue:       colorCode = Blue;        break;
	case PropertyColor::Railroad:   colorCode = White;       break;
	case PropertyColor::Utility:    colorCode = BrightCyan;  break;
	default:                        colorCode = Reset;       break;
	}

	return MyString(colorCode) + name + MyString(Reset);
}

inline PropertyColor stringToColor(const MyString& str) {
	for (size_t i = 0; i < (size_t)(PropertyColor::Count); ++i) {
		if (str == PropertyColorStrings[i]) {
			return (PropertyColor)(i);
		}
	}
	return PropertyColor::Unknown;
}

inline void CheckFileOpen(const std::ifstream& file, const MyString& filename) {
	if (!file.is_open()) {
		std::cerr << Red << "Failed to open file for reading: " << filename << Reset << std::endl;
		throw std::runtime_error("Failed to open file for reading");
	}
}

inline void CheckFileOpen(const std::ofstream& file, const MyString& filename) {
	if (!file.is_open()) {
		std::cerr << Red << "Failed to open file for writing: " << filename << Reset << std::endl;
		throw std::runtime_error("Failed to open file for writing");
	}
}