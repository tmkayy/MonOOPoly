#include "RollForJailReleaseCommand.h"
#include <iostream>

RollForJailReleaseCommand::RollForJailReleaseCommand(Player& player, bool rolledDoubles)
    : player(player), rolledDoubles(rolledDoubles),
    wasInJail(player.isImprisoned()),
    previousTurns(player.getTurnsInJail()) {
}

void RollForJailReleaseCommand::execute() {
    if (!wasInJail) return;

    if (rolledDoubles) {
        player.setImprisoned(false);
        std::cout << Green << player.tokenToString()
            << " rolled doubles and was released from jail!" << Reset << std::endl;
    }
    else {
        player.setTurnsInJail(previousTurns + 1);
        std::cout << Yellow << player.tokenToString()
            << " failed to roll doubles (turns in jail: "
            << player.getTurnsInJail() << ")" << Reset << std::endl;
    }
}

void RollForJailReleaseCommand::undo() {
    if (!wasInJail) return;

    if (rolledDoubles) {
        player.setImprisoned(true);
        std::cout << Cyan << player.tokenToString()
            << " was returned to jail (undo)" << Reset << std::endl;
    }
    else {
        player.setTurnsInJail(previousTurns);
    }
}

GameCommand* RollForJailReleaseCommand::clone() const {
    return new RollForJailReleaseCommand(*this);
}