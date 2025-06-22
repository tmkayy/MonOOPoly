#pragma once
#include "GameCommand.h"
#include "Property.h"
#include "Vector.hpp"
#include "Player.h"

class SelectPropertyCommand : public GameCommand {
    Player& player;
    Vector<Property*>& sourceList;
    Vector<Property*>& targetList;
    Property*& selectedProperty;
    size_t originalIndex;
    bool wasExecuted;

public:
    SelectPropertyCommand(Player& player,
        Vector<Property*>& sourceList,
        Vector<Property*>& targetList,
        Property*& property);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;

    Property* getSelectedProperty() const;
    const char* getDescription() const override { return "Select property"; }
};