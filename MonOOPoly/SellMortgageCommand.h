#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Property.h"
#include "Cottage.h"
#include "Castle.h"
#include "Bank.h"

class SellMortgageCommand : public GameCommand {
    Player* player;
    Property& property;
    bool sold;
    bool monopolyLost;

public:
    bool wasCottage;

    SellMortgageCommand(Player* player, Property& property);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    bool monopolyWasLost() const { return monopolyLost; }
    const char* getDescription() const override {
        return "Sell Mortgage";
    }
};