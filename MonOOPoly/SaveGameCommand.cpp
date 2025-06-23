#include "SaveGameCommand.h"
#include "Monopoly.h"
#include <fstream>
#include "PaymentCard.h"
#include "GroupPaymentCard.h"
#include "MovePositionCard.h"
#include "Cottage.h"
#include "Castle.h"
#include "Consts.h"

void SaveGameCommand::savePlayers()
{
    std::ofstream playersOut("players.bin", std::ios::binary);
    CheckFileOpen(playersOut, "players.bin");
    const Vector<Player*>& players = game->getPlayers();
    size_t playerCount = players.getSize();
    playersOut.write(reinterpret_cast<const char*>(&playerCount), sizeof(playerCount));
    for (size_t i = 0; i < playerCount; ++i) {
        Player* player = players[i];
        Token username = player->getUsername();
        size_t usernameValue = (size_t)(username);
        double money = player->getMoney();
        size_t id = player->getId();
        size_t turnsInJail = player->getTurnsInJail();
        size_t pairsThrown = player->getPairsThrown();
        bool imprisoned = player->isImprisoned();
        playersOut.write(reinterpret_cast<const char*>(&usernameValue), sizeof(usernameValue));
        playersOut.write(reinterpret_cast<const char*>(&money), sizeof(money));
        playersOut.write(reinterpret_cast<const char*>(&id), sizeof(id));
        playersOut.write(reinterpret_cast<const char*>(&turnsInJail), sizeof(turnsInJail));
        playersOut.write(reinterpret_cast<const char*>(&pairsThrown), sizeof(pairsThrown));
        playersOut.write(reinterpret_cast<const char*>(&imprisoned), sizeof(imprisoned));
    }
    size_t currentPlayerIndex = game->getCurrentPlayerIndex();
    playersOut.write(reinterpret_cast<const char*>(&currentPlayerIndex), sizeof(currentPlayerIndex));
    playersOut.close();
}

void SaveGameCommand::saveProperties()
{
    std::ofstream propsOut("properties.bin", std::ios::binary);
    CheckFileOpen(propsOut, "properties.bin");
    const Vector<Player*>& players = game->getPlayers();
    const Vector<Property*>& properties = game->getGameBoard().getProperties();
    size_t playerCount = players.getSize();
    size_t propCount = properties.getSize();
    propsOut.write(reinterpret_cast<const char*>(&propCount), sizeof(propCount));
    for (size_t i = 0; i < propCount; ++i) {
        Property* prop = properties[i];
        size_t nameLen = strlen(prop->getName().c_str());
        propsOut.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        propsOut.write(prop->getName().c_str(), nameLen);
        double buyPrice = prop->getPriceToBuy();
        double cottagePrice = prop->getPriceForCottage();
        double castlePrice = prop->getPriceForCastle();
        double rentPrice = prop->getPriceForRent();
        PropertyColor color = prop->getColor();
        propsOut.write(reinterpret_cast<const char*>(&buyPrice), sizeof(buyPrice));
        propsOut.write(reinterpret_cast<const char*>(&cottagePrice), sizeof(cottagePrice));
        propsOut.write(reinterpret_cast<const char*>(&castlePrice), sizeof(castlePrice));
        propsOut.write(reinterpret_cast<const char*>(&rentPrice), sizeof(rentPrice));
        propsOut.write(reinterpret_cast<const char*>(&color), sizeof(color));
        int ownerId = -1;
        for (size_t j = 0; j < playerCount; ++j) {
            if (players[j] == prop->getOwner()) {
                ownerId = (int)(j);
                break;
            }
        }
        propsOut.write(reinterpret_cast<const char*>(&ownerId), sizeof(ownerId));

        //save mortgages
        const Vector<Mortgage*>& mortgages = prop->getMortgages();
        size_t mortgageCount = mortgages.getSize();
        propsOut.write(reinterpret_cast<const char*>(&mortgageCount), sizeof(mortgageCount));
        for (size_t m = 0; m < mortgageCount; ++m) {
            int mortgageType = -1;
            if (dynamic_cast<Cottage*>(mortgages[m])) mortgageType = 0;
            else if (dynamic_cast<Castle*>(mortgages[m])) mortgageType = 1;
            propsOut.write(reinterpret_cast<const char*>(&mortgageType), sizeof(mortgageType));
        }
    }
    propsOut.close();
}

void SaveGameCommand::saveBoard()
{
    std::ofstream boardOut("board.bin", std::ios::binary);
    CheckFileOpen(boardOut, "board.bin");
    const Vector<Property*>& properties = game->getGameBoard().getProperties();
    size_t propCount = properties.getSize();
    const Vector<Field*>& boardFields = game->getGameBoard().getBoard();
    size_t fieldCount = boardFields.getSize();
    boardOut.write(reinterpret_cast<const char*>(&fieldCount), sizeof(fieldCount));
    for (size_t i = 0; i < fieldCount; ++i) {
        int type = -1;
        int propIndex = -1;
        if (dynamic_cast<Property*>(boardFields[i])) {
            type = 1;
            for (size_t j = 0; j < propCount; ++j) {
                if (properties[j] == boardFields[i]) {
                    propIndex = (int)(j);
                    break;
                }
            }
        }
        else if (dynamic_cast<SpecialField*>(boardFields[i])) {
            type = 0;
            boardOut.write(reinterpret_cast<const char*>(&type), sizeof(type));
            SpecialField* sf = (SpecialField*)(boardFields[i]);
            int sfType = (int)(sf->getType());
            boardOut.write(reinterpret_cast<const char*>(&sfType), sizeof(sfType));
            size_t nameLen = sf->getName().getSize();
            boardOut.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
            boardOut.write(sf->getName().c_str(), nameLen);
            double value = sf->getValue();
            boardOut.write(reinterpret_cast<const char*>(&value), sizeof(value));
            continue;
        }
        else if (dynamic_cast<CardField*>(boardFields[i])) {
            type = 2;
        }
        else if (dynamic_cast<Jail*>(boardFields[i])) {
            type = 3;
        }
        boardOut.write(reinterpret_cast<const char*>(&type), sizeof(type));
        if (type == 1) {
            boardOut.write(reinterpret_cast<const char*>(&propIndex), sizeof(propIndex));
        }
    }
    boardOut.close();
}

void SaveGameCommand::saveDeck()
{
    std::ofstream deckOut("deck.bin", std::ios::binary);
    CheckFileOpen(deckOut, "deck.bin");
    Stack<Card*> cards = game->getCardDeck().getCards();
    size_t cardCount = cards.getSize();
    deckOut.write(reinterpret_cast<const char*>(&cardCount), sizeof(cardCount));
    while (cards.getSize() > 0) {
        Card* card = cards.top();
        int type = -1;
        double value = 0;
        if (dynamic_cast<PaymentCard*>(card)) {
            type = 0;
            value = static_cast<PaymentCard*>(card)->getMoneyAmount();
        }
        else if (dynamic_cast<GroupPaymentCard*>(card)) {
            type = 1;
            value = static_cast<GroupPaymentCard*>(card)->getPaymentAmount();
        }
        else if (dynamic_cast<MovePositionCard*>(card)) {
            type = 2;
            value = static_cast<MovePositionCard*>(card)->getTiles();
        }
        deckOut.write(reinterpret_cast<const char*>(&type), sizeof(type));
        deckOut.write(reinterpret_cast<const char*>(&value), sizeof(value));
        cards.pop();
    }
    deckOut.close();
}

void SaveGameCommand::savePendingTrades()
{
    std::ofstream tradesOut("trades.bin", std::ios::binary);
    CheckFileOpen(tradesOut, "trades.bin");
    const Vector<Player*>& players = game->getPlayers();
    const Vector<Property*>& properties = game->getGameBoard().getProperties();
    size_t playerCount = players.getSize();
    tradesOut.write(reinterpret_cast<const char*>(&playerCount), sizeof(playerCount));

    for (size_t i = 0; i < playerCount; ++i) {
        const Vector<Trade*>& trades = players[i]->getPendingTrades();
        size_t tradeCount = trades.getSize();
        tradesOut.write(reinterpret_cast<const char*>(&tradeCount), sizeof(tradeCount));
        for (size_t t = 0; t < tradeCount; ++t) {
            Trade* trade = trades[t];
            int proposerIdx = -1, receiverIdx = -1;
            for (size_t p = 0; p < playerCount; ++p) {
                if (players[p] == trade->getProposer()) proposerIdx = (int)(p);
                if (players[p] == trade->getReceiver()) receiverIdx = (int)(p);
            }
            tradesOut.write(reinterpret_cast<const char*>(&proposerIdx), sizeof(proposerIdx));
            tradesOut.write(reinterpret_cast<const char*>(&receiverIdx), sizeof(receiverIdx));
            int proposerMoney = trade->getProposerMoney();
            int receiverMoney = trade->getReceiverMoney();
            tradesOut.write(reinterpret_cast<const char*>(&proposerMoney), sizeof(proposerMoney));
            tradesOut.write(reinterpret_cast<const char*>(&receiverMoney), sizeof(receiverMoney));

            //save proposer properties
            const Vector<Property*>& proposerProps = trade->getProposerProperties();
            size_t proposerPropCount = proposerProps.getSize();
            tradesOut.write(reinterpret_cast<const char*>(&proposerPropCount), sizeof(proposerPropCount));
            for (size_t pi = 0; pi < proposerPropCount; ++pi) {
                int propIdx = -1;
                for (size_t j = 0; j < properties.getSize(); ++j) {
                    if (properties[j] == proposerProps[pi]) {
                        propIdx = (int)(j);
                        break;
                    }
                }
                tradesOut.write(reinterpret_cast<const char*>(&propIdx), sizeof(propIdx));
            }

            //save receiver properties
            const Vector<Property*>& receiverProps = trade->getReceiverProperties();
            size_t receiverPropCount = receiverProps.getSize();
            tradesOut.write(reinterpret_cast<const char*>(&receiverPropCount), sizeof(receiverPropCount));
            for (size_t ri = 0; ri < receiverPropCount; ++ri) {
                int propIdx = -1;
                for (size_t j = 0; j < properties.getSize(); ++j) {
                    if (properties[j] == receiverProps[ri]) {
                        propIdx = (int)(j);
                        break;
                    }
                }
                tradesOut.write(reinterpret_cast<const char*>(&propIdx), sizeof(propIdx));
            }
        }
    }
    tradesOut.close();
}

SaveGameCommand::SaveGameCommand(Monopoly* game)
    : game(game) {
}

void SaveGameCommand::execute() {
    if (!game) return;

    savePlayers();
    saveProperties();
    saveBoard();
    saveDeck();
    savePendingTrades();
}

void SaveGameCommand::undo() {
    //saving is not undoable
}

GameCommand* SaveGameCommand::clone() const {
    return new SaveGameCommand(game);
}