#pragma once
#include "Field.h"
#include "Mortgage.h"
#include "Vector.hpp"
#include <stdexcept>
#include "MyString.h"

class Player;

class Property : public Field
{
    double priceToBuy;
    double priceForCottage;
    double priceForCastle;
    double priceForRent;
    Player* owner = nullptr;
    Vector<Mortgage*> mortgages;
    MyString color; 

    void copyFrom(const Property& other);
    void moveFrom(Property&& other) noexcept;
    void free();
    void validatePrice(double price);

public:
    Property();
    Property(double buyPrice, double cottagePrice, double castlePrice,
        double rentPrice, const MyString& color, Player* owner = nullptr);

    ~Property();
    Property(const Property& other);
    Property(Property&& other) noexcept;
    Property& operator=(const Property& other);
    Property& operator=(Property&& other) noexcept;

    double getPriceToBuy() const;
    double getPriceForCottage() const;
    double getPriceForCastle() const;
    double getPriceForRent() const;
    Player* getOwner() const;
    const Vector<Mortgage*>& getMortgages() const;
    const MyString& getColor() const;

    void setPriceToBuy(double price);
    void setPriceForCottage(double price);
    void setPriceForCastle(double price);
    void setPriceForRent(double price);
    void setOwner(Player* newOwner);
    void setColor(const MyString& newColor); 

    bool buildCottage(Player* player);
    bool buildCastle(Player* player);
    Field* clone() const override;
};