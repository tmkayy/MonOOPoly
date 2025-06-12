#pragma once
#include "Vector.hpp"

class Property;
class Player;

class Trade {

    Player* proposer;
    Player* receiver;
    Vector<Property*> proposerProperties;
    int proposerMoney;
    Vector<Property*> receiverProperties;
    int receiverMoney;
    bool isAccepted;

    void execute();
    bool isValid() const;
public:
    Trade(Player* proposer, Player* receiver);

    void addProposerProperty(Property* property);
    void addReceiverProperty(Property* property);

    void setProposerMoney(int amount);
    void setReceiverMoney(int amount);

    void accept();
    void reject();

    const Vector<Property*>& getProposerProperties() const;
    const Vector<Property*>& getReceiverProperties() const;
    int getProposerMoney() const;
    int getReceiverMoney() const;
    bool getIsAccepted() const;
    Player* getProposer() const;
    Player* getReceiver() const;
};

