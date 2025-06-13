#include "Monopoly.h"

void Monopoly::executeCommand(GameCommand* command) {
    command->execute();
    commandHistory.push_back(command);
}

void Monopoly::undoLastCommand() {
    if (!commandHistory.isEmpty()) {
        commandHistory.peek()->undo();
        delete commandHistory.peek();
        commandHistory.pop_back();
    }
}

void Monopoly::copyFrom(const Monopoly& other)
{
    for (size_t i = 0; i < other.players.getSize(); i++) {
        players.push_back(new Player(*other.players[i]));
    }

    for (size_t i = 0; i < other.commandHistory.getSize(); i++) {
        commandHistory.push_back(other.commandHistory[i]->clone());
    }

    board = other.board;
    cards = other.cards;
    bank = other.bank;
    d1 = other.d1;
    d2 = other.d2;
}

void Monopoly::moveFrom(Monopoly&& other) noexcept
{
    players = std::move(other.players);
    commandHistory = std::move(other.commandHistory);
    board = std::move(other.board);
    cards = std::move(other.cards);
    bank = std::move(other.bank);
    d1 = std::move(other.d1);
    d2 = std::move(other.d2);

    other.players = Vector<Player*>();
    other.commandHistory = Vector<GameCommand*>();
}

void Monopoly::free() {
    for (size_t i = 0; i < players.getSize(); i++) {
        delete players[i];
    }
    players.clear();

    for (size_t i = 0; i < commandHistory.getSize(); i++) {
        delete commandHistory[i];
    }
    commandHistory.clear();
}

Monopoly::Monopoly()
    : board(), cards(), bank(), d1(), d2() {
}

Monopoly::~Monopoly() noexcept {
    free();
}

Monopoly::Monopoly(const Monopoly& other) {
    copyFrom(other);
}

Monopoly& Monopoly::operator=(const Monopoly& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

Monopoly::Monopoly(Monopoly&& other) noexcept {
    moveFrom(std::move(other));
}

Monopoly& Monopoly::operator=(Monopoly&& other) noexcept {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

void Monopoly::startGame() {
    //initialize game state
    cards.shuffle();
    //other initialization
}

void Monopoly::endGame() {
    free();
}