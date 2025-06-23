#pragma once
#include "GameCommand.h"
#include "CardDeck.h"
#include "Player.h"
#include "Card.h"
#include "Board.h"

class DrawCardCommand : public GameCommand {
    Player& player;
    CardDeck& deck;
    const Vector<Player*>& allPlayers;
    Card* drawnCard;

public:
    DrawCardCommand(Player& player, CardDeck& deck, const Vector<Player*>& allPlayers);
    ~DrawCardCommand();
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    void handleFieldAfterMove(Player& player, Board& board, CardDeck& deck, const Vector<Player*>& allPlayers);
    const char* getDescription() const override {
        return "Draw Card";
    }
};

