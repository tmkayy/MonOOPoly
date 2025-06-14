#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Property.h"

class PayRentCommand : public GameCommand {
    Player& payer;
    Player& receiver;
    Property& property;
    double amountPaid;

public:
    PayRentCommand(Player& payer, Property& property);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
};

