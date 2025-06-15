#include "SelectTradePartnerCommand.h"
SelectTradePartnerCommand::SelectTradePartnerCommand(Vector<Player*>& availablePartners,
    Player*& currentSelection)
    : availablePartners(availablePartners), currentSelection(currentSelection),
    previousSelection(nullptr), selectedPartner(nullptr), wasExecuted(false) {
}

void SelectTradePartnerCommand::execute() {
    if (availablePartners.isEmpty()) {
        return;
    }

    std::cout << "Select a trade partner:\n";
    for (size_t i = 0; i < availablePartners.getSize(); i++) {
        std::cout << i + 1 << ". " << availablePartners[i]->tokenToString() << "\n";
    }
    std::cout << "0. Cancel\n"
        << "Enter choice (0-" << availablePartners.getSize() << "): ";

    size_t choice;
    std::cin >> choice;

    if (choice > 0 && choice <= availablePartners.getSize()) {
        previousSelection = currentSelection;
        selectedPartner = availablePartners[choice - 1];
        currentSelection = selectedPartner;
        wasExecuted = true;
    }
}

void SelectTradePartnerCommand::undo() {
    if (wasExecuted && selectedPartner) {
        currentSelection = previousSelection;
    }
}

GameCommand* SelectTradePartnerCommand::clone() const {
    return new SelectTradePartnerCommand(*this);
}

Player* SelectTradePartnerCommand::getSelectedPartner() const {
    return selectedPartner;
}