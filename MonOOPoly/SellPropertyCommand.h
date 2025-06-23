#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Property.h"
class SellPropertyCommand : public GameCommand {
    Player& seller;
    Property& property;
    bool sold;

public:
    SellPropertyCommand(Player& seller, Property& property);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override {
        return "Sell Property";
    }
};
