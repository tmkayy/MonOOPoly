#pragma once
#include "Vector.hpp"
#include "Consts.h"

class Property;
class Player;

class Trade {

    Player* proposer;
    Player* receiver;
    Vector<Property*> proposerProperties;
    Vector<Property*> receiverProperties;
    double proposerMoney;
    double receiverMoney;
    bool isAccepted;

    void execute();
public:
    bool isValid() const;
    Trade(Player* proposer, Player* receiver);

    void addProposerProperty(Property* property);
    void addReceiverProperty(Property* property);

    void setProposerMoney(double amount);
    void setReceiverMoney(double amount);

    void accept();
    void reject();

    const Vector<Property*>& getProposerProperties() const;
    const Vector<Property*>& getReceiverProperties() const;
    double getProposerMoney() const;
    double getReceiverMoney() const;
    bool getIsAccepted() const;
    Player* getProposer() const;
    Player* getReceiver() const;

    void checkMonopolyChange(PropertyColor color, Player* oldOwner, Player* newOwner);
};

