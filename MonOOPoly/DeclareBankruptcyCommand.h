#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Property.h"
#include "Vector.hpp"

class DeclareBankruptcyCommand : public GameCommand {
    Player& bankruptPlayer;
    const Vector<Property*>& allGameProperties;
    Vector<Property*> previouslyOwnedProperties;

public:
    DeclareBankruptcyCommand(Player& player, const Vector<Property*>& allProperties);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override {
        return "Declare Bankruptcy";
    }
};