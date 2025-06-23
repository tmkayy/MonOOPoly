#pragma once
#include "GameCommand.h"

class Monopoly;

class SaveGameCommand : public GameCommand
{
    Monopoly* game;
    void savePlayers();
    void saveProperties();
    void saveBoard();
    void saveDeck();
    void savePendingTrades();
public:
    SaveGameCommand(Monopoly* game);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override {
        return "Save Game";
    }
};