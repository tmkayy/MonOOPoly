#pragma once
#include "Field.h"
#include "Vector.hpp"
#include <stdexcept>
#include "MyString.h"
#include "Consts.h"
#include "BuildCastleCommand.h"
#include "BuildCottageCommand.h"
#include "Castle.h"
#include "Cottage.h"
#include "Bank.h"


class Mortgage;
class Property : public Field
{
    friend class Player;
    MyString name;
    double priceToBuy;
    double priceForCottage;
    double priceForCastle;
    double priceForRent;
    Player* owner = nullptr;
    Vector<Mortgage*> mortgages;
    PropertyColor color; 

    void copyFrom(const Property& other);
    void moveFrom(Property&& other) noexcept;
    void free();
    void validatePrice(double price);

public:
    Property();
     Property(const MyString& name, double buyPrice, double cottagePrice, double castlePrice,
        double rentPrice, PropertyColor color, Player* owner = nullptr);

    ~Property();
    Property(const Property& other);
    Property(Property&& other) noexcept;
    Property& operator=(const Property& other);
    Property& operator=(Property&& other) noexcept;

    const MyString& getName() const;
    double getPriceToBuy() const;
    double getPriceForCottage() const;
    double getPriceForCastle() const;
    double getPriceForRent() const;
    Player* getOwner() const;
    const Vector<Mortgage*>& getMortgages() const;
    Vector<Mortgage*>& getMortgages();
    PropertyColor getColor() const;

    void setName(const MyString& newName);
    void setPriceToBuy(double price);
    void setPriceForCottage(double price);
    void setPriceForCastle(double price);
    void setPriceForRent(double price);
    void setOwner(Player* newOwner);
    void setColor(PropertyColor newColor);

    bool hasMortgages() const;

    bool buildCottage(Player* player);
    bool buildCastle(Player* player);
    Field* clone() const override;
};