#pragma once
#include "GameCommand.h"
#include "CardDeck.h"
#include "Player.h"
#include "Card.h"

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
};

