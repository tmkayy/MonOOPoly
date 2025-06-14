#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Property.h"
#include "Vector.hpp"

class ProposeTradeCommand : public GameCommand {
    Player& proposer;
    Player& receiver;
    Vector<Property*> propertiesOffered;
    Vector<Property*> propertiesRequested;
    int moneyOffered;
    int moneyRequested;
    Trade* createdTrade = nullptr;

public:
    ProposeTradeCommand(Player& proposer, Player& receiver,
        const Vector<Property*>& propertiesOffered,
        const Vector<Property*>& propertiesRequested,
        int moneyOffered, int moneyRequested);
    ~ProposeTradeCommand();

    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
};
