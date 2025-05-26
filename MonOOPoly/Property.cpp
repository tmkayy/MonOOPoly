#include "Property.h"
#include "Consts.h"

void Property::free() {
    for (size_t i = 0; i < mortgages.getCapacity(); ++i) {
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

    for (size_t i = 0; i < other.mortgages.getSize(); ++i) {
        Mortgage* mortgage = other.mortgages[i];
        mortgages.push_back(mortgage ? new Mortgage(*mortgage) : nullptr);
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
    other.owner = Token::Unknown;

    mortgages = std::move(other.mortgages);
    other.mortgages.clear();
}

Property::Property()
    : priceToBuy(0), priceForCottage(0), priceForCastle(0),
    priceForRent(0), owner(Token::Unknown), mortgages() {
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
Token Property::getOwner() const { return owner; }

const Vector<Mortgage*>& Property::getMortgages() const { return mortgages; }

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

void Property::setOwner(Token newOwner)
{
    owner = newOwner;
}

Property::Property(double buyPrice, double cottagePrice, double castlePrice,
    double rentPrice, Token ownerToken) {
    setPriceToBuy(buyPrice);
    setPriceForCottage(cottagePrice);
    setPriceForCastle(castlePrice);
    setPriceForRent(rentPrice);
    owner = ownerToken;
}


bool Property::buildCottage() {
    // Implementation goes here
}

bool Property::buildCastle() {
    // Implementation goes here
}

Field* Property::clone() const {
    return new Property(*this);
}