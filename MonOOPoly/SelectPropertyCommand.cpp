#include "SelectPropertyCommand.h"

SelectPropertyCommand::SelectPropertyCommand(Player& player,
    Vector<Property*>& sourceList,
    Vector<Property*>& targetList,
    Property*& property)
    : player(player), sourceList(sourceList), targetList(targetList),
    selectedProperty(property), originalIndex(-1), wasExecuted(false) {
}

void SelectPropertyCommand::execute() {
    if (sourceList.isEmpty()) {
        return;
    }

    std::cout << "Select a property:\n";
    for (size_t i = 0; i < sourceList.getSize(); i++) {
        std::cout << i + 1 << ". " << sourceList[i]->getName()
            << " (" << colorToString(sourceList[i]->getColor()) << ")\n";
    }
    std::cout << "0. Cancel\n"
        << "Enter choice (0-" << sourceList.getSize() << "): ";

    size_t choice;
    std::cin >> choice;

    if (choice > 0 && choice <= sourceList.getSize()) {
        selectedProperty = sourceList[choice - 1];
        originalIndex = choice - 1;
        sourceList.remove(originalIndex);
        targetList.push_back(selectedProperty);
        wasExecuted = true;
    }
}

void SelectPropertyCommand::undo() {
    if (wasExecuted) {
        // Remove from target list
        for (size_t i = 0; i < targetList.getSize(); i++) {
            if (targetList[i] == selectedProperty) {
                targetList.remove(i);
                break;
            }
        }
        //add back to source list
        if (originalIndex != -1) {
            sourceList.insert(selectedProperty, originalIndex);
        }
        wasExecuted = false;
    }
}

GameCommand* SelectPropertyCommand::clone() const {
    return new SelectPropertyCommand(*this);
}

Property* SelectPropertyCommand::getSelectedProperty() const {
    return selectedProperty;
}