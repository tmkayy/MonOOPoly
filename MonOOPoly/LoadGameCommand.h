#pragma once
#include "GameCommand.h"
#include "Vector.hpp"
#include "Player.h"
#include "Property.h"


class Monopoly;

class LoadGameCommand : public GameCommand {
    Monopoly* game;
    void loadPlayers(Vector<Player*>& loadedPlayers);
    void loadProperties(const Vector<Player*>& loadedPlayers, Vector<Property*>& loadedProps);
    void loadBoard(const Vector<Property*>& loadedProps);
    void loadDeck();
    void loadPendingTrades(const Vector<Player*>& loadedPlayers, const Vector<Property*>& loadedProps);
public:
    LoadGameCommand(Monopoly* game);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override {
        return "Load Game";
    }
};