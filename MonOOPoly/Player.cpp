#include "Player.h"
#include "Bank.h"
#include "Board.h"
#include "Cottage.h"
#include "Monopoly.h"
#include "ProposeTradeCommand.h"
#include "AcceptTradeCommand.h"
#include "RejectTradeCommand.h"

void Player::free()
{
	for (size_t i = 0; i < pendingTrades.getSize(); i++) {
		delete pendingTrades[i];
	}
	pendingTrades.clear();
}

void Player::copyFrom(const Player& other)
{
	username = other.username;
	money = other.money;
	id = other.id;
	imprisoned = other.imprisoned;
	this->game = other.game;

	for (size_t i = 0; i < (size_t)(PropertyColor::Count); i++) {
		propertyCounts[i] = other.propertyCounts[i];
	}

	for (size_t i = 0; i < other.pendingTrades.getSize(); i++) {
		pendingTrades.push_back(new Trade(*other.pendingTrades[i]));
	}
}

void Player::moveFrom(Player&& other) noexcept
{
	username = other.username;
	money = other.money;
	id = other.id;
	imprisoned = other.imprisoned;
	this->game = other.game;

	for (size_t i = 0; i < (size_t)(PropertyColor::Count); i++) {
		propertyCounts[i] = other.propertyCounts[i];
		other.propertyCounts[i] = 0;
	}
	pendingTrades = std::move(other.pendingTrades);
	other.game = nullptr;
}

Player::Player(Monopoly* game)
	: game(game), username(Token::Unknown), money(0), id(0), imprisoned(false), pendingTrades()
{
	// propertyCounts is already zero-initialized
}

Player::Player(Monopoly* game, size_t username, double money)
	: Player(game)  //delegate to the other constructor
{
	setUsername(username);
	this->money = money;
	this->pendingTrades = Vector<Trade*>();
}

Player::~Player()
{
	free();
}

Player::Player(const Player& other)
{
	copyFrom(other);
}

Player::Player(Player&& other) noexcept
{
	moveFrom(std::move(other));
}

Player& Player::operator=(const Player& other)
{
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

Player& Player::operator=(Player&& other) noexcept
{
	if (this != &other) {
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

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

void Player::setTurnsInJail(size_t n)
{
	turnsInJail = n;
}

void Player::setPairsThrown(size_t n)
{
	pairsThrown = n;
}

Token Player::getUsername() const
{
	return username;
}

double Player::getMoney() const
{
	return money;
}

size_t Player::getTurnsInJail() const
{
	return turnsInJail;
}

size_t Player::getId() const
{
	return id;
}

Monopoly* Player::getGame() const
{
	return game;
}

size_t Player::getPairsThrown() const
{
	return pairsThrown;
}

bool Player::isImprisoned() const
{
	return imprisoned;
}

bool Player::isBankrupt() const
{
	return money == 0;
}

bool Player::buyProperty(Property& property) {
	if (property.getOwner() != nullptr) {
		std::cout << tokenToString() << " cannot buy "
			<< property.getName() << " because it is already owned!" << std::endl;
		return false;
	}
	if (!Bank::subtractMoney(*this, property.getPriceToBuy())) {
		std::cout << tokenToString() << " cannot afford to buy "
			<< property.getName() << "!" << std::endl;
		return false;
	}

	property.setOwner(this);
	incrementPropertyCount(property.getColor());

	if (hasMonopoly(property.getColor())) {
		std::cout << tokenToString() << " gained a monopoly on "
			<< colorToString(property.getColor()) << " properties!" << std::endl;
	}

	return true;
}

bool Player::sellProperty(Property& property) {
	if (!game || property.getOwner() != this)
		return false;

	bool wasMonopoly = hasMonopoly(property.getColor());
	double totalRefund = property.getPriceToBuy() / 2;

	//sell all mortgages using command
	while (!property.getMortgages().isEmpty()) {
		SellMortgageCommand* cmd = new SellMortgageCommand(this, property);
		cmd->execute();
		totalRefund += (cmd->wasCottage ?
			property.getPriceForCottage() :
			property.getPriceForCastle()) / 2;

		game->executeCommand(cmd);
	}

	Bank::addMoney(*this, totalRefund);
	property.setOwner(nullptr);
	decrementPropertyCount(property.getColor());

	if (wasMonopoly && !hasMonopoly(property.getColor())) {
		std::cout << tokenToString() << " lost monopoly on "
			<< colorToString(property.getColor()) << " properties!" << std::endl;
	}

	return true;
}

bool Player::proposeTrade(Player& receiver,
	const Vector<Property*>& propertiesOffered,
	const Vector<Property*>& propertiesRequested,
	double moneyOffered,
	double moneyRequested)
{
	ProposeTradeCommand* cmd = new ProposeTradeCommand(*this, receiver,
		propertiesOffered, propertiesRequested,
		moneyOffered, moneyRequested);
	return game->executeCommand(cmd);
}

bool Player::acceptTrade(Trade& trade)
{
	AcceptTradeCommand* cmd = new AcceptTradeCommand(trade);
	return game->executeCommand(cmd);
}

void Player::rejectTrade(Trade& trade)
{
	RejectTradeCommand* cmd = new RejectTradeCommand(trade);
	game->executeCommand(cmd);
}

const Vector<Trade*>& Player::getPendingTrades() const {
	return pendingTrades;
}

Vector<Trade*>& Player::getPendingTrades() {
	return pendingTrades;
}

int Player::getPropertyCount(PropertyColor color) const
{
	if ((size_t)(color) >= (size_t)(PropertyColor::Count)) {
		return -1;
	}
	return propertyCounts[(size_t)(color)];
}

int Player::getPropertiesOfColor(const MyString& colorStr) const
{
	try {
		PropertyColor color = colorStringToEnum(colorStr);
		return propertyCounts[(size_t)(color)];
	}
	catch (const std::invalid_argument&) {
		return -1;
	}
}
PropertyColor Player::colorStringToEnum(const MyString& colorStr) const
{
	if (colorStr == "Brown") return PropertyColor::Brown;
	if (colorStr == "Light Blue") return PropertyColor::LightBlue;
	if (colorStr == "Pink") return PropertyColor::Pink;
	if (colorStr == "Orange") return PropertyColor::Orange;
	if (colorStr == "Red") return PropertyColor::Red;
	if (colorStr == "Yellow") return PropertyColor::Yellow;
	if (colorStr == "Green") return PropertyColor::Green;
	if (colorStr == "Blue") return PropertyColor::Blue;
	if (colorStr == "Railroad") return PropertyColor::Railroad;
	if (colorStr == "Utility") return PropertyColor::Utility;
	throw std::invalid_argument("Invalid color string");
}

void Player::incrementPropertyCount(PropertyColor color)
{
	if ((size_t)(color) >= (size_t)(PropertyColor::Count)) {
		return;
	}
	propertyCounts[(size_t)(color)]++;
}

void Player::decrementPropertyCount(PropertyColor color)
{
	if ((size_t)(color) >= (size_t)(PropertyColor::Count)) {
		return;
	}
	propertyCounts[(size_t)(color)]--;
}

MyString Player::tokenToString() const
{
	return MyString(Tokens[(int)(username)]);
}

bool Player::hasMonopoly(PropertyColor color) const {
	if (color == PropertyColor::Railroad || color == PropertyColor::Utility) {
		const size_t required = (color == PropertyColor::Railroad) ? 4 : 2;
		return propertyCounts[(size_t)(color)] == required;
	}

	//standard property groups
	size_t required = [color]() {
		switch (color) {
		case PropertyColor::Brown:
		case PropertyColor::Blue: return 2;
		default: return 3;
		}
		}();

	return propertyCounts[(size_t)(color)] == required;
	return true;
}
