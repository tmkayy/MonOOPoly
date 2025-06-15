#pragma once
#include "GameCommand.h"
#include "Player.h"

class PayTaxCommand : public GameCommand {
    Player& player;
    double amount;
    bool wasPaid;

public:
    PayTaxCommand(Player& player, double amount);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
};