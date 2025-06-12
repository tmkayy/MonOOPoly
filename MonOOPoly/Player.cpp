#include "Player.h"
#include "Bank.h"
#include "Board.h"
#include "Cottage.h"

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

    for (size_t i = 0; i < static_cast<size_t>(PropertyColor::Count); i++) {
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

    for (size_t i = 0; i < static_cast<size_t>(PropertyColor::Count); i++) {
        propertyCounts[i] = other.propertyCounts[i];
        other.propertyCounts[i] = 0;
    }
    pendingTrades = std::move(other.pendingTrades);
}

Player::Player() : username(Token::Unknown), money(0), id(0), imprisoned(false), pendingTrades()
{
    // propertyCounts is already zero-initialized
}

Player::Player(size_t username, double money) : Player()
{
    setUsername(username);
    this->money = money;
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

Token Player::getUsername() const
{
	return username;
}

double Player::getMoney() const
{
	return money;
}

size_t Player::getId()
{
	return id;
}

bool Player::isImprisoned()
{
	return imprisoned;
}


Player::Player(size_t username, double money)
{
	setUsername(username);
	this->money = money;
    this->pendingTrades = Vector<Trade*>();
}

bool Player::buyProperty(Property& property)
{
    if (property.getOwner() != nullptr) {
        return false;
    }

    if (!Bank::subtractMoney(*this, property.getPriceToBuy())) {
        return false;
    }

    property.setOwner(this);
    propertyCounts[static_cast<size_t>(property.getColor())]++;
    return true;
}

bool Player::sellProperty(Property& property)
{
    if (property.getOwner() != this) {
        return false;
    }

    property.setOwner(nullptr);
    propertyCounts[static_cast<size_t>(property.getColor())]--;

    //handle mortgages
    while (property.mortgages.getSize() != 0) {
        if (dynamic_cast<Cottage*>(property.mortgages.peek())) {
            Bank::addMoney(*this, property.priceForCottage / 2);
        }
        else {
            Bank::addMoney(*this, property.priceForCastle / 2);
        }
        property.mortgages.pop_back();
    }

    return true;
}

bool Player::proposeTrade(Player& receiver,
    const Vector<Property*>& propertiesOffered,
    const Vector<Property*>& propertiesRequested,
    double moneyOffered,
    double moneyRequested)
{
    if (this == &receiver) {
        return false;
    }

    Trade* newTrade = new Trade(this, &receiver);

    //verify and add offered properties
    for (size_t i = 0; i < propertiesOffered.getSize(); i++) {
        Property* prop = propertiesOffered[i];
        if (prop == nullptr || prop->getOwner() != this) {
            delete newTrade;
            return false;
        }
        newTrade->addProposerProperty(prop);
    }

    //verify and add requested properties
    for (size_t i = 0; i < propertiesRequested.getSize(); i++) {
        Property* prop = propertiesRequested[i];
        if (prop == nullptr || prop->getOwner() != &receiver) {
            delete newTrade;
            return false;
        }
        newTrade->addReceiverProperty(prop);
    }

    newTrade->setProposerMoney(moneyOffered);
    newTrade->setReceiverMoney(moneyRequested);
    receiver.pendingTrades.push_back(newTrade);

    return true;
}

bool Player::acceptTrade(Trade& trade)
{
    if (trade.getReceiver() != this) {
        return false;
    }

    //check if trade is in pending trades
    bool found = false;
    for (size_t i = 0; i < pendingTrades.getSize(); i++) {
        if (pendingTrades[i] == &trade) {
            found = true;
            break;
        }
    }

    if (!found) {
        return false;
    }

    trade.accept();

    //remove from pending trades
    for (size_t i = 0; i < pendingTrades.getSize(); i++) {
        if (pendingTrades[i] == &trade) {
            pendingTrades.remove(i);
            break;
        }
    }

    return true;
}

void Player::rejectTrade(Trade& trade)
{
    if (trade.getReceiver() != this) {
        return;
    }

    trade.reject();

    for (size_t i = 0; i < pendingTrades.getSize(); i++) {
        if (pendingTrades[i] == &trade) {
            Trade* toDelete = pendingTrades[i];
            pendingTrades.remove(i);
            delete toDelete;
            break;
        }
    }
}

const Vector<Trade*>& Player::getPendingTrades() const {
    return pendingTrades;
}

int Player::getPropertyCount(PropertyColor color) const
{
    if (static_cast<size_t>(color) >= static_cast<size_t>(PropertyColor::Count)) {
        return -1;
    }
    return propertyCounts[static_cast<size_t>(color)];
}

int Player::getPropertiesOfColor(const MyString& colorStr) const
{
    try {
        PropertyColor color = colorStringToEnum(colorStr);
        return propertyCounts[static_cast<size_t>(color)];
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
    if (static_cast<size_t>(color) >= static_cast<size_t>(PropertyColor::Count)) {
        return;
    }
    propertyCounts[static_cast<size_t>(color)]++;
}

void Player::decrementPropertyCount(PropertyColor color)
{
    if (static_cast<size_t>(color) >= static_cast<size_t>(PropertyColor::Count)) {
        return;
    }
    propertyCounts[static_cast<size_t>(color)]--;
}