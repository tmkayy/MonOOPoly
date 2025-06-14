#include "AcceptTradeCommand.h"

AcceptTradeCommand::AcceptTradeCommand(Trade& trade)
    : trade(trade), wasAccepted(trade.getIsAccepted()) {
}

void AcceptTradeCommand::execute() {
    wasAccepted = trade.getIsAccepted();
    trade.accept();
}

void AcceptTradeCommand::undo() {
    if (!wasAccepted)
        return;

    //reverse money transfers first
    if (trade.getProposerMoney() > 0) {
        Bank::transferMoney(*trade.getReceiver(), *trade.getProposer(),
            trade.getProposerMoney());
    }
    if (trade.getReceiverMoney() > 0) {
        Bank::transferMoney(*trade.getProposer(), *trade.getReceiver(),
            trade.getReceiverMoney());
    }

    //reverse property transfers
    for (size_t i = 0; i < trade.getProposerProperties().getSize(); ++i) {
        Property* prop = trade.getProposerProperties()[i];
        if (prop) {
            trade.getReceiver()->decrementPropertyCount(prop->getColor());
            prop->setOwner(trade.getProposer());
            trade.getProposer()->incrementPropertyCount(prop->getColor());
            trade.checkMonopolyChange(prop->getColor(), trade.getReceiver(), trade.getProposer());
        }
    }

    for (size_t i = 0; i < trade.getReceiverProperties().getSize(); ++i) {
        Property* prop = trade.getReceiverProperties()[i];
        if (prop) {
            trade.getProposer()->decrementPropertyCount(prop->getColor());
            prop->setOwner(trade.getReceiver());
            trade.getReceiver()->incrementPropertyCount(prop->getColor());
            trade.checkMonopolyChange(prop->getColor(), trade.getProposer(), trade.getReceiver());
        }
    }

    //reset trade state
    trade.reject();
}

GameCommand* AcceptTradeCommand::clone() const {
    return new AcceptTradeCommand(trade);
}