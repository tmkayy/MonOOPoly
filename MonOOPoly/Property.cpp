#include "Property.h"
#include "Consts.h"
#include "Player.h"
#include <stdexcept>
#include "Castle.h"
#include "Cottage.h"
#include "Bank.h"

void Property::free() {
    for (size_t i = 0; i < mortgages.getSize(); ++i) {
        delete mortgages[i];
    }
    mortgages.clear();
}

void Property::copyFrom(const Property& other) {
    priceToBuy = other.priceToBuy;
    priceForCottage = other.priceForCottage;
    priceForCastle = other.priceForCastle;
    priceForRent = other.priceForRent;
    owner = other.owner;
    color = other.color; // Copy color

    for (size_t i = 0; i < other.mortgages.getSize(); ++i) {
        Mortgage* mortgage = other.mortgages[i];
        mortgages.push_back(mortgage ? mortgage->clone() : nullptr);
    }
}

void Property::moveFrom(Property&& other) noexcept {
    priceToBuy = other.priceToBuy;
    other.priceToBuy = 0;

    priceForCottage = other.priceForCottage;
    other.priceForCottage = 0;

    priceForCastle = other.priceForCastle;
    other.priceForCastle = 0;

    priceForRent = other.priceForRent;
    other.priceForRent = 0;

    owner = other.owner;
    other.owner = nullptr;

    color = std::move(other.color); // Move color
    other.color = MyString();

    mortgages = std::move(other.mortgages);
    other.mortgages.clear();
}

Property::Property()
    : priceToBuy(0), priceForCottage(0), priceForCastle(0),
    priceForRent(0), owner(nullptr), mortgages(), color() {
}

Property::Property(double buyPrice, double cottagePrice, double castlePrice,
    double rentPrice, const MyString& color, Player* owner)
    : color(color)
{
    setPriceToBuy(buyPrice);
    setPriceForCottage(cottagePrice);
    setPriceForCastle(castlePrice);
    setPriceForRent(rentPrice);
    setOwner(owner);
}

Property::~Property() {
    free();
}

Property::Property(const Property& other) {
    copyFrom(other);
}

Property::Property(Property&& other) noexcept {
    moveFrom(std::move(other));
}

Property& Property::operator=(const Property& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

Property& Property::operator=(Property&& other) noexcept {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

double Property::getPriceToBuy() const { return priceToBuy; }
double Property::getPriceForCottage() const { return priceForCottage; }
double Property::getPriceForCastle() const { return priceForCastle; }
double Property::getPriceForRent() const { return priceForRent; }
Player* Property::getOwner() const { return owner; }
const Vector<Mortgage*>& Property::getMortgages() const { return mortgages; }
const MyString& Property::getColor() const { return color; } // Implemented getter

void Property::validatePrice(double price) {
    if (price < 0) {
        throw std::invalid_argument("Price cannot be negative");
    }
}

void Property::setPriceToBuy(double price) {
    validatePrice(price);
    priceToBuy = price;
}

void Property::setPriceForCottage(double price) {
    validatePrice(price);
    priceForCottage = price;
}

void Property::setPriceForCastle(double price) {
    validatePrice(price);
    priceForCastle = price;
}

void Property::setPriceForRent(double price) {
    validatePrice(price);
    priceForRent = price;
}

void Property::setOwner(Player* newOwner) {
    owner = newOwner;
}

void Property::setColor(const MyString& newColor) { // Implemented setter
    color = newColor;
}

bool Property::buildCottage(Player* player) {
    if (!player || owner != player) {
        return false;
    }

    if (!Bank::subtractMoney(*player, priceForCottage)) {
        return false;
    }
    Cottage* cottage = new Cottage();
    mortgages.push_back(cottage);
    cottage->increaseRent(*this);
    return true;
}

bool Property::buildCastle(Player* player) {
    if (!player || owner != player) {
        return false;
    }

    if (player->getMoney() < priceForCastle) {
        return false;
    }

    Castle* castle = new Castle();
    mortgages.push_back(castle);
    castle->increaseRent(*this);
    return true;
}

Field* Property::clone() const {
    return new Property(*this);
}