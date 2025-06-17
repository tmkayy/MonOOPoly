#include "Monopoly.h"
#include "RollForJailReleaseCommand.h";
#include "MovePlayerCommand.h";
#include "PayJailFineCommand.h";
#include "PayRentCommand.h";
#include "BuyPropertyCommand.h";
#include "DeclareBankruptcyCommand.h"
#include "SellPropertyCommand.h";
#include "GoToJailCommand.h";
#include "CardCommands.h";
#include "PassGoCommand.h";
#include "PayTaxCommand.h";
#include "TradeSelectionCommand.h";
#include "SelectPropertyCommand.h";
#include "SelectTradePartnerCommand.h";
#include "ProposeTradeCommand.h";
#include "Property.h"
#include "BuildCottageCommand.h"
#include "BuildCastleCommand.h"
#include "SellMortgageCommand.h"
#include "AddPlayerCommand.h";
#include "AcceptTradeCommand.h";
#include "RejectTradeCommand.h";

bool Monopoly::executeCommand(GameCommand* command) {
	if (!command)
		return false;
	command->execute();
	commandHistory.push_back(command);
	return true;
}

void Monopoly::undoLastCommand() {
	if (!commandHistory.isEmpty()) {
		commandHistory.peek()->undo();
		delete commandHistory.peek();
		commandHistory.pop_back();
	}
}


void Monopoly::copyFrom(const Monopoly& other) {
	for (size_t i = 0; i < other.players.getSize(); i++) {
		players.push_back(new Player(*other.players[i]));
	}

	for (size_t i = 0; i < other.commandHistory.getSize(); i++) {
		commandHistory.push_back(other.commandHistory[i]->clone());
	}

	board = other.board;
	cards = other.cards;
	bank = other.bank;
	d1 = other.d1;
	d2 = other.d2;
	currentPlayerIndex = other.currentPlayerIndex;
	currentPlayer = (players.getSize() > currentPlayerIndex) ? players[currentPlayerIndex] : nullptr;
}

void Monopoly::moveFrom(Monopoly&& other) noexcept
{
	players = std::move(other.players);
	commandHistory = std::move(other.commandHistory);
	board = std::move(other.board);
	cards = std::move(other.cards);
	bank = std::move(other.bank);
	d1 = std::move(other.d1);
	d2 = std::move(other.d2);
	currentPlayerIndex = other.currentPlayerIndex;
	currentPlayer = other.currentPlayer;

	other.currentPlayerIndex = 0;
	other.currentPlayer = nullptr;
	other.players = Vector<Player*>();
	other.commandHistory = Vector<GameCommand*>();
}

void Monopoly::free() {
	for (size_t i = 0; i < players.getSize(); i++) {
		delete players[i];
	}
	players.clear();

	for (size_t i = 0; i < commandHistory.getSize(); i++) {
		delete commandHistory[i];
	}
	commandHistory.clear();

	currentPlayerIndex = 0;
	currentPlayer = nullptr;
}

void Monopoly::addPlayerWithTokenSelection() {
	std::cout << "Player " << (players.getSize() + 1) << " - Select token (1-8):\n"
		<< "1. RaceCar\n2. Thimble\n3. ScottishTerrier\n4. SackOfMoney\n"
		<< "5. Cat\n6. Penguin\n7. RubberDuck\n8. Unknown\n";

	int tokenChoice;
	while (true) {
		std::cin >> tokenChoice;
		if (tokenChoice < 1 || tokenChoice > 8) {
			std::cout << "Invalid choice. Select 1-8: ";
			continue;
		}

		Token token = static_cast<Token>(tokenChoice - 1);
		bool taken = false;
		for (size_t i = 0; i < players.getSize(); ++i) {
			if (players[i]->getUsername() == token) {
				taken = true;
				break;
			}
		}
		if (taken) {
			std::cout << "Token already taken. Select another: ";
			continue;
		}

		executeCommand(new AddPlayerCommand(this, token));
		break;
	}
}

bool Monopoly::isGameOver() const {
	int activePlayers = 0;
	for (size_t i = 0; i < players.getSize(); i++) {
		if (!players[i]->isBankrupt()) {
			activePlayers++;
			if (activePlayers > 1) return false;
		}
	}
	return true;
}

void Monopoly::announceWinner() const {
	for (size_t i = 0; i < players.getSize(); i++) {
		if (!players[i]->isBankrupt()) {
			std::cout << "\n=== GAME OVER ===\n";
			std::cout << "Winner: " << players[i]->tokenToString() << std::endl;
			return;
		}
	}
	std::cout << "\n=== GAME OVER ===\nNo winner - all players bankrupt!\n";
}

Monopoly::Monopoly()
	: board(), cards(), bank(), d1(), d2(),
	moneyOffered(0), moneyRequested(0), tradePartner(nullptr) {
}

Monopoly::~Monopoly() noexcept {
	free();
}

Monopoly::Monopoly(const Monopoly& other) {
	copyFrom(other);
}

Monopoly& Monopoly::operator=(const Monopoly& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

Monopoly::Monopoly(Monopoly&& other) noexcept {
	moveFrom(std::move(other));
}

Monopoly& Monopoly::operator=(Monopoly&& other) noexcept {
	if (this != &other) {
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

Board& Monopoly::getGameBoard()
{
	return board;
}

void Monopoly::startGame() {
	cards.shuffle();
	if (!players.isEmpty()) {
		currentPlayerIndex = 0;
		currentPlayer = players[0];
	}
}

void Monopoly::endGame() {
	free();
}

void Monopoly::nextTurn() {
	if (players.isEmpty()) return;

	if (currentPlayer->isImprisoned()) {
		handleJailTurn();
		if (currentPlayer->isImprisoned()) {
			advanceToNextPlayer();
			return;
		}
	}

	// building/trading phase if not bankrupt
	if (!currentPlayer->isBankrupt()) {
		std::cout << "\n" << currentPlayer->tokenToString() << "'s turn. " << currentPlayer->getMoney() << "$ Options:\n";
		handleMovement();
		handleCurrentField();
		handlePlayerOptions();
	}

	advanceToNextPlayer();
}

void Monopoly::handleJailTurn() {
	int choice = showJailOptions();

	if (choice == 1) {
		bool doubles = (d1.roll() == d2.roll());
		executeCommand(new RollForJailReleaseCommand(*currentPlayer, doubles));
	}
	else if (choice == 2) {
		executeCommand(new PayJailFineCommand(*currentPlayer));
	}
}


void Monopoly::handleMovement() {
	size_t previousPosition = currentPlayer->getId();
	executeCommand(new MovePlayerCommand(*currentPlayer, board, d1, d2));

	// Handle passing Go
	if (currentPlayer->getId() < previousPosition) {
		executeCommand(new PassGoCommand(*currentPlayer, false));
	}
}

void Monopoly::handleCurrentField() {
	Field* field = board.getBoard()[currentPlayer->getId()];
	if (dynamic_cast<Jail*>(field)) { //just visiting
		std::cout << "Landed on jail field (just visiting): " << "\n";
		return;
	}

	if (SpecialField* specialField = dynamic_cast<SpecialField*>(field)) {
		switch (specialField->getType()) {
		case SpecialField::Type::GO:
			executeCommand(new PassGoCommand(*currentPlayer, true));
			std::cout << "Passed GO! Collected $" << goMoney << std::endl;
			break;

		case SpecialField::Type::FREE_PARKING:
			std::cout << "Free Parking! No action taken.\n";
			break;

		case SpecialField::Type::GO_TO_JAIL:
			std::cout << "Go to Jail! Moving to jail...\n";
			executeCommand(new GoToJailCommand(*currentPlayer));
			break;

		case SpecialField::Type::INCOME_TAX:
		case SpecialField::Type::LUXURY_TAX:
			std::cout << "Landed on " << specialField->getName() << ". Paying tax of $"
				<< specialField->getValue() << ".\n";
			executeCommand(new PayTaxCommand(*currentPlayer, specialField->getValue()));
			break;

		default:
			break;
		}
		return;
	}

	if (Property* property = dynamic_cast<Property*>(field)) {
		handlePropertyLanding(property);
	}
	else if (CardField* cardField = dynamic_cast<CardField*>(field)) {
		std::cout << "Landed on card field: " << "\n";
		executeCommand(new DrawCardCommand(*currentPlayer, cards, players));
	}
}


void Monopoly::handlePropertyLanding(Property* property) {
	std::cout << "Landed on: " << property->getName()
		<< " (" << colorToString(property->getColor()) << ")\n";

	if (!property->getOwner()) {
		//offer to buy
		if (showBuyPropertyPrompt(*property)) {
			executeCommand(new BuyPropertyCommand(*currentPlayer, *property));
		}
	}
	else if (property->getOwner() != currentPlayer) {
		executeCommand(new PayRentCommand(*currentPlayer, *property));
	}
}

void Monopoly::handlePlayerOptions() {
	while (true) {
		int choice = showPlayerOptions();

		switch (choice) {
		case 1: //build
			handleBuildingOptions();
			break;
		case 2: //trade
			handleTradeOptions();
			break;
		case 3: //sell property
			handleSellProperty();
			break;
		case 4: //pending Trades
			handlePendingTrades();
			break;
		case 5: // end Turn
			system("cls");
			return;
		case 6: // declare Bankruptcy
			executeCommand(new DeclareBankruptcyCommand(*currentPlayer, board.getProperties()));
			return;
		}
	}
}

void Monopoly::handleBuildingOptions() {
	Vector<Property*> buildable = getBuildableProperties(*currentPlayer);
	if (buildable.isEmpty()) {
		showMessage("No properties available for building");
		return;
	}

	Property* chosen = selectProperty(buildable);
	if (!chosen) return;

	int buildingType = selectBuildingType(chosen);
	if (buildingType == 1) {
		executeCommand(new BuildCottageCommand(currentPlayer, *chosen));
	}
	else if (buildingType == 2) {
		executeCommand(new BuildCastleCommand(currentPlayer, *chosen));
	}
}

void Monopoly::handleTradeOptions() {
	Player* partner = selectTradePartner();
	if (!partner) return;

	moneyOffered = 0;
	moneyRequested = 0;
	tradePartner = partner;

	Vector<Property*> availableToOffer = getPlayerProperties(*currentPlayer);
	Vector<Property*> availableToRequest = getPlayerProperties(*partner);
	Vector<Property*> selectedToOffer;
	Vector<Property*> selectedToRequest;

	while (true) {
		int choice = showTradeSelectionMenu();
		switch (choice) {
		case 1: { // Add property to offer
			Property* prop = selectProperty(availableToOffer);
			std::cout << prop->getName();
			if (prop) {
				executeCommand(new TradeSelectionCommand(
					*currentPlayer, availableToOffer, selectedToOffer, prop));
			}
			break;
		}
		case 2: { // Remove property from offer
			Property* prop = selectProperty(selectedToOffer);
			if (prop) {
				executeCommand(new TradeSelectionCommand(
					*currentPlayer, selectedToOffer, availableToOffer, prop));
			}
			break;
		}
		case 3: { // Add property to request
			Property* prop = selectProperty(availableToRequest);
			if (prop) {
				executeCommand(new TradeSelectionCommand(
					*partner, availableToRequest, selectedToRequest, prop));
			}
			break;
		}
		case 4: { // Remove property from request
			Property* prop = selectProperty(selectedToRequest);
			if (prop) {
				executeCommand(new TradeSelectionCommand(
					*partner, selectedToRequest, availableToRequest, prop));
			}
			break;
		}
		case 5: { // Set money amounts
			handleTradeMoneyOptions();
			break;
		}
		case 6: { // Finalize trade
			if (validateTrade(selectedToOffer, selectedToRequest)) {
				executeCommand(new ProposeTradeCommand(
					*currentPlayer, *partner,
					selectedToOffer, selectedToRequest,
					moneyOffered, moneyRequested));
			}
			tradePartner = nullptr;  // Reset trade partner
			return;
		}
		case 7: { // Cancel
			tradePartner = nullptr;  // Reset trade partner
			return;
		}
		}
	}
}


void Monopoly::displayTrade(const Trade* trade) {
	std::cout << "From: " << trade->getProposer()->tokenToString()
		<< " | Offer: $" << trade->getProposerMoney();
	if (!trade->getProposerProperties().isEmpty()) {
		std::cout << " | Properties: ";
		for (size_t j = 0; j < trade->getProposerProperties().getSize(); ++j) {
			std::cout << trade->getProposerProperties()[j]->getName();
			if (j + 1 < trade->getProposerProperties().getSize()) std::cout << ", ";
		}
	}
	std::cout << " | Request: $" << trade->getReceiverMoney();
	if (!trade->getReceiverProperties().isEmpty()) {
		std::cout << " | Wants: ";
		for (size_t j = 0; j < trade->getReceiverProperties().getSize(); ++j) {
			std::cout << trade->getReceiverProperties()[j]->getName();
			if (j + 1 < trade->getReceiverProperties().getSize()) std::cout << ", ";
		}
	}
	std::cout << "\n";
}

void Monopoly::handlePendingTrades() {
	if (!currentPlayer) return;
	Vector<Trade*>& pending = currentPlayer->getPendingTrades();
	if (pending.isEmpty()) {
		showMessage("No pending trades.");
		return;
	}

	while (true) {
		std::cout << "\nPending Trades:\n";
		for (size_t i = 0; i < pending.getSize(); ++i) {
			std::cout << i + 1 << ". ";
			displayTrade(pending[i]);
		}
		std::cout << "0. Back\nSelect a trade to respond to (0-" << pending.getSize() << "): ";

		size_t choice;
		std::cin >> choice;
		if (choice == 0 || choice > pending.getSize()) return;

		Trade* selectedTrade = pending[choice - 1];
		std::cout << "1. Accept\n2. Reject\n3. View Details\n0. Back\nEnter choice: ";
		int action;
		std::cin >> action;
		if (action == 1) {
			executeCommand(new AcceptTradeCommand(*selectedTrade));
			pending.remove(choice - 1);
			showMessage("Trade accepted.");
		}
		else if (action == 2) {
			executeCommand(new RejectTradeCommand(*selectedTrade));
			pending.remove(choice - 1);
			showMessage("Trade rejected.");
		}
		else if (action == 3) {
			std::cout << "\n--- Trade Details ---\n";
			displayTrade(selectedTrade);
			std::cout << "---------------------\n";
		}
		else if (action == 0) {
			return;
		}
		else {
			showMessage("Invalid choice.");
		}
	}
}


void Monopoly::handleSellProperty() {
	Vector<Property*> sellable = getPlayerProperties(*currentPlayer);
	if (sellable.isEmpty()) {
		showMessage("No properties available to sell");
		return;
	}

	Property* chosen = selectProperty(sellable);
	if (!chosen) return;

	if (chosen->hasMortgages()) {
		executeCommand(new SellMortgageCommand(currentPlayer, *chosen));
	}
	else {
		executeCommand(new SellPropertyCommand(*currentPlayer, *chosen));
	}
}

void Monopoly::advanceToNextPlayer() {
	if (players.isEmpty()) return;

	do {
		currentPlayerIndex = (currentPlayerIndex + 1) % players.getSize();
		currentPlayer = players[currentPlayerIndex];
	} while (currentPlayer->isBankrupt() && currentPlayerIndex != 0);

	// Check if all players are bankrupt
	if (currentPlayer->isBankrupt()) {
		endGame();
	}
}

int Monopoly::showJailOptions() {
	std::cout << currentPlayer->tokenToString() << " is in jail. Options:\n"
		<< "1. Roll for doubles (cost: " << maxTurnsJail << " turns max)\n"
		<< "2. Pay $" << bailCost << " fine\n"
		<< "Enter choice (1-2): ";

	int choice;
	while (true) {
		std::cin >> choice;
		if (choice == 1 || choice == 2) {
			return choice;
		}
		std::cout << "Invalid choice. Please enter 1 or 2: ";
	}
}

bool Monopoly::showBuyPropertyPrompt(Property& property) {
	std::cout << "Would you like to buy " << property.getName()
		<< " for $" << property.getPriceToBuy() << "? (y/n): ";
	char choice;
	std::cin >> choice;
	return (choice == 'y' || choice == 'Y');
}

int Monopoly::showPlayerOptions() {
	std::cout
		<< "1. Build\n"
		<< "2. Trade\n"
		<< "3. Sell Property\n"
		<< "4. Pending Trades\n" // Add this line
		<< "5. End Turn\n"
		<< "6. Declare Bankruptcy\n"
		<< "Enter choice (1-6): ";

	int choice;
	while (true) {
		std::cin >> choice;
		if (choice >= 1 && choice <= 6) {
			return choice;
		}
		std::cout << "Invalid choice. Please enter 1-6: ";
	}
}

void Monopoly::showMessage(const std::string& message) {
	std::cout << message << "\n";
}

Vector<Property*> Monopoly::getPlayerProperties(Player& player) {
	Vector<Property*> properties;
	const Vector<Property*>& allProperties = board.getProperties();

	for (size_t i = 0; i < allProperties.getSize(); i++) {
		if (allProperties[i]->getOwner() == &player) {
			properties.push_back(allProperties[i]);
		}
	}

	return properties;
}

Vector<Property*> Monopoly::getBuildableProperties(Player& player) {
	Vector<Property*> buildable;
	Vector<Property*> properties = getPlayerProperties(player);

	for (size_t i = 0; i < properties.getSize(); i++) {
		Property* prop = properties[i];
		if (player.hasMonopoly(prop->getColor())) {
			buildable.push_back(prop);
		}
	}

	return buildable;
}


Property* Monopoly::selectProperty(const Vector<Property*>& properties) {
	if (properties.isEmpty()) {
		return nullptr;
	}

	//create temporary vectors for the command
	Vector<Property*> mutableProps = properties;
	Vector<Property*> selectionResult;
	Property* selected = nullptr;

	executeCommand(new SelectPropertyCommand(
		*currentPlayer,
		mutableProps,
		selectionResult,
		selected
	));

	return selected;
}

Player* Monopoly::selectTradePartner() {
	Vector<Player*> availablePartners;
	for (size_t i = 0; i < players.getSize(); i++) {
		Player* p = players[i];
		if (p != currentPlayer && !p->isBankrupt()) {
			availablePartners.push_back(p);
		}
	}

	if (availablePartners.isEmpty()) {
		showMessage("No available players to trade with");
		return nullptr;
	}

	Player* selected = nullptr;
	executeCommand(new SelectTradePartnerCommand(
		availablePartners,
		selected
	));

	return selected;
}

void Monopoly::handleTradeMoneyOptions() {
	std::cout << "\nTrade Money Options:\n"
		<< "1. Set money to offer\n"
		<< "2. Set money to request\n"
		<< "3. Back to trade menu\n"
		<< "Enter choice (1-3): ";

	int choice;
	std::cin >> choice;

	switch (choice) {
	case 1: {
		std::cout << "Enter amount to offer (current money: $"
			<< currentPlayer->getMoney() << "): ";
		int amount;
		std::cin >> amount;
		if (amount >= 0 && amount <= currentPlayer->getMoney()) {
			moneyOffered = amount;
		}
		else {
			showMessage("Invalid amount!");
		}
		break;
	}
	case 2: {
		std::cout << "Enter amount to request (partner has: $"
			<< tradePartner->getMoney() << "): ";
		int amount;
		std::cin >> amount;
		if (amount >= 0) {
			moneyRequested = amount;
		}
		else {
			showMessage("Invalid amount!");
		}
		break;
	}
	case 3:
		// Return to trade menu
		break;
	default:
		showMessage("Invalid choice!");
	}
}

bool Monopoly::validateTrade(const Vector<Property*>& offer,
	const Vector<Property*>& request) {
	// Trade must include at least one item (property or money)
	if (offer.isEmpty() && request.isEmpty() && moneyOffered == 0 && moneyRequested == 0) {
		showMessage("Trade must include at least one item!");
		return false;
	}

	// Check if player can afford the money they're offering
	if (moneyOffered > currentPlayer->getMoney()) {
		showMessage("You don't have enough money for this offer!");
		return false;
	}

	// Check if trade partner can afford the money they're being asked for
	if (moneyRequested > tradePartner->getMoney()) {
		showMessage("Trade partner doesn't have enough money for this request!");
		return false;
	}

	// All properties must be unencumbered (not mortgaged)
	for (size_t i = 0; i < offer.getSize(); i++) {
		if (offer[i]->hasMortgages()) {
			showMessage("Cannot trade mortgaged properties!");
			return false;
		}
	}

	for (size_t i = 0; i < request.getSize(); i++) {
		if (request[i]->hasMortgages()) {
			showMessage("Cannot request mortgaged properties!");
			return false;
		}
	}

	return true;
}

void Monopoly::initializePlayers()
{
	int numPlayers;
	std::cout << "Enter number of players (2-8): ";
	std::cin >> numPlayers;

	while (numPlayers < 2 || numPlayers > 8) {
		std::cout << "Invalid number. Please enter between 2 and 8: ";
		std::cin >> numPlayers;
	}

	for (int i = 0; i < numPlayers; i++) {
		addPlayerWithTokenSelection();
	}
}

int Monopoly::showTradeSelectionMenu() {
	std::cout << "\nTrade Selection Menu:\n"
		<< "1. Add property to offer\n"
		<< "2. Remove property from offer\n"
		<< "3. Add property to request\n"
		<< "4. Remove property from request\n"
		<< "5. Set money amounts\n"
		<< "6. Finalize trade\n"
		<< "7. Cancel trade\n"
		<< "Enter choice (1-7): ";

	int choice;
	while (true) {
		std::cin >> choice;
		if (choice >= 1 && choice <= 7) {
			return choice;
		}
		std::cout << "Invalid choice. Please enter 1-7: ";
	}
}

int Monopoly::selectBuildingType(Property* property) {
	if (!property) return 3; // Cancel if invalid property

	std::cout << "\nSelect Building Type for " << property->getName() << ":\n"
		<< "1. Cottage (rent multiplier: " << cottageRent << "x)\n"
		<< "2. Castle (rent multiplier: " << castleRent << "x)\n"
		<< "3. Cancel\n"
		<< "Enter choice (1-3): ";

	int choice;
	while (true) {
		std::cin >> choice;
		if (choice >= 1 && choice <= 3) {
			return choice;
		}
		std::cout << "Invalid choice. Please enter 1-3: ";
	}
}