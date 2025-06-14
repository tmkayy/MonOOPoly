#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Board.h"
#include "Dice.h"

class MovePlayerCommand : public GameCommand {
    Player& player;
    Board& board;
    Dice& dice1;
    Dice& dice2;
    int previousPosition;
    bool rolledDoubles;

public:
    MovePlayerCommand(Player& player, Board& board, Dice& dice1, Dice& dice2);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
};