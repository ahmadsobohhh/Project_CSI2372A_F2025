#pragma once

#include "Card.h"
#include "DeckFactory.h"
#include "Enums.h"
#include "Exceptions.h"

#include <array>
#include <iosfwd>
#include <memory>
#include <utility>
#include <vector>

class Board {
public:
    // Parameters: deck (DeckFactory<Card>&), storage (vector<unique_ptr<Card>>&). Builds grid from deck.
    Board(DeckFactory<Card>& deck, std::vector<std::unique_ptr<Card>>& storage);

    // Parameters: letter (Letter), number (Number). Returns true if that slot is face up.
    bool isFaceUp(const Letter& letter, const Number& number) const;
    // Parameters: letter (Letter), number (Number). Returns false when already face up.
    bool turnFaceUp(const Letter& letter, const Number& number);
    // Parameters: letter (Letter), number (Number). Returns false when already face down.
    bool turnFaceDown(const Letter& letter, const Number& number);
    // Parameters: letter (Letter), number (Number). Returns Card* pointing at the slot.
    Card* getCard(const Letter& letter, const Number& number);
    // Parameters: letter (Letter), number (Number), card (Card*). Replaces stored pointer.
    void setCard(const Letter& letter, const Number& number, Card* card);
    // No parameters. Turns every occupied slot face down and clears block flags.
    void allFacesDown();
    // Parameters: letter (Letter), number (Number). Returns true if marked blocked.
    bool isBlocked(const Letter& letter, const Number& number) const;
    // Parameters: letter (Letter), number (Number), blocked (bool). Updates blocked state.
    void setBlocked(const Letter& letter, const Number& number, bool blocked);
    // No parameters. Clears all blocked flags on the grid.
    void clearBlocked();
    // No parameters. Returns true if at least one card remains face down.
    bool hasFaceDownCards() const;
    // Parameters: first (Position), second (Position). Swaps underlying cells.
    void swapCells(const Position& first, const Position& second);

    // No parameters. Returns vector pairs of Position and Card* for face-up cards.
    std::vector<std::pair<Position, const Card*>> faceUpCards() const;

    // Streams either the full grid or empty-row placeholders depending on state.
    friend std::ostream& operator<<(std::ostream& os, const Board& board);

private:
    struct Cell {
        Card* card{nullptr};
        bool faceUp{false};
        bool blocked{false};
    };

    std::array<std::array<Cell, 5>, 5> m_grid{};
    std::vector<std::unique_ptr<Card>>& m_cardStorage;

    static bool isCenter(Letter letter, Number number);
    Cell& at(const Letter& letter, const Number& number);
    const Cell& at(const Letter& letter, const Number& number) const;
};

std::ostream& operator<<(std::ostream& os, const Board& board);
