#pragma once
#include "GameCommand.h"
#include "Consts.h"
#include "Player.h"

class Monopoly;

class AddPlayerCommand : public GameCommand {
    Monopoly* game;
    Token token;
    Player* addedPlayer;

public:
    AddPlayerCommand(Monopoly* game, Token token);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override { return "Add Player"; }
};