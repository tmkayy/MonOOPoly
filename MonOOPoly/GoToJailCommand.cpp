#include "GoToJailCommand.h"
#include "Jail.h"
#include <cassert>

GoToJailCommand::GoToJailCommand(Player& player)
    : player(player), wasInJail(player.isImprisoned()) {
}

void GoToJailCommand::execute() {
    wasInJail = player.isImprisoned();
    Jail::sendToJail(player);
}

void GoToJailCommand::undo() {
    if (!wasInJail) {
        //only undo if player wasn't already in jail
        player.setImprisoned(false);
        player.setTurnsInJail(0);
        std::cout << Green << player.tokenToString()
            << " was released from jail (undo)" << Reset << std::endl;
    }
    else {
        //restore original turn count if player was already jailed
        player.setTurnsInJail(player.getTurnsInJail() - 1);
    }
}

GameCommand* GoToJailCommand::clone() const {
    return new GoToJailCommand(*this);
}