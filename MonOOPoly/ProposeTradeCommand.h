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
    double moneyOffered;
    double moneyRequested;
    Trade* createdTrade = nullptr;

public:
    ProposeTradeCommand(Player& proposer, Player& receiver,
        const Vector<Property*>& propertiesOffered,
        const Vector<Property*>& propertiesRequested,
        double moneyOffered, double moneyRequested);
    ~ProposeTradeCommand();

    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override {
        return "Propose Trade";
    }
};
