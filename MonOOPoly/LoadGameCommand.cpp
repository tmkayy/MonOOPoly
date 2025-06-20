#include "LoadGameCommand.h"
#include "Monopoly.h"
#include "Player.h"
#include "Property.h"
#include "Cottage.h"
#include "Castle.h"
#include "PaymentCard.h"
#include "GroupPaymentCard.h"
#include "MovePositionCard.h"
#include "SpecialField.h"
#include "CardField.h"
#include "Jail.h"
#include <fstream>
#include <vector>

LoadGameCommand::LoadGameCommand(Monopoly* game)
    : game(game) {
}

void LoadGameCommand::execute() {
    if (!game) return;

    // load players
    std::ifstream playersIn("players.bin", std::ios::binary);
    size_t playerCount = 0;
    playersIn.read(reinterpret_cast<char*>(&playerCount), sizeof(playerCount));
    std::vector<Player*> loadedPlayers;
    for (size_t i = 0; i < playerCount; ++i) {
        size_t username;
        double money;
        size_t id, turnsInJail, pairsThrown;
        bool imprisoned;
        playersIn.read(reinterpret_cast<char*>(&username), sizeof(username));
        playersIn.read(reinterpret_cast<char*>(&money), sizeof(money));
        playersIn.read(reinterpret_cast<char*>(&id), sizeof(id));
        playersIn.read(reinterpret_cast<char*>(&turnsInJail), sizeof(turnsInJail));
        playersIn.read(reinterpret_cast<char*>(&pairsThrown), sizeof(pairsThrown));
        playersIn.read(reinterpret_cast<char*>(&imprisoned), sizeof(imprisoned));
        Player* player = new Player(game, username, money);
        player->setTurnsInJail(turnsInJail);
        player->setPairsThrown(pairsThrown);
        player->setImprisoned(imprisoned);
        loadedPlayers.push_back(player);
    }
    playersIn.close();
    Vector<Player*>& players = game->getPlayers();
    players.clear();
    for (size_t i = 0; i < loadedPlayers.size(); ++i) {
        players.push_back(loadedPlayers[i]);
    }

    // load properties
    std::ifstream propsIn("properties.bin", std::ios::binary);
    size_t propCount = 0;
    propsIn.read(reinterpret_cast<char*>(&propCount), sizeof(propCount));
    std::vector<Property*> loadedProps;
    for (size_t i = 0; i < propCount; ++i) {
        size_t nameLen;
        propsIn.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        std::string name(nameLen, '\0');
        propsIn.read(&name[0], nameLen);
        double buyPrice, cottagePrice, castlePrice, rentPrice;
        PropertyColor color;
        propsIn.read(reinterpret_cast<char*>(&buyPrice), sizeof(buyPrice));
        propsIn.read(reinterpret_cast<char*>(&cottagePrice), sizeof(cottagePrice));
        propsIn.read(reinterpret_cast<char*>(&castlePrice), sizeof(castlePrice));
        propsIn.read(reinterpret_cast<char*>(&rentPrice), sizeof(rentPrice));
        propsIn.read(reinterpret_cast<char*>(&color), sizeof(color));
        int ownerId;
        propsIn.read(reinterpret_cast<char*>(&ownerId), sizeof(ownerId));
        Property* prop = new Property(name.c_str(), buyPrice, cottagePrice, castlePrice, rentPrice, color, ownerId >= 0 ? loadedPlayers[ownerId] : nullptr);

        // mortgages
        size_t mortgageCount;
        propsIn.read(reinterpret_cast<char*>(&mortgageCount), sizeof(mortgageCount));
        for (size_t m = 0; m < mortgageCount; ++m) {
            int mortgageType;
            propsIn.read(reinterpret_cast<char*>(&mortgageType), sizeof(mortgageType));
            if (mortgageType == 0) prop->getMortgages().push_back(new Cottage());
            else if (mortgageType == 1) prop->getMortgages().push_back(new Castle());
        }
        loadedProps.push_back(prop);
    }
    propsIn.close();
    Vector<Property*>& properties = game->getGameBoard().getProperties();
    properties.clear();
    for (size_t i = 0; i < loadedProps.size(); ++i) {
        properties.push_back(loadedProps[i]);
    }

    // load board
    std::ifstream boardIn("board.bin", std::ios::binary);
    size_t fieldCount = 0;
    boardIn.read(reinterpret_cast<char*>(&fieldCount), sizeof(fieldCount));
    Vector<Field*>& boardFields = game->getGameBoard().getBoard();
    boardFields.clear();
    for (size_t i = 0; i < fieldCount; ++i) {
        int type;
        boardIn.read(reinterpret_cast<char*>(&type), sizeof(type));
        if (type == 1) {
            int propIndex;
            boardIn.read(reinterpret_cast<char*>(&propIndex), sizeof(propIndex));
            boardFields.push_back(loadedProps[propIndex]);
        }
        else if (type == 0) {
            int sfTypeInt;
            boardIn.read(reinterpret_cast<char*>(&sfTypeInt), sizeof(sfTypeInt));
            SpecialField::Type sfType = static_cast<SpecialField::Type>(sfTypeInt);
            size_t nameLen;
            boardIn.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
            std::string name(nameLen, '\0');
            boardIn.read(&name[0], nameLen);
            double value;
            boardIn.read(reinterpret_cast<char*>(&value), sizeof(value));
            boardFields.push_back(new SpecialField(sfType, name.c_str(), value));
        }
        else if (type == 2) {
            boardFields.push_back(new CardField());
        }
        else if (type == 3) {
            boardFields.push_back(new Jail());
        }
    }
    boardIn.close();

    // load deck 
    std::ifstream deckIn("deck.bin", std::ios::binary);
    size_t cardCount = 0;
    deckIn.read(reinterpret_cast<char*>(&cardCount), sizeof(cardCount));
    Stack<Card*> loadedCards;
    for (size_t i = 0; i < cardCount; ++i) {
        int type;
        double value;
        deckIn.read(reinterpret_cast<char*>(&type), sizeof(type));
        deckIn.read(reinterpret_cast<char*>(&value), sizeof(value));
        if (type == 0) loadedCards.push(new PaymentCard(value));
        else if (type == 1) loadedCards.push(new GroupPaymentCard(value));
        else if (type == 2) loadedCards.push(new MovePositionCard(static_cast<int>(value)));
    }
    CardDeck& deck = game->getCardDeck();
    while (deck.getCards().getSize() > 0) {
        Card* card = deck.drawCard();
        delete card;
    }
    Stack<Card*> temp;
    while (loadedCards.getSize() > 0) {
        temp.push(loadedCards.top());
        loadedCards.pop();
    }
    while (temp.getSize() > 0) {
        deck.returnCard(temp.top());
        temp.pop();
    }
    deckIn.close();
}
void LoadGameCommand::undo() {
    //not implemented lol 
}

GameCommand* LoadGameCommand::clone() const {
    return new LoadGameCommand(game);
}