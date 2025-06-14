#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Property.h"
#include "Bank.h"
#include "Vector.hpp"
#include "Cottage.h"
#include "Castle.h"

class DeclareBankruptcyCommand : public GameCommand {
    Player& bankruptPlayer;
    const Vector<Property*>& allGameProperties;
    Vector<Property*> previouslyOwnedProperties;
    int originalMoney;
    bool couldHavePaid;

public:
    DeclareBankruptcyCommand(Player& player, const Vector<Property*>& allProperties);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
};