#include "ProposeTradeCommand.h"
#include "Trade.h"
#include "Bank.h"
#include <iostream>

ProposeTradeCommand::ProposeTradeCommand(
	Player& proposer,
	Player& receiver,
	const Vector<Property*>& propertiesOffered,
	const Vector<Property*>& propertiesRequested, double moneyOffered, double moneyRequested)
	: proposer(proposer), receiver(receiver),
	propertiesOffered(propertiesOffered), propertiesRequested(propertiesRequested), moneyOffered(moneyOffered), moneyRequested(moneyRequested), createdTrade(nullptr) {
}

ProposeTradeCommand::~ProposeTradeCommand() {
	//check if trade exists in pending trades
	bool isPending = false;
	for (size_t i = 0; i < receiver.getPendingTrades().getSize(); ++i) {
		if (receiver.getPendingTrades()[i] == createdTrade) {
			isPending = true;
			break;
		}
	}

	//delete if not in pending trades
	if (createdTrade && !isPending) {
		delete createdTrade;
	}
}

void ProposeTradeCommand::execute() {
	if (&proposer == &receiver) {
		std::cerr << Red << "Error: Cannot trade with yourself." << Reset << std::endl;
		return;
	}

	createdTrade = new Trade(&proposer, &receiver);

	//add properties to trade
	for (size_t i = 0; i < propertiesOffered.getSize(); ++i) {
		Property* prop = propertiesOffered[i];
		if (prop && prop->getOwner() == &proposer) {
			createdTrade->addProposerProperty(prop);
		}
	}

	for (size_t i = 0; i < propertiesRequested.getSize(); ++i) {
		Property* prop = propertiesRequested[i];
		if (prop && prop->getOwner() == &receiver) {
			createdTrade->addReceiverProperty(prop);
		}
	}

	createdTrade->setProposerMoney(moneyOffered);
	createdTrade->setReceiverMoney(moneyRequested);

	if (!createdTrade->isValid()) {
		std::cerr << Red << "Error: Invalid trade proposal." << Reset << std::endl;
		delete createdTrade;
		createdTrade = nullptr;
		return;
	}

	//add to receiver's pending trades
	receiver.getPendingTrades().push_back(createdTrade);
	std::cout << Green << proposer.tokenToString() << " proposed a trade to "
		<< receiver.tokenToString() << "." << Reset << std::endl;
}

void ProposeTradeCommand::undo() {
	if (!createdTrade) return;

	//remove from pending trades
	Vector<Trade*>& pending = receiver.getPendingTrades();
	for (size_t i = 0; i < pending.getSize(); ++i) {
		if (pending[i] == createdTrade) {
			pending.remove(i);
			delete createdTrade;
			createdTrade = nullptr;
			std::cout << Yellow << "Trade proposal has been withdrawn." << Reset << std::endl;
			return;
		}
	}
}

GameCommand* ProposeTradeCommand::clone() const {
	return new ProposeTradeCommand(
		proposer,
		receiver,
		propertiesOffered,
		propertiesRequested,
		moneyOffered,
		moneyRequested
	);
}