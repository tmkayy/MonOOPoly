#include "Monopoly.h"
#include "LoadGameCommand.h"
#include <iostream>

int main() {
    Monopoly game;
    while (true) {
        std::cout << "==== Monopoly ====\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Exit\n";
        std::cout << "Select option (1-3): ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
			system("cls");
            game.initializePlayers();
            game.startGame();
            break;
        }
        else if (choice == 2) {
            system("cls");
            game.executeCommand(new LoadGameCommand(&game));
            std::cout << "Game loaded.\n";
            break;
        }
        else if (choice == 3) {
            std::cout << "Goodbye!\n";
            return 0;
        }
        else {
            std::cout << "Invalid option. Try again.\n";
        }
    }

    while (!game.isGameOver()) {
        game.nextTurn();
    }
    game.announceWinner();
    return 0;
}