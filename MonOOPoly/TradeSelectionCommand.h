#pragma once
#include "GameCommand.h"
#include "Property.h"
#include "Vector.hpp"

class TradeSelectionCommand : public GameCommand {
    Player& selector;
    Vector<Property*>& sourcePool;
    Vector<Property*>& selectionPool;
    Property* property;
    bool selected;

public:
    TradeSelectionCommand(Player& selector,
        Vector<Property*>& sourcePool,
        Vector<Property*>& selectionPool,
        Property* property);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override { return "Trade selection"; }
};