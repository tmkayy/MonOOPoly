#include "CardCommands.h"
#include "Board.h"
#include "Monopoly.h"
#include "PassGoCommand.h"
#include "GoToJailCommand.h"
#include "PayTaxCommand.h"

DrawCardCommand::DrawCardCommand(Player& player, CardDeck& deck, const Vector<Player*>& allPlayers)
    : player(player), deck(deck), allPlayers(allPlayers), drawnCard(nullptr) {
}

DrawCardCommand::~DrawCardCommand() {
    delete drawnCard;
}

void DrawCardCommand::execute() {
    Monopoly* game = player.getGame();
    if (!game) return;
    Board& board = game->getGameBoard();

    drawnCard = CardField::drawCard(player, deck, allPlayers);

    if (drawnCard && drawnCard->isMovementCard()) {
        handleFieldAfterMove(player, board, deck, allPlayers);
    }
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

void DrawCardCommand::handleFieldAfterMove(Player& player, Board& board, CardDeck& deck, const Vector<Player*>& allPlayers) {
    Field* field = board.getBoard()[player.getId()];

    if (!field) {
        std::cout << "Error: Player is on an invalid field." << std::endl;
        return;
    }

    if (dynamic_cast<Jail*>(field)) {
        return;
    }

    // You may need to get the Monopoly instance if you need to call its methods
    Monopoly* game = player.getGame();
    if (!game) return;

    if (SpecialField* specialField = dynamic_cast<SpecialField*>(field)) {
        switch (specialField->getType()) {
        case SpecialField::Type::GO:
            game->executeCommand(new PassGoCommand(player, true));
            std::cout << "Passed GO! Collected $" << goMoney << std::endl;
            break;
        case SpecialField::Type::FREE_PARKING:
            std::cout << "Free Parking! No action taken.\n";
            break;
        case SpecialField::Type::GO_TO_JAIL:
            std::cout << "Go to Jail! Moving to jail...\n";
            game->executeCommand(new GoToJailCommand(player));
            break;
        case SpecialField::Type::INCOME_TAX:
        case SpecialField::Type::LUXURY_TAX:
            std::cout << "Landed on " << specialField->getName() << ". Paying tax of $"
                << specialField->getValue() << ".\n";
            game->executeCommand(new PayTaxCommand(player, specialField->getValue()));
            break;
        default:
            break;
        }
        return;
    }

    if (Property* property = dynamic_cast<Property*>(field)) {
        game->handlePropertyLanding(property);
    }
    else if (CardField* cardField = dynamic_cast<CardField*>(field)) {
        std::cout << "Landed on card field: " << "\n";
        game->executeCommand(new DrawCardCommand(player, deck, allPlayers));
    }
}