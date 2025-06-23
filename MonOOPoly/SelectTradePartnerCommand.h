#pragma once
#include "GameCommand.h"
#include "Player.h"
#include "Vector.hpp"

class SelectTradePartnerCommand : public GameCommand {
    Vector<Player*>& availablePartners;
    Player*& currentSelection;
    Player* previousSelection;
    Player* selectedPartner;
    bool wasExecuted;

public:
    SelectTradePartnerCommand(Vector<Player*>& availablePartners,
        Player*& currentSelection);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;

    Player* getSelectedPartner() const;
    const char* getDescription() const override {
        return "Select trade partner";
    }
};