#include "SelectTradePartnerCommand.h"
#include <iostream>

SelectTradePartnerCommand::SelectTradePartnerCommand(Vector<Player*>& availablePartners,
    Player*& currentSelection)
    : availablePartners(availablePartners), currentSelection(currentSelection),
    previousSelection(nullptr), selectedPartner(nullptr), wasExecuted(false) {
}

void SelectTradePartnerCommand::execute() {
    if (availablePartners.isEmpty()) {
        std::cout << Red << "No trade partners available." << Reset << std::endl;
        return;
    }

    std::cout << Cyan << "Select a trade partner:" << Reset << std::endl;
    for (size_t i = 0; i < availablePartners.getSize(); i++) {
        std::cout << Yellow << i + 1 << ". " << availablePartners[i]->tokenToString() << Reset << std::endl;
    }
    std::cout << Yellow << "0. Cancel" << Reset << std::endl
        << "Enter choice (0-" << availablePartners.getSize() << "): ";

    size_t choice;
    std::cin >> choice;

    if (choice > 0 && choice <= availablePartners.getSize()) {
        previousSelection = currentSelection;
        selectedPartner = availablePartners[choice - 1];
        currentSelection = selectedPartner;
        wasExecuted = true;
        std::cout << Green << "Trade partner selected: " << selectedPartner->tokenToString() << Reset << std::endl;
    }
    else if (choice != 0) {
        std::cout << Red << "Invalid choice." << Reset << std::endl;
    }
}

void SelectTradePartnerCommand::undo() {
    if (wasExecuted && selectedPartner) {
        currentSelection = previousSelection;
        std::cout << Yellow << "Trade partner selection undone." << Reset << std::endl;
    }
}

GameCommand* SelectTradePartnerCommand::clone() const {
    return new SelectTradePartnerCommand(*this);
}

Player* SelectTradePartnerCommand::getSelectedPartner() const {
    return selectedPartner;
}