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
    Board(DeckFactory<Card>& deck, std::vector<std::unique_ptr<Card>>& storage);

    bool isFaceUp(const Letter& letter, const Number& number) const;
    bool turnFaceUp(const Letter& letter, const Number& number);
    bool turnFaceDown(const Letter& letter, const Number& number);
    Card* getCard(const Letter& letter, const Number& number);
    void setCard(const Letter& letter, const Number& number, Card* card);
    void allFacesDown();
    bool isBlocked(const Letter& letter, const Number& number) const;
    void setBlocked(const Letter& letter, const Number& number, bool blocked);
    void clearBlocked();
    bool hasFaceDownCards() const;
    void swapCells(const Position& first, const Position& second);

    std::vector<std::pair<Position, const Card*>> faceUpCards() const;

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
