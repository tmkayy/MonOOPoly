#include "Board.h"
#include "Property.h"

void Board::free()
{
    for (size_t i = 0; i < board.getSize(); ++i)
        delete board[i];
    board.clear();
	//they were all stored in board, so i don't need to delete them again
    properties.clear();
}

void Board::copyFrom(const Board& other)
{
    for (size_t i = 0; i < other.board.getSize(); ++i)
        board.push_back(other.board[i]->clone());
    initializeProperties();
}

void Board::moveFrom(Board&& other) noexcept
{
    board = std::move(other.board);
    properties = std::move(other.properties);
    other.board.clear();
    other.properties.clear();
}

void Board::initializeProperties()
{
    for (size_t i = 0; i < board.getSize(); ++i)
    {
        if (Property* prop = dynamic_cast<Property*>(board[i]))
            properties.push_back(prop);
    }
}


Board::Board() {
    board = Vector<Field*>(fieldCount);

    // 0 - GO (Special Field)
    board.push_back(new SpecialField(SpecialField::Type::GO, "GO"));

    // 1 - Mediterranean Avenue (Brown)
    board.push_back(new Property("Mediterranean Avenue", 60, 25, 50, 2, PropertyColor::Brown));

    // 2 - Community Chest (Card Field)
    board.push_back(new CardField());

    // 3 - Baltic Avenue (Brown)
    board.push_back(new Property("Baltic Avenue", 60, 25, 50, 4, PropertyColor::Brown));

    // 4 - Income Tax (Special Field)
    board.push_back(new SpecialField(SpecialField::Type::INCOME_TAX, "Income Tax", 200));

    // 5 - Reading Railroad (Railroad Property)
    board.push_back(new Property("Reading Railroad", 200, 0, 0, 25, PropertyColor::Railroad));

    // 6 - Oriental Avenue (Light Blue)
    board.push_back(new Property("Oriental Avenue", 100, 25, 50, 6, PropertyColor::LightBlue));

    // 7 - Chance (Card Field)
    board.push_back(new CardField());

    // 8 - Vermont Avenue (Light Blue)
    board.push_back(new Property("Vermont Avenue", 100, 25, 50, 6, PropertyColor::LightBlue));

    // 9 - Connecticut Avenue (Light Blue)
    board.push_back(new Property("Connecticut Avenue", 120, 25, 50, 8, PropertyColor::LightBlue));

    // 10 - Jail/Just Visiting (Special Field)
    board.push_back(new Jail());

    // 11 - St. Charles Place (Pink)
    board.push_back(new Property("St. Charles Place", 140, 50, 100, 10, PropertyColor::Pink));

    // 12 - Electric Company (Utility Property)
    board.push_back(new Property("Electric Company", 150, 0, 0, 20, PropertyColor::Utility));

    // 13 - States Avenue (Pink)
    board.push_back(new Property("States Avenue", 140, 50, 100, 10, PropertyColor::Pink));

    // 14 - Virginia Avenue (Pink)
    board.push_back(new Property("Virginia Avenue", 160, 50, 100, 12, PropertyColor::Pink));

    // 15 - Pennsylvania Railroad (Railroad Property)
    board.push_back(new Property("Pennsylvania Railroad", 200, 0, 0, 25, PropertyColor::Railroad));

    // 16 - St. James Place (Orange)
    board.push_back(new Property("St. James Place", 180, 50, 100, 14, PropertyColor::Orange));

    // 17 - Community Chest (Card Field)
    board.push_back(new CardField());

    // 18 - Tennessee Avenue (Orange)
    board.push_back(new Property("Tennessee Avenue", 180, 50, 100, 14, PropertyColor::Orange));

    // 19 - New York Avenue (Orange)
    board.push_back(new Property("New York Avenue", 200, 50, 100, 16, PropertyColor::Orange));

    // 20 - Free Parking (Special Field)
    board.push_back(new SpecialField(SpecialField::Type::FREE_PARKING, "Free Parking"));

    // 21 - Kentucky Avenue (Red)
    board.push_back(new Property("Kentucky Avenue", 220, 75, 150, 18, PropertyColor::Red));

    // 22 - Chance (Card Field)
    board.push_back(new CardField());

    // 23 - Indiana Avenue (Red)
    board.push_back(new Property("Indiana Avenue", 220, 75, 150, 18, PropertyColor::Red));

    // 24 - Illinois Avenue (Red)
    board.push_back(new Property("Illinois Avenue", 240, 75, 150, 20, PropertyColor::Red));

    // 25 - B. & O. Railroad (Railroad Property)
    board.push_back(new Property("B. & O. Railroad", 200, 0, 0, 25, PropertyColor::Railroad));

    // 26 - Atlantic Avenue (Yellow)
    board.push_back(new Property("Atlantic Avenue", 260, 75, 150, 22, PropertyColor::Yellow));

    // 27 - Ventnor Avenue (Yellow)
    board.push_back(new Property("Ventnor Avenue", 260, 75, 150, 22, PropertyColor::Yellow));

    // 28 - Water Works (Utility Property)
    board.push_back(new Property("Water Works", 150, 0, 0, 20, PropertyColor::Utility));

    // 29 - Marvin Gardens (Yellow)
    board.push_back(new Property("Marvin Gardens", 280, 75, 150, 24, PropertyColor::Yellow));

    // 30 - Go To Jail (Special Field)
    board.push_back(new SpecialField(SpecialField::Type::GO_TO_JAIL, "Go To Jail"));

    // 31 - Pacific Avenue (Green)
    board.push_back(new Property("Pacific Avenue", 300, 100, 200, 26, PropertyColor::Green));

    // 32 - North Carolina Avenue (Green)
    board.push_back(new Property("North Carolina Avenue", 300, 100, 200, 26, PropertyColor::Green));

    // 33 - Community Chest (Card Field)
    board.push_back(new CardField());

    // 34 - Pennsylvania Avenue (Green)
    board.push_back(new Property("Pennsylvania Avenue", 320, 100, 200, 28, PropertyColor::Green));

    // 35 - Short Line (Railroad Property)
    board.push_back(new Property("Short Line", 200, 0, 0, 25, PropertyColor::Railroad));

    // 36 - Chance (Card Field)
    board.push_back(new CardField());

    // 37 - Park Place (Blue)
    board.push_back(new Property("Park Place", 350, 100, 200, 35, PropertyColor::Blue));

    // 38 - Luxury Tax (Special Field)
    board.push_back(new SpecialField(SpecialField::Type::LUXURY_TAX, "Luxury Tax", 100));

    // 39 - Boardwalk (Blue)
    board.push_back(new Property("Boardwalk", 400, 100, 200, 50, PropertyColor::Blue));

    initializeProperties();
}

Board::~Board() { free(); }
Board::Board(const Board& other) { copyFrom(other); }
Board::Board(Board&& other) noexcept { moveFrom(std::move(other)); }

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

void Board::setId(Player& player, int n)
{
    player.id = (n % fieldCount + fieldCount) % fieldCount;
}

Vector<Field*>& Board::getBoard() { return board; }

Vector<Property*>& Board::getProperties() { return properties; }

Vector<Property*>& Board::getPropertiesByColor(PropertyColor color) {
    Vector<Property*> result;
    for (size_t i = 0; i < properties.getSize(); ++i) {
        if (properties[i]->getColor() == color) {
            result.push_back(properties[i]);
        }
    }
    return result;
}