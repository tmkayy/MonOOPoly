#pragma once
#include "GameCommand.h"
#include "Trade.h"

class RejectTradeCommand : public GameCommand {
    Trade& trade;
    bool wasPending;

public:
    explicit RejectTradeCommand(Trade& trade);
    void execute() override;
    void undo() override;
    GameCommand* clone() const override;
    const char* getDescription() const override { return "Reject Trade"; }
};