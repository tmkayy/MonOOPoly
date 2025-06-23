#include "SelectPropertyCommand.h"
#include <iostream>

SelectPropertyCommand::SelectPropertyCommand(Player& player,
    Vector<Property*>& sourceList,
    Vector<Property*>& targetList,
    Property*& property)
    : player(player), sourceList(sourceList), targetList(targetList),
    selectedProperty(property), originalIndex(-1), wasExecuted(false) {
}

void SelectPropertyCommand::execute() {
    if (sourceList.isEmpty()) {
        std::cout << Red << "No properties available to select." << Reset << std::endl;
        return;
    }

    std::cout << Cyan << "Select a property:" << Reset << std::endl;
    for (size_t i = 0; i < sourceList.getSize(); i++) {
        std::cout << Yellow << i + 1 << ". " << sourceList[i]->getName() << Reset
            << " (" << colorToString(sourceList[i]->getColor()) << ")" << std::endl;
    }
    std::cout << Yellow << "0. Cancel" << Reset << std::endl
        << "Enter choice (0-" << sourceList.getSize() << "): ";

    size_t choice;
    std::cin >> choice;

    if (choice > 0 && choice <= sourceList.getSize()) {
        selectedProperty = sourceList[choice - 1];
        originalIndex = choice - 1;
        sourceList.remove(originalIndex);
        targetList.push_back(selectedProperty);
        wasExecuted = true;
        std::cout << Green << "Property selected: " << selectedProperty->getName() << Reset << std::endl;
    }
    else if (choice != 0) {
        std::cout << Red << "Invalid choice." << Reset << std::endl;
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
        std::cout << Yellow << "Property selection undone." << Reset << std::endl;
    }
}

GameCommand* SelectPropertyCommand::clone() const {
    return new SelectPropertyCommand(*this);
}

Property* SelectPropertyCommand::getSelectedProperty() const {
    return selectedProperty;
}