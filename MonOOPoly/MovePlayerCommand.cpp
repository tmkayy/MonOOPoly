#include "MovePlayerCommand.h"
#include "Monopoly.h"
#include "GoToJailCommand.h"

MovePlayerCommand::MovePlayerCommand(Player& player, Board& board, Dice& dice1, Dice& dice2)
    : player(player), board(board), dice1(dice1), dice2(dice2),
    previousPosition(player.getId()), rolledDoubles(false) {
}

void MovePlayerCommand::execute() {
	Monopoly* game = player.getGame();
    if(player.isImprisoned()) {
        std::cout << Red << player.tokenToString() << " is in jail and cannot move." << Reset << std::endl;
        return;
	}
    int roll1 = dice1.roll();
    int roll2 = dice2.roll();
    rolledDoubles = (roll1 == roll2);

    if (rolledDoubles) {
        player.setPairsThrown(player.getPairsThrown() + 1);
    }
    else {
        player.setPairsThrown(0);
    }

    if (player.getPairsThrown() == 3) {
        std::cout << Red << "Three consecutive doubles! " << player.tokenToString() << " goes to jail." << Reset << std::endl;;
        if (game) {
            game->executeCommand(new GoToJailCommand(player));
        }
        player.setPairsThrown(0);
        return;
    }

    board.setId(player, player.getId() + roll1 + roll2);
	game->handleCurrentField();
    if(rolledDoubles){
        std::cout << Green << player.tokenToString() << " rolled doubles: " << roll1 << " and " << roll2 << Reset << std::endl;
		execute(); // get another turn
    }
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