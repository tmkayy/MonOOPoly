#pragma once

class GameCommand {
public:
    virtual ~GameCommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual GameCommand* clone() const = 0;
};

