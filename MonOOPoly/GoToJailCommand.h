#pragma once
#include "GameCommand.h"
#include "Player.h"

class GoToJailCommand : public GameCommand {
    Player& player;
    bool wasInJail;

public:
    GoToJailCommand(Player& player);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
};