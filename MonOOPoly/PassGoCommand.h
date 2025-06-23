#pragma once
#include "GameCommand.h"
#include "Player.h"

class PassGoCommand : public GameCommand {
    Player& player;
    bool landed;
    bool moneyCollected;

public:
    PassGoCommand(Player& player, bool landed);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override {
        return "Pass Go";
    }
};