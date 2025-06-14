#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Property.h"
#include "Bank.h"
#include "Cottage.h"

class BuildCottageCommand : public GameCommand {
    Player* player;
    Property& property;
    bool built;

public:
    BuildCottageCommand(Player* player, Property& property);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    bool wasBuilt() const;
};