#include "MovePlayerCommand.h"

MovePlayerCommand::MovePlayerCommand(Player& player, Board& board, Dice& dice1, Dice& dice2)
    : player(player), board(board), dice1(dice1), dice2(dice2),
    previousPosition(player.getId()), rolledDoubles(false) {
}

void MovePlayerCommand::execute() {
    int roll1 = dice1.roll();
    int roll2 = dice2.roll();
    rolledDoubles = (roll1 == roll2);
    board.setId(player, player.getId() + roll1 + roll2);
}

void MovePlayerCommand::undo() {
    board.setId(player, previousPosition);
    if (rolledDoubles) {
        player.setPairsThrown(player.getPairsThrown() - 1);
    }
}

GameCommand* MovePlayerCommand::clone() const {
    return new MovePlayerCommand(*this);
}