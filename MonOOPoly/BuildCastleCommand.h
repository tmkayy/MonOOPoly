#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Property.h"
#include "Bank.h"
#include "Castle.h"

class BuildCastleCommand : public GameCommand {
    Player* player;
    Property& property;
    bool built;

public:
    BuildCastleCommand(Player* player, Property& property);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
};
