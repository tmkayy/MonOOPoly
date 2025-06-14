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
        std::cout << player.tokenToString()
            << " rolled doubles and was released from jail!\n";
    }
    else {
        player.setTurnsInJail(previousTurns + 1);
        std::cout << player.tokenToString()
            << " failed to roll doubles (turns in jail: "
            << player.getTurnsInJail() << ")\n";
    }
}

void RollForJailReleaseCommand::undo() {
    if (!wasInJail) return;

    if (rolledDoubles) {
        player.setImprisoned(true);
        std::cout << player.tokenToString()
            << " was returned to jail (undo)\n";
    }
    else {
        player.setTurnsInJail(previousTurns);
    }
}

GameCommand* RollForJailReleaseCommand::clone() const {
    return new RollForJailReleaseCommand(*this);
}