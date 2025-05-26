#pragma once
#include "Field.h"
#include "Mortgage.h"
#include "Vector.hpp"
#include <stdexcept>
#include "MyString.h"

class Property : public Field
{
    double priceToBuy;
    double priceForCottage;
    double priceForCastle;
    double priceForRent;
    Token owner;
    Vector<Mortgage*> mortgages;

    void copyFrom(const Property& other);
    void moveFrom(Property&& other) noexcept;
    void free();
    void validatePrice(double price);

public:
    Property();
    Property(double buyPrice, double cottagePrice, double castlePrice,
        double rentPrice, Token ownerToken = Token::Unknown);

    ~Property();
    Property(const Property& other);
    Property(Property&& other) noexcept;
    Property& operator=(const Property& other);
    Property& operator=(Property&& other) noexcept;

    double getPriceToBuy() const;
    double getPriceForCottage() const;
    double getPriceForCastle() const;
    double getPriceForRent() const;
    Token getOwner() const;
    const Vector<Mortgage*>& getMortgages() const;

    void setPriceToBuy(double price);
    void setPriceForCottage(double price);
    void setPriceForCastle(double price);
    void setPriceForRent(double price);
    void setOwner(Token newOwner);

    bool buildCottage();
    bool buildCastle();
    Field* clone() const override;
};