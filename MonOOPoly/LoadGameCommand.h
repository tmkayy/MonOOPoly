#pragma once
#include "GameCommand.h"

class Monopoly;

class LoadGameCommand : public GameCommand {
    Monopoly* game;
public:
    LoadGameCommand(Monopoly* game);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
};