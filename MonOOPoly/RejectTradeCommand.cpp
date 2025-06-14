#include "RejectTradeCommand.h"
#include "Trade.h"
#include "Player.h"

RejectTradeCommand::RejectTradeCommand(Trade& trade)
    : trade(trade), wasPending(true) {
}

void RejectTradeCommand::execute() {
    wasPending = true;
    trade.reject();
}

void RejectTradeCommand::undo() {
    if (wasPending) {
        if (Player* receiver = trade.getReceiver()) {
            receiver->getPendingTrades().push_back(&trade);
        }
    }
}

GameCommand* RejectTradeCommand::clone() const {
    return new RejectTradeCommand(*this);
}