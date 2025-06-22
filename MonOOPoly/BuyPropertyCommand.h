#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Property.h"

class BuyPropertyCommand : public GameCommand {
    Player& buyer;
    Property& property;
    bool purchased;

public:
    BuyPropertyCommand(Player& buyer, Property& property);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override { return "Buy Property"; }
};

