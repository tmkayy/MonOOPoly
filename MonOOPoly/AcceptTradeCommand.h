#pragma once
#include "GameCommand.h"
#include "Bank.h"
#include "Trade.h"

class AcceptTradeCommand : public GameCommand {
    Trade& trade;
    bool wasAccepted;

public:
    explicit AcceptTradeCommand(Trade& trade);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override {
        return "Accept Trade";
    }
};