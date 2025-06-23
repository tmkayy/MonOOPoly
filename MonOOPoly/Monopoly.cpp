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
#include "SaveGameCommand.h"

bool Monopoly::executeCommand(GameCommand* command) {
	if (!command)
		return false;
	command->execute();
	commandHistory.push_back(command);
	return true;
}

void Monopoly::undoLastCommand() {
	if (!commandHistory.isEmpty()) {
		GameCommand* last = commandHistory.peek();
		std::cout << Yellow << "Undoing: " << last->getDescription() << Reset << std::endl;
		last->undo();
		delete last;
		commandHistory.pop_back();
	}
	else {
		std::cout << Red << "No command to undo." << Reset << std::endl;
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
	std::cout << Cyan << "Player " << (players.getSize() + 1) << " - Select token (1-8):" << Reset << std::endl
		<< "1. RaceCar" << std::endl << "2. Thimble" << std::endl << "3. ScottishTerrier" << std::endl << "4. SackOfMoney" << std::endl
		<< "5. Cat" << std::endl << "6. Penguin" << std::endl << "7. RubberDuck" << std::endl << "8. Unknown" << std::endl;

	int tokenChoice;
	while (true) {
		std::cin >> tokenChoice;
		if (tokenChoice < 1 || tokenChoice > 8) {
			std::cout << Red << "Invalid choice. Select 1-8: " << Reset;
			continue;
		}

		Token token = (Token)(tokenChoice - 1);
		bool taken = false;
		for (size_t i = 0; i < players.getSize(); ++i) {
			if (players[i]->getUsername() == token) {
				taken = true;
				break;
			}
		}
		if (taken) {
			std::cout << Red << "Token already taken. Select another: " << Reset;
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
			std::cout << Green << std::endl << "=== GAME OVER ===" << Reset << std::endl;
			std::cout << Green << "Winner: " << players[i]->tokenToString() << Reset << std::endl;
			return;
		}
	}
	std::cout << Red << std::endl << "=== GAME OVER ===" << std::endl << "No winner - all players bankrupt!" << Reset << std::endl;
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

CardDeck& Monopoly::getCardDeck()
{
	return cards;
}

void Monopoly::startGame() {
	cards.generateDeck();
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
		std::cout << Cyan << currentPlayer->tokenToString() << "'s turn. " << Green << currentPlayer->getMoney() << "$" << Reset << std::endl;
		handleMovement();
		handleCurrentField();
		handlePlayerOptions();
	}

	advanceToNextPlayer();
}

void Monopoly::handleJailTurn() {
	std::cout << std::endl;
	if (currentPlayer->getTurnsInJail() >= maxTurnsJail) {
		std::cout << Yellow << currentPlayer->tokenToString() << " has been in jail for too long. Must pay bail." << Reset << std::endl;
		executeCommand(new PayJailFineCommand(*currentPlayer));
		return;
	}
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

	std::cout << Magenta << "Landed on field: " << currentPlayer->getId() << Reset << std::endl;
	// handle passing Go
	if (currentPlayer->getId() < previousPosition) {
		executeCommand(new PassGoCommand(*currentPlayer, false));
	}
}

void Monopoly::handleCurrentField() {
	Field* field = board.getBoard()[currentPlayer->getId()];
	if (dynamic_cast<Jail*>(field)) { //just visiting
		std::cout << Cyan << "Landed on jail field (just visiting): " << Reset << std::endl;
		return;
	}

	if (SpecialField* specialField = dynamic_cast<SpecialField*>(field)) {
		switch (specialField->getType()) {
		case SpecialField::Type::GO:
			//have to pass to get money
			break;

		case SpecialField::Type::FREE_PARKING:
			std::cout << Cyan << "Free Parking! No action taken." << Reset << std::endl;
			break;

		case SpecialField::Type::GO_TO_JAIL:
			std::cout << Red << "Go to Jail! Moving to jail..." << Reset << std::endl;
			executeCommand(new GoToJailCommand(*currentPlayer));
			break;

		case SpecialField::Type::INCOME_TAX:
		case SpecialField::Type::LUXURY_TAX:
			std::cout << Yellow << "Landed on " << specialField->getName() << ". Paying tax of " << Green << "$"
				<< specialField->getValue() << "." << Reset << std::endl;
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
		std::cout << Magenta << "Landed on card field: " << Reset << std::endl;
		executeCommand(new DrawCardCommand(*currentPlayer, cards, players));
	}
}


void Monopoly::handlePropertyLanding(Property* property) {
	std::cout << "Landed on: " << property->getName()
		<< " (" << colorToString(property->getColor()) << ")" << std::endl;

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

		std::cout << std::endl;
		switch (choice) {
		case 1: // build
			handleBuildingOptions();
			break;
		case 2: // trade
			handleTradeOptions();
			break;
		case 3: // sell property
			handleSellProperty();
			break;
		case 4: // pending Trades
			handlePendingTrades();
			break;
		case 5: // end Turn
			system("cls");
			return;
		case 6: // declare Bankruptcy
			executeCommand(new DeclareBankruptcyCommand(*currentPlayer, board.getProperties()));
			return;
		case 7: // save game and exit
			executeCommand(new SaveGameCommand(this));
			std::cout << "Game saved. Exiting..." << std::endl;
			exit(0);
		case 8: // undo last action
			undoLastCommand();
			std::cout << "Last action undone." << std::endl;
			break;
		case 9: // view properties by color
			viewPropertiesByColor();
			break;
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
	if (chosen->getPriceForCottage() == 0 || chosen->getPriceForCastle() == 0) {
		showMessage(MyString(Red) + "You cannot build on this property." + MyString(Reset));
		return;
	}
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
		case 1: { // add property to offer
			Property* prop = selectProperty(availableToOffer);
			if (prop) {
				executeCommand(new TradeSelectionCommand(
					*currentPlayer, availableToOffer, selectedToOffer, prop));
			}
			break;
		}
		case 2: { // remove property from offer
			Property* prop = selectProperty(selectedToOffer);
			if (prop) {
				executeCommand(new TradeSelectionCommand(
					*currentPlayer, selectedToOffer, availableToOffer, prop));
			}
			break;
		}
		case 3: { // add property to request
			Property* prop = selectProperty(availableToRequest);
			if (prop) {
				executeCommand(new TradeSelectionCommand(
					*partner, availableToRequest, selectedToRequest, prop));
			}
			break;
		}
		case 4: { // remove property from request
			Property* prop = selectProperty(selectedToRequest);
			if (prop) {
				executeCommand(new TradeSelectionCommand(
					*partner, selectedToRequest, availableToRequest, prop));
			}
			break;
		}
		case 5: { // set money amounts
			handleTradeMoneyOptions();
			break;
		}
		case 6: { // finalize trade
			if (validateTrade(selectedToOffer, selectedToRequest)) {
				executeCommand(new ProposeTradeCommand(
					*currentPlayer, *partner,
					selectedToOffer, selectedToRequest,
					moneyOffered, moneyRequested));
			}
			tradePartner = nullptr;  // reset trade partner
			return;
		}
		case 7: { // cancel
			tradePartner = nullptr;  // reset trade partner
			return;
		}
		}
	}
}


void Monopoly::displayTrade(const Trade* trade) {
	std::cout << Yellow << "From: " << trade->getProposer()->tokenToString()
		<< " | Offer: " << Green << "$" << trade->getProposerMoney() << Yellow;
	if (!trade->getProposerProperties().isEmpty()) {
		std::cout << " | Properties: ";
		for (size_t j = 0; j < trade->getProposerProperties().getSize(); ++j) {
			std::cout << trade->getProposerProperties()[j]->getName();
			if (j + 1 < trade->getProposerProperties().getSize()) std::cout << ", ";
		}
	}
	std::cout << " | Request: " << Green << "$" << trade->getReceiverMoney() << Yellow;
	if (!trade->getReceiverProperties().isEmpty()) {
		std::cout << " | Wants: ";
		for (size_t j = 0; j < trade->getReceiverProperties().getSize(); ++j) {
			std::cout << trade->getReceiverProperties()[j]->getName();
			if (j + 1 < trade->getReceiverProperties().getSize()) std::cout << ", ";
		}
	}
	std::cout << Reset << std::endl;
}

void Monopoly::handlePendingTrades() {
	if (!currentPlayer) {
		showMessage(MyString(Red) + "Error: No current player." + MyString(Reset));
		return;
	}
	Vector<Trade*>& pending = currentPlayer->getPendingTrades();
	if (pending.isEmpty()) {
		showMessage("No pending trades.");
		return;
	}

	while (true) {
		std::cout << Cyan << std::endl << "Pending Trades:" << Reset << std::endl;
		for (size_t i = 0; i < pending.getSize(); ++i) {
			std::cout << Yellow << i + 1 << ". " << Reset;
			displayTrade(pending[i]);
		}
		std::cout << Yellow << "0. Back" << Reset << std::endl << std::endl << Cyan
			<< "Select a trade to respond to (0-" << pending.getSize() << "): ";

		size_t choice;
		std::cin >> choice;
		if (choice == 0 || choice > pending.getSize()) return;

		Trade* selectedTrade = pending[choice - 1];
		if (!selectedTrade) {
			showMessage(MyString(Red) + "Error: Invalid trade selection." + MyString(Reset));
			continue;
		}
		std::cout << Yellow << "1. Accept" << std::endl << "2. Reject" << std::endl << "3. View Details"
			<< std::endl << "0. Back" << std::endl << Reset << "Enter choice: ";
		int action;
		std::cin >> action;
		if (action == 1) {
			executeCommand(new AcceptTradeCommand(*selectedTrade));
			pending.remove(choice - 1);
			showMessage(MyString(Green) + "Trade accepted." + MyString(Reset));
		}
		else if (action == 2) {
			executeCommand(new RejectTradeCommand(*selectedTrade));
			pending.remove(choice - 1);
			showMessage(MyString(Yellow) + "Trade rejected." + MyString(Reset));
		}
		else if (action == 3) {
			std::cout << Cyan << std::endl << "--- Trade Details ---" << Reset << std::endl;
			displayTrade(selectedTrade);
			std::cout << Cyan << "---------------------" << Reset << std::endl;
		}
		else if (action == 0) {
			return;
		}
		else {
			showMessage(MyString(Red) + "Invalid choice." + MyString(Reset));
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

	//automatically sell all mortgages before selling the property
	while (chosen->hasMortgages()) {
		executeCommand(new SellMortgageCommand(currentPlayer, *chosen));
	}

	executeCommand(new SellPropertyCommand(*currentPlayer, *chosen));
}

void Monopoly::advanceToNextPlayer() {
	if (players.isEmpty()) return;

	do {
		currentPlayerIndex = (currentPlayerIndex + 1) % players.getSize();
		currentPlayer = players[currentPlayerIndex];
	} while (currentPlayer->isBankrupt() && currentPlayerIndex != 0);

	//check if all players are bankrupt
	if (currentPlayer->isBankrupt()) {
		endGame();
	}
}

int Monopoly::showJailOptions() {
	std::cout << std::endl << Cyan << currentPlayer->tokenToString() << " is in jail. Options:" << std::endl << Yellow
		<< "1. Roll for doubles (cost: " << maxTurnsJail << " turns max)" << std::endl
		<< "2. Pay " << Green << "$" << bailCost << Yellow << " fine" << std::endl << Reset
		<< "Enter choice (1-2): ";

	int choice;
	while (true) {
		std::cin >> choice;
		if (choice == 1 || choice == 2) {
			return choice;
		}
		std::cout << Red << "Invalid choice. Please enter 1 or 2: " << Reset;
	}
}

bool Monopoly::showBuyPropertyPrompt(Property& property) {
	std::cout << "Would you like to buy " << property.getName()
		<< " for " << Green << "$" << property.getPriceToBuy() << Reset << "? (y/n): ";
	char choice;
	std::cin >> choice;
	return (choice == 'y' || choice == 'Y');
}

int Monopoly::showPlayerOptions() {
	std::cout
		<< Cyan << std::endl << "Options:" << std::endl << Reset
		<< "1. Build" << std::endl
		<< "2. Trade" << std::endl
		<< "3. Sell Property" << std::endl
		<< "4. Pending Trades" << std::endl
		<< "5. End Turn" << std::endl
		<< "6. Declare Bankruptcy" << std::endl
		<< "7. Save Game and Exit " << Red << "(!!!THIS WILL END YOUR TURN!!!)" << Reset << std::endl
		<< "8. Undo Last Action" << std::endl
		<< "9. View Properties by Color" << std::endl
		<< "Enter choice (1-9): ";

	int choice;
	while (true) {
		std::cin >> choice;
		if (choice >= 1 && choice <= 9) {
			return choice;
		}
		std::cout << Red << "Invalid choice. Please enter 1-9: " << Reset;
	}
}

void Monopoly::showMessage(const MyString& message) {
	std::cout << message << std::endl;
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
		showMessage(MyString(Red) + "No properties available to select." + MyString(Reset));
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

	if (!selected) {
		showMessage(MyString(Red) + "No property was selected." + MyString(Reset));
	}

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
		showMessage(MyString(Red) + "No available players to trade with" + MyString(Reset));
		return nullptr;
	}

	Player* selected = nullptr;
	executeCommand(new SelectTradePartnerCommand(
		availablePartners,
		selected
	));

	if (!selected) {
		showMessage(MyString(Red) + "No trade partner was selected." + MyString(Reset));
	}

	return selected;
}

size_t Monopoly::getCurrentPlayerIndex()
{
	return currentPlayerIndex;
}

void Monopoly::handleTradeMoneyOptions() {
	std::cout << Cyan << std::endl << "Trade Money Options:" << std::endl << Yellow
		<< "1. Set money to offer" << std::endl
		<< "2. Set money to request" << std::endl
		<< "3. Back to trade menu" << std::endl << Reset
		<< "Enter choice (1-3): ";

	int choice;
	std::cin >> choice;

	switch (choice) {
	case 1: {
		std::cout << "Enter amount to offer (current money: " << Green << "$"
			<< currentPlayer->getMoney() << Reset << "): ";
		int amount;
		std::cin >> amount;
		if (amount >= 0 && amount <= currentPlayer->getMoney()) {
			moneyOffered = amount;
		}
		else {
			showMessage(MyString(Red) + "Invalid amount!" + MyString(Reset));
		}
		break;
	}
	case 2: {
		std::cout << "Enter amount to request (partner has: " << Green << "$"
			<< currentPlayer->getMoney() << Reset << "): ";
		int amount;
		std::cin >> amount;
		if (amount >= 0) {
			moneyRequested = amount;
		}
		else {
			showMessage(MyString(Red) + "Invalid amount!" + MyString(Reset));
		}
		break;
	}
	case 3:
		// return to trade menu
		break;
	default:
		showMessage(MyString(Red) + "Invalid choice!" + MyString(Reset));
	}
}

void Monopoly::setCurrentPlayerIndexAndPlayer(size_t index) {
	currentPlayerIndex = index;
	if (players.getSize() > currentPlayerIndex) {
		currentPlayer = players[currentPlayerIndex];
	}
	else {
		currentPlayer = nullptr;
	}
}

bool Monopoly::validateTrade(const Vector<Property*>& offer,
	const Vector<Property*>& request) {
	if (!currentPlayer) {
		showMessage(MyString(Red) + "Error: No current player for trade validation." + MyString(Reset));
		return false;
	}
	if (!tradePartner) {
		showMessage(MyString(Red) + "Error: No trade partner selected." + MyString(Reset));
		return false;
	}

	//trade must include at least one item (property or money)
	if (offer.isEmpty() && request.isEmpty() && moneyOffered == 0 && moneyRequested == 0) {
		showMessage(MyString(Red) + "Trade must include at least one item!" + MyString(Reset));
		return false;
	}

	if (moneyOffered > currentPlayer->getMoney()) {
		showMessage(MyString(Red) + "You don't have enough money for this offer!" + MyString(Reset));
		return false;
	}

	if (moneyRequested > tradePartner->getMoney()) {
		showMessage(MyString(Red) + "Trade partner doesn't have enough money for this request!" + MyString(Reset));
		return false;
	}

	for (size_t i = 0; i < offer.getSize(); i++) {
		if (!offer[i]) {
			showMessage(MyString(Red) + "Error: Null property in offer." + MyString(Reset));
			return false;
		}
		if (offer[i]->hasMortgages()) {
			showMessage(MyString(Red) + "Cannot trade mortgaged properties!" + MyString(Reset));
			return false;
		}
	}

	for (size_t i = 0; i < request.getSize(); i++) {
		if (!request[i]) {
			showMessage(MyString(Red) + "Error: Null property in request." + MyString(Reset));
			return false;
		}
		if (request[i]->hasMortgages()) {
			showMessage(MyString(Red) + "Cannot request mortgaged properties!" + MyString(Reset));
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
		showMessage(MyString(Red) + "Invalid number. Please enter between 2 and 8: " + MyString(Reset));
		std::cin >> numPlayers;
	}

	for (int i = 0; i < numPlayers; i++) {
		addPlayerWithTokenSelection();
	}
	system("cls");
}

int Monopoly::showTradeSelectionMenu() {
	std::cout << Cyan << std::endl << "Trade Selection Menu:" << std::endl << Reset
		<< "1. Add property to offer" << std::endl
		<< "2. Remove property from offer" << std::endl
		<< "3. Add property to request" << std::endl
		<< "4. Remove property from request" << std::endl
		<< "5. Set money amounts" << std::endl
		<< "6. Finalize trade" << std::endl
		<< "7. Cancel trade" << std::endl
		<< "Enter choice (1-7): ";

	int choice;
	while (true) {
		std::cin >> choice;
		if (choice >= 1 && choice <= 7) {
			return choice;
		}
		std::cout << Red << "Invalid choice. Please enter 1-7: " << Reset;
	}
}

int Monopoly::selectBuildingType(Property* property) {
	if (!property) {
		showMessage(MyString(Red) + "Error: Null property in offer." + MyString(Reset));
		return 3; // cancel if invalid property
	}

	std::cout << Cyan << std::endl << "Select Building Type for " << property->getName() << ":" << std::endl << Yellow
		<< "1. Cottage " << Green << "$" << property->getPriceForCottage() << Yellow << "(rent multiplier : " << cottageRent << "x)" << std::endl
		<< "2. Castle " << Green << "$" << property->getPriceForCastle() << Yellow << "(rent multiplier : " << castleRent << "x)" << std::endl
		<< "3. Cancel" << std::endl << Reset
		<< "Enter choice (1-3): ";

	int choice;
	while (true) {
		std::cin >> choice;
		if (choice >= 1 && choice <= 3) {
			return choice;
		}
		std::cout << MyString(Red) + "Invalid choice. Please enter 1-3: " + MyString(Reset);
	}
}

void Monopoly::viewPropertiesByColor() {
	if (!currentPlayer) {
		showMessage(MyString(Red) + "Error: No current player." + MyString(Reset));
		return;
	}
	Vector<Property*> allProps = getPlayerProperties(*currentPlayer);
	if (allProps.isEmpty()) {
		showMessage(MyString(Yellow) + "You have no properties." + MyString(Reset));
		return;
	}

	std::cout << Reset << std::endl << std::endl << "Your Properties by Color:" << std::endl;
	for (int color = 0; color < (int)(PropertyColor::Count); ++color) {
		bool hasAny = false;
		for (size_t i = 0; i < allProps.getSize(); ++i) {
			if (allProps[i]->getColor() == (PropertyColor)(color)) {
				if (!hasAny) {
					std::cout << colorToString((PropertyColor)(color)) << ":" << std::endl;
					hasAny = true;
				}
				std::cout << "  - " << allProps[i]->getName() << std::endl;
			}
		}
	}
}