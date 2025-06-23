#include "AddPlayerCommand.h"
#include "Monopoly.h"

AddPlayerCommand::AddPlayerCommand(Monopoly* game, Token token)
    : game(game), token(token), addedPlayer(nullptr) {
}

void AddPlayerCommand::execute() {
    addedPlayer = new Player(game, (size_t)(token), startMoney);
    addedPlayer->setImprisoned(false);
    addedPlayer->setTurnsInJail(0);
    addedPlayer->setPairsThrown(0);

    game->getPlayers().push_back(addedPlayer);
}

void AddPlayerCommand::undo() {
    if (addedPlayer) {
        for (size_t i = 0; i < game->getPlayers().getSize(); i++) {
            if (game->getPlayers()[i] == addedPlayer) {
                game->getPlayers().remove(i);
                break;
            }
        }
        delete addedPlayer;
        addedPlayer = nullptr;
    }
}

GameCommand* AddPlayerCommand::clone() const {
    return new AddPlayerCommand(*this);
}

