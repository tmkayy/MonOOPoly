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
#include "Vector.hpp"
#include "MyString.h"
#include <fstream>
#include "Consts.h"

void LoadGameCommand::loadPlayers(Vector<Player*>& loadedPlayers)
{
	std::ifstream playersIn("players.bin", std::ios::binary);
	CheckFileOpen(playersIn, "players.bin");
	size_t playerCount = 0;
	playersIn.read(reinterpret_cast<char*>(&playerCount), sizeof(playerCount));
	std::cout << "[LoadGame] Player count: " << playerCount << std::endl;
	for (size_t i = 0; i < playerCount; ++i) {
		size_t username;
		double money;
		size_t id, turnsInJail, pairsThrown;
		bool imprisoned;
		bool bankrupt;
		size_t turnsBankrupt;
		playersIn.read(reinterpret_cast<char*>(&username), sizeof(username));
		playersIn.read(reinterpret_cast<char*>(&money), sizeof(money));
		playersIn.read(reinterpret_cast<char*>(&id), sizeof(id));
		playersIn.read(reinterpret_cast<char*>(&turnsInJail), sizeof(turnsInJail));
		playersIn.read(reinterpret_cast<char*>(&pairsThrown), sizeof(pairsThrown));
		playersIn.read(reinterpret_cast<char*>(&imprisoned), sizeof(imprisoned));
		playersIn.read(reinterpret_cast<char*>(&turnsBankrupt), sizeof(turnsBankrupt));

		std::cout << "[LoadGame] Player " << i << ": username=" << username
			<< ", money=" << money << ", id=" << id
			<< ", turnsInJail=" << turnsInJail
			<< ", pairsThrown=" << pairsThrown
			<< ", imprisoned=" << imprisoned << std::endl;
		Player* player = new Player(game, username, money);
		player->setTurnsInJail(turnsInJail);
		player->setPairsThrown(pairsThrown);
		player->setImprisoned(imprisoned);
		player->setTurnsBankrupt(turnsBankrupt);
		loadedPlayers.push_back(player);
	}
	Vector<Player*>& players = game->getPlayers();
	players.clear();
	for (size_t i = 0; i < loadedPlayers.getSize(); ++i) {
		players.push_back(loadedPlayers[i]);
	}
	std::cout << "[LoadGame] Finished loading players." << std::endl;
	size_t currentPlayerIndex = 0;
	playersIn.read(reinterpret_cast<char*>(&currentPlayerIndex), sizeof(currentPlayerIndex));
	game->setCurrentPlayerIndexAndPlayer(currentPlayerIndex);
	playersIn.close();
}

void LoadGameCommand::loadProperties(const Vector<Player*>& loadedPlayers, Vector<Property*>& loadedProps)
{
	std::ifstream propsIn("properties.bin", std::ios::binary);
	CheckFileOpen(propsIn, "properties.bin");
	size_t propCount = 0;
	propsIn.read(reinterpret_cast<char*>(&propCount), sizeof(propCount));
	std::cout << "[LoadGame] Property count: " << propCount << std::endl;
	for (size_t i = 0; i < propCount; ++i) {
		size_t nameLen;
		propsIn.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
		MyString name;
		if (nameLen > 0) {
			char* buffer = new char[nameLen + 1];
			propsIn.read(buffer, nameLen);
			buffer[nameLen] = '\0';
			name = MyString(buffer);
			delete[] buffer;
		}
		double buyPrice, cottagePrice, castlePrice, rentPrice;
		PropertyColor color;
		propsIn.read(reinterpret_cast<char*>(&buyPrice), sizeof(buyPrice));
		propsIn.read(reinterpret_cast<char*>(&cottagePrice), sizeof(cottagePrice));
		propsIn.read(reinterpret_cast<char*>(&castlePrice), sizeof(castlePrice));
		propsIn.read(reinterpret_cast<char*>(&rentPrice), sizeof(rentPrice));
		propsIn.read(reinterpret_cast<char*>(&color), sizeof(color));
		int ownerId;
		propsIn.read(reinterpret_cast<char*>(&ownerId), sizeof(ownerId));
		std::cout << "[LoadGame] Property " << i << ": name=" << name.c_str()
			<< ", buyPrice=" << buyPrice << ", cottagePrice=" << cottagePrice
			<< ", castlePrice=" << castlePrice << ", rentPrice=" << rentPrice
			<< ", color=" << (int)(color)
			<< ", ownerId=" << ownerId << std::endl;
		Property* prop = new Property(name, buyPrice, cottagePrice, castlePrice, rentPrice, color, ownerId >= 0 ? loadedPlayers[ownerId] : nullptr);

		// mortgages
		size_t mortgageCount;
		propsIn.read(reinterpret_cast<char*>(&mortgageCount), sizeof(mortgageCount));
		std::cout << "[LoadGame]   Mortgages: " << mortgageCount << std::endl;
		for (size_t m = 0; m < mortgageCount; ++m) {
			int mortgageType;
			propsIn.read(reinterpret_cast<char*>(&mortgageType), sizeof(mortgageType));
			std::cout << "[LoadGame]     Mortgage " << m << ": type=" << mortgageType << std::endl;
			if (mortgageType == 0) prop->getMortgages().push_back(new Cottage());
			else if (mortgageType == 1) prop->getMortgages().push_back(new Castle());
		}
		if (prop->getOwner() != nullptr) {
			prop->getOwner()->incrementPropertyCount(prop->getColor());
		}
		loadedProps.push_back(prop);
	}
	Vector<Property*>& properties = game->getGameBoard().getProperties();
	properties.clear();
	for (size_t i = 0; i < loadedProps.getSize(); ++i) {
		properties.push_back(loadedProps[i]);
	}
	std::cout << "[LoadGame] Finished loading properties." << std::endl;
	propsIn.close();
}

void LoadGameCommand::loadBoard(const Vector<Property*>& loadedProps)
{
	std::ifstream boardIn("board.bin", std::ios::binary);
	CheckFileOpen(boardIn, "board.bin");
	size_t fieldCount = 0;
	boardIn.read(reinterpret_cast<char*>(&fieldCount), sizeof(fieldCount));
	std::cout << "[LoadGame] Board field count: " << fieldCount << std::endl;
	Vector<Field*>& boardFields = game->getGameBoard().getBoard();
	boardFields.clear();
	for (size_t i = 0; i < fieldCount; ++i) {
		int type;
		boardIn.read(reinterpret_cast<char*>(&type), sizeof(type));
		std::cout << "[LoadGame] Field " << i << ": type=" << type << std::endl;
		if (type == 1) {
			int propIndex;
			boardIn.read(reinterpret_cast<char*>(&propIndex), sizeof(propIndex));
			std::cout << "[LoadGame]   Property index: " << propIndex << std::endl;
			boardFields.push_back(loadedProps[propIndex]);
		}
		else if (type == 0) {
			int sfTypeInt;
			boardIn.read(reinterpret_cast<char*>(&sfTypeInt), sizeof(sfTypeInt));
			SpecialField::Type sfType = (SpecialField::Type)(sfTypeInt);
			size_t nameLen;
			boardIn.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
			MyString name;
			if (nameLen > 0) {
				char* buffer = new char[nameLen + 1];
				boardIn.read(buffer, nameLen);
				buffer[nameLen] = '\0';
				name = MyString(buffer);
				delete[] buffer;
			}
			double value;
			boardIn.read(reinterpret_cast<char*>(&value), sizeof(value));
			std::cout << "[LoadGame]   SpecialField: type=" << sfTypeInt << ", name=" << name.c_str() << ", value=" << value << std::endl;
			boardFields.push_back(new SpecialField(sfType, name, value));
		}
		else if (type == 2) {
			std::cout << "[LoadGame]   CardField" << std::endl;
			boardFields.push_back(new CardField());
		}
		else if (type == 3) {
			std::cout << "[LoadGame]   Jail" << std::endl;
			boardFields.push_back(new Jail());
		}
	}
	std::cout << "[LoadGame] Finished loading board." << std::endl;
	boardIn.close();
}

void LoadGameCommand::loadDeck()
{
	std::ifstream deckIn("deck.bin", std::ios::binary);
	CheckFileOpen(deckIn, "deck.bin");
	size_t cardCount = 0;
	deckIn.read(reinterpret_cast<char*>(&cardCount), sizeof(cardCount));
	std::cout << "[LoadGame] Deck card count: " << cardCount << std::endl;
	Stack<Card*> loadedCards;
	for (size_t i = 0; i < cardCount; ++i) {
		int type;
		double value;
		deckIn.read(reinterpret_cast<char*>(&type), sizeof(type));
		deckIn.read(reinterpret_cast<char*>(&value), sizeof(value));
		std::cout << "[LoadGame]   Card " << i << ": type=" << type << ", value=" << value << std::endl;
		if (type == 0) loadedCards.push(new PaymentCard(value));
		else if (type == 1) loadedCards.push(new GroupPaymentCard(value));
		else if (type == 2) loadedCards.push(new MovePositionCard((int)(value)));
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
	std::cout << "[LoadGame] Finished loading deck." << std::endl;
}

void LoadGameCommand::loadPendingTrades(const Vector<Player*>& loadedPlayers, const Vector<Property*>& loadedProps)
{
	std::ifstream tradesIn("trades.bin", std::ios::binary);
	CheckFileOpen(tradesIn, "trades.bin");

	size_t loadedPlayerCount = 0;
	tradesIn.read(reinterpret_cast<char*>(&loadedPlayerCount), sizeof(loadedPlayerCount));
	for (size_t i = 0; i < loadedPlayerCount; ++i) {
		size_t tradeCount = 0;
		tradesIn.read(reinterpret_cast<char*>(&tradeCount), sizeof(tradeCount));
		for (size_t t = 0; t < tradeCount; ++t) {
			int proposerIdx, receiverIdx;
			tradesIn.read(reinterpret_cast<char*>(&proposerIdx), sizeof(proposerIdx));
			tradesIn.read(reinterpret_cast<char*>(&receiverIdx), sizeof(receiverIdx));
			int proposerMoney, receiverMoney;
			tradesIn.read(reinterpret_cast<char*>(&proposerMoney), sizeof(proposerMoney));
			tradesIn.read(reinterpret_cast<char*>(&receiverMoney), sizeof(receiverMoney));

			Player* proposer = loadedPlayers[proposerIdx];
			Player* receiver = loadedPlayers[receiverIdx];

			size_t proposerPropCount = 0;
			tradesIn.read(reinterpret_cast<char*>(&proposerPropCount), sizeof(proposerPropCount));
			Vector<Property*> proposerProps;
			for (size_t pi = 0; pi < proposerPropCount; ++pi) {
				int propIdx;
				tradesIn.read(reinterpret_cast<char*>(&propIdx), sizeof(propIdx));
				proposerProps.push_back(loadedProps[propIdx]);
			}

			size_t receiverPropCount = 0;
			tradesIn.read(reinterpret_cast<char*>(&receiverPropCount), sizeof(receiverPropCount));
			Vector<Property*> receiverProps;
			for (size_t ri = 0; ri < receiverPropCount; ++ri) {
				int propIdx;
				tradesIn.read(reinterpret_cast<char*>(&propIdx), sizeof(propIdx));
				receiverProps.push_back(loadedProps[propIdx]);
			}

			Trade* trade = new Trade(proposer, receiver);
			for (size_t pi = 0; pi < proposerProps.getSize(); ++pi)
				trade->addProposerProperty(proposerProps[pi]);
			for (size_t ri = 0; ri < receiverProps.getSize(); ++ri)
				trade->addReceiverProperty(receiverProps[ri]);
			trade->setProposerMoney(proposerMoney);
			trade->setReceiverMoney(receiverMoney);

			receiver->getPendingTrades().push_back(trade);

			std::cout << "[LoadGame] Loaded trade: "
				<< "Proposer=" << proposer->tokenToString()
				<< ", Receiver=" << receiver->tokenToString()
				<< ", Offer=$" << proposerMoney
				<< ", Request=$" << receiverMoney;

			if (proposerProps.getSize() > 0) {
				std::cout << ", ProposerProps=[";
				for (size_t pi = 0; pi < proposerProps.getSize(); ++pi) {
					std::cout << proposerProps[pi]->getName().c_str();
					if (pi + 1 < proposerProps.getSize()) std::cout << ", ";
				}
				std::cout << "]";
			}
			if (receiverProps.getSize() > 0) {
				std::cout << ", ReceiverProps=[";
				for (size_t ri = 0; ri < receiverProps.getSize(); ++ri) {
					std::cout << receiverProps[ri]->getName().c_str();
					if (ri + 1 < receiverProps.getSize()) std::cout << ", ";
				}
				std::cout << "]";
			}
			std::cout << std::endl;
		}
	}
	tradesIn.close();
}

LoadGameCommand::LoadGameCommand(Monopoly* game)
	: game(game) {
}

void LoadGameCommand::execute() {
	if (!game) {
		std::cout << "[LoadGame] No game instance provided." << std::endl;
		return;
	}
	std::cout << "[LoadGame] Starting load process..." << std::endl;

	Vector<Player*> loadedPlayers;
	loadPlayers(loadedPlayers);

	Vector<Property*> loadedProps;
	loadProperties(loadedPlayers, loadedProps);

	loadBoard(loadedProps);

	loadDeck();

	loadPendingTrades(loadedPlayers, loadedProps);

	std::cout << "[LoadGame] Load process complete." << std::endl;

	game->advanceToNextPlayer();
}
void LoadGameCommand::undo() {
	//not implemented lol 
}

GameCommand* LoadGameCommand::clone() const {
	return new LoadGameCommand(game);
}