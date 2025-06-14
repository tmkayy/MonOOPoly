#pragma once
#include "GameCommand.h"
#include "Player.h"

class RollForJailReleaseCommand : public GameCommand{
    Player& player;
    bool rolledDoubles;
    bool wasInJail;
    int previousTurns;

public:
    explicit RollForJailReleaseCommand(Player& player, bool rolledDoubles);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
};
