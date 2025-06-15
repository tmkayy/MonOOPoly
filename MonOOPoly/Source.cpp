#include "Monopoly.h"
#include <iostream>

int main() {
    std::cout << "=== MONOPOLY GAME ===" << std::endl;

    Monopoly game;
    game.initializePlayers();
    game.startGame();

    while (!game.isGameOver()) {
        game.nextTurn();
    }

    game.announceWinner();
    return 0;
}