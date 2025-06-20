#pragma once
#include "GameCommand.h"

class Monopoly;

class SaveGameCommand : public GameCommand
{
    Monopoly* game;
public:
    SaveGameCommand(Monopoly* game);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
};