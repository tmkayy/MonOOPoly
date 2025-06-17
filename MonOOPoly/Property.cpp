#include "Property.h"
#include "Player.h"


void Property::free() {
    for (size_t i = 0; i < mortgages.getSize(); ++i) {
        delete mortgages[i];
    }
    mortgages.clear();
}

void Property::copyFrom(const Property& other) {
    name = other.name;
    priceToBuy = other.priceToBuy;
    priceForCottage = other.priceForCottage;
    priceForCastle = other.priceForCastle;
    priceForRent = other.priceForRent;
    owner = other.owner;
    color = other.color;

    for (size_t i = 0; i < other.mortgages.getSize(); ++i) {
        Mortgage* mortgage = other.mortgages[i];
        mortgages.push_back(mortgage ? mortgage->clone() : nullptr);
    }
}

void Property::moveFrom(Property&& other) noexcept {
    name = std::move(other.name);
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

    color = other.color;
    other.color = PropertyColor::Unknown;

    mortgages = std::move(other.mortgages);
    other.mortgages.clear();
}

void Property::onLand(Player& player)
{
    if (owner != nullptr && owner != &player) {
        Bank::transferMoney(player, *owner, priceForRent);
    }
}

Property::Property()
    : name(""), priceToBuy(0), priceForCottage(0), priceForCastle(0),
    priceForRent(0), owner(nullptr), mortgages(), color(PropertyColor::Unknown) {
}

Property::Property(const MyString& name, double buyPrice, double cottagePrice, double castlePrice,
    double rentPrice, PropertyColor color, Player* owner)
    : name(name), color(color)
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

const MyString& Property::getName() const { return name; }
double Property::getPriceToBuy() const { return priceToBuy; }
double Property::getPriceForCottage() const { return priceForCottage; }
double Property::getPriceForCastle() const { return priceForCastle; }
double Property::getPriceForRent() const { return priceForRent; }
Player* Property::getOwner() const { return owner; }
const Vector<Mortgage*>& Property::getMortgages() const { return mortgages; }
Vector<Mortgage*>& Property::getMortgages() { return mortgages; }

PropertyColor Property::getColor() const {
    return color;
}

void Property::setName(const MyString& newName) {
    name = newName;
}

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

void Property::setColor(PropertyColor newColor) {
    color = newColor;
}

bool Property::hasMortgages() const
{
    return !mortgages.isEmpty();
}

bool Property::buildCottage(Player* player) {
    BuildCottageCommand cmd(player, *this);
    cmd.execute();
    return cmd.wasBuilt();
}

bool Property::buildCastle(Player* player) {
    BuildCastleCommand cmd(player, *this);
    cmd.execute();
    return cmd.wasBuilt();
}

Field* Property::clone() const {
    return new Property(*this);
}