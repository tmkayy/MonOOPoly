#include "Board.h"
#include "Property.h"
#include "SpecialField.h"
#include "CardField.h"

void Board::free()
{
	for (size_t i = 0; i < board.getSize(); ++i)
	{
		delete board[i];
	}
	board.clear();
}

void Board::copyFrom(const Board& other)
{
	for (size_t i = 0; i < other.board.getSize(); ++i)
	{
		board.push_back(other.board[i]->clone());
	}
}

void Board::moveFrom(Board&& other) noexcept
{
	board = std::move(other.board);
	other.board.clear();
}

Board::Board()
{
	board = Vector<Field*>(fieldCount);

	// 0 - GO
	board.push_back(new SpecialField(SpecialField::Type::GO));

	// 1 - Mediterranean Avenue
	board.push_back(new Property(60, 50, 50, 2));

	// 2 - Community Chest
	board.push_back(new CardField());

	// 3 - Baltic Avenue
	board.push_back(new Property(60, 50, 50, 4));

	// 4 - Income Tax
	board.push_back(new Property(200, -1, -1, -1));

	// 5 - Reading Railroad
	board.push_back(new Property(200, -1, -1, 25));

	// 6 - Oriental Avenue
	board.push_back(new Property(100, 50, 50, 6));

	// 7 - Chance
	board.push_back(new CardField());

	// 8 - Vermont Avenue
	board.push_back(new Property(100, 50, 50, 6));

	// 9 - Connecticut Avenue
	board.push_back(new Property(120, 50, 50, 8));

	// 10 - Jail/Just Visiting
	board.push_back(new SpecialField(SpecialField::Type::GO_TO_JAIL));

	// 11 - St. Charles Place
	board.push_back(new Property(140, 100, 100, 10));

	// 12 - Electric Company
	board.push_back(new Property(150, -1, -1, 4));

	// 13 - States Avenue
	board.push_back(new Property(140, 100, 100, 10));

	// 14 - Virginia Avenue
	board.push_back(new Property(160, 100, 100, 12));

	// 15 - Pennsylvania Railroad
	board.push_back(new Property(200, -1, -1, 25));

	// 16 - St. James Place
	board.push_back(new Property(180, 100, 100, 14));

	// 17 - Community Chest
	board.push_back(new CardField());

	// 18 - Tennessee Avenue
	board.push_back(new Property(180, 100, 100, 14));

	// 19 - New York Avenue
	board.push_back(new Property(200, 100, 100, 16));

	// 20 - Free Parking
	board.push_back(new SpecialField(SpecialField::Type::FREE_PARKING));

	// 21 - Kentucky Avenue
	board.push_back(new Property(220, 150, 150, 18));

	// 22 - Chance
	board.push_back(new CardField());

	// 23 - Indiana Avenue
	board.push_back(new Property(220, 150, 150, 18));

	// 24 - Illinois Avenue
	board.push_back(new Property(240, 150, 150, 20));

	// 25 - B. & O. Railroad
	board.push_back(new Property(200, -1, -1, 25));

	// 26 - Atlantic Avenue
	board.push_back(new Property(260, 150, 150, 22));

	// 27 - Ventnor Avenue
	board.push_back(new Property(260, 150, 150, 22));

	// 28 - Water Works
	board.push_back(new Property(150, -1, -1, 4));

	// 29 - Marvin Gardens
	board.push_back(new Property(280, 150, 150, 24));

	// 30 - Go To Jail
	board.push_back(new SpecialField(SpecialField::Type::GO_TO_JAIL));

	// 31 - Pacific Avenue
	board.push_back(new Property(300, 200, 200, 26));

	// 32 - North Carolina Avenue
	board.push_back(new Property(300, 200, 200, 26));

	// 33 - Community Chest
	board.push_back(new CardField());

	// 34 - Pennsylvania Avenue
	board.push_back(new Property(320, 200, 200, 28));

	// 35 - Short Line
	board.push_back(new Property(200, -1, -1, 25));

	// 36 - Chance
	board.push_back(new CardField());

	// 37 - Park Place
	board.push_back(new Property(350, 200, 200, 35));

	// 38 - Luxury Tax
	board.push_back(new Property(100, -1, -1, -1));

	// 39 - Boardwalk
	board.push_back(new Property(400, 200, 200, 50));
}

Board::~Board()
{
	free();
}

Board::Board(const Board& other)
{
	copyFrom(other);
}

Board::Board(Board&& other) noexcept
{
	moveFrom(std::move(other));
}

Board& Board::operator=(const Board& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

Board& Board::operator=(Board&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

Vector<Field*> Board::getBoard() const
{
	return board;
}