#include "Monopoly.h"
#include "LoadGameCommand.h"
#include <iostream>

int main() {
    try {
        Monopoly game;
        while (true) {
            std::cout << "==== Monopoly ====" << std::endl;
            std::cout << "1. New Game" << std::endl;
            std::cout << "2. Load Game" << std::endl;
            std::cout << "3. Exit" << std::endl;
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
                std::cout << "Game loaded." << std::endl;
                break;
            }
            else if (choice == 3) {
                std::cout << "Goodbye!" << std::endl;
                return 0;
            }
            else {
                std::cout << "Invalid option. Try again." << std::endl;
            }
        }

        while (!game.isGameOver()) {
            game.nextTurn();
        }
        game.announceWinner();
		exit(0);
    }
    catch (const std::exception& ex) {
        std::cerr << "An error occurred: " << ex.what() << std::endl;
        return -1;
    }
    return 0;
}