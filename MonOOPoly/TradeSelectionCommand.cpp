#include "TradeSelectionCommand.h"

TradeSelectionCommand::TradeSelectionCommand(Player& selector,
    Vector<Property*>& sourcePool,
    Vector<Property*>& selectionPool,
    Property* property)
    : selector(selector), sourcePool(sourcePool),
    selectionPool(selectionPool), property(property),
    selected(false) {
}

void TradeSelectionCommand::execute() {
    if (!property || property->getOwner() != &selector) return;

    for (size_t i = 0; i < sourcePool.getSize(); i++) {
        if (sourcePool[i] == property) {
            sourcePool.remove(i);
            selectionPool.push_back(property);
            selected = true;
            return;
        }
    }
}

void TradeSelectionCommand::undo() {
    if (selected) {
        for (size_t i = 0; i < selectionPool.getSize(); i++) {
            if (selectionPool[i] == property) {
                selectionPool.remove(i);
                sourcePool.push_back(property);
                return;
            }
        }
    }
}

GameCommand* TradeSelectionCommand::clone() const {
    return new TradeSelectionCommand(*this);
}