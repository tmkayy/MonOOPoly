#pragma once
#include "GameCommand.h"
#include "Player.h"

class PayJailFineCommand : public GameCommand {
    Player& player;
    bool paid;
    bool wasInJail;

public:
    PayJailFineCommand(Player& player);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override { return "Pay jail fine"; }
};