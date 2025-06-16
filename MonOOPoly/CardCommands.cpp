#include "CardCommands.h"

DrawCardCommand::DrawCardCommand(Player& player, CardDeck& deck, const Vector<Player*>& allPlayers)
    : player(player), deck(deck), allPlayers(allPlayers), drawnCard(nullptr) {
}

DrawCardCommand::~DrawCardCommand() {
    delete drawnCard;
}

void DrawCardCommand::execute() {
    drawnCard = CardField::drawCard(player, deck, allPlayers);
}

void DrawCardCommand::undo() {
    if (drawnCard) {
        drawnCard->reverseEffect(player, allPlayers);
        deck.returnCard(drawnCard);
        drawnCard = nullptr;
    }
}

GameCommand* DrawCardCommand::clone() const {
    return new DrawCardCommand(*this);
}