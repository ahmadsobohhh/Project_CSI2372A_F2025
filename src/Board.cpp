#include "Board.h"

#include <ostream>
#include <string>
#include <utility>

namespace {
std::string face_down_row() {
    return std::string("zzz");
}

std::string empty_row() {
    return std::string("   ");
}
}

Board::Board(DeckFactory<Card>& deck, std::vector<std::unique_ptr<Card>>& storage)
    : m_cardStorage(storage) {
    for (std::size_t row = 0; row < 5; ++row) {
        for (std::size_t col = 0; col < 5; ++col) {
            Letter letter = static_cast<Letter>(row);
            Number number = static_cast<Number>(col);
            if (isCenter(letter, number)) {
                continue;
            }
            Card* next = deck.getNext();
            if (!next) {
                throw NoMoreCards("Not enough cards to populate the board");
            }
            m_cardStorage.emplace_back(next);
            m_grid[row][col].card = m_cardStorage.back().get();
        }
    }
}

bool Board::isFaceUp(const Letter& letter, const Number& number) const {
    return at(letter, number).faceUp;
}

bool Board::turnFaceUp(const Letter& letter, const Number& number) {
    Cell& cell = at(letter, number);
    if (cell.card == nullptr) {
        throw OutOfRange("Empty position");
    }
    if (cell.faceUp) {
        return false;
    }
    cell.faceUp = true;
    return true;
}

bool Board::turnFaceDown(const Letter& letter, const Number& number) {
    Cell& cell = at(letter, number);
    if (cell.card == nullptr) {
        throw OutOfRange("Empty position");
    }
    if (!cell.faceUp) {
        return false;
    }
    cell.faceUp = false;
    return true;
}

Card* Board::getCard(const Letter& letter, const Number& number) {
    return at(letter, number).card;
}

void Board::setCard(const Letter& letter, const Number& number, Card* card) {
    Cell& cell = at(letter, number);
    cell.card = card;
}

void Board::allFacesDown() {
    for (auto& row : m_grid) {
        for (auto& cell : row) {
            cell.faceUp = false;
            cell.blocked = false;
        }
    }
}

bool Board::isBlocked(const Letter& letter, const Number& number) const {
    return at(letter, number).blocked;
}

void Board::setBlocked(const Letter& letter, const Number& number, bool blocked) {
    at(letter, number).blocked = blocked;
}

void Board::clearBlocked() {
    for (auto& row : m_grid) {
        for (auto& cell : row) {
            cell.blocked = false;
        }
    }
}

bool Board::hasFaceDownCards() const {
    for (const auto& row : m_grid) {
        for (const auto& cell : row) {
            if (cell.card && !cell.faceUp) {
                return true;
            }
        }
    }
    return false;
}

void Board::swapCells(const Position& first, const Position& second) {
    Cell& a = at(first.letter, first.number);
    Cell& b = at(second.letter, second.number);
    std::swap(a, b);
}

std::vector<std::pair<Position, const Card*>> Board::faceUpCards() const {
    std::vector<std::pair<Position, const Card*>> result;
    for (std::size_t row = 0; row < 5; ++row) {
        for (std::size_t col = 0; col < 5; ++col) {
            const Cell& cell = m_grid[row][col];
            if (cell.card && cell.faceUp) {
                result.push_back({Position{static_cast<Letter>(row), static_cast<Number>(col)}, cell.card});
            }
        }
    }
    return result;
}

bool Board::isCenter(Letter letter, Number number) {
    return letter == Letter::C && number == Number::Three;
}

Board::Cell& Board::at(const Letter& letter, const Number& number) {
    if (isCenter(letter, number)) {
        throw OutOfRange("Center position is empty");
    }
    std::size_t row = to_index(letter);
    std::size_t col = to_index(number);
    if (row >= m_grid.size() || col >= m_grid[row].size()) {
        throw OutOfRange("Position outside of board");
    }
    return m_grid[row][col];
}

const Board::Cell& Board::at(const Letter& letter, const Number& number) const {
    if (isCenter(letter, number)) {
        throw OutOfRange("Center position is empty");
    }
    std::size_t row = to_index(letter);
    std::size_t col = to_index(number);
    if (row >= m_grid.size() || col >= m_grid[row].size()) {
        throw OutOfRange("Position outside of board");
    }
    return m_grid[row][col];
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
    for (std::size_t row = 0; row < 5; ++row) {
        Letter letter = static_cast<Letter>(row);
        for (std::size_t inner = 0; inner < 3; ++inner) {
            os << (inner == 1 ? letter_symbol(letter) : ' ') << ' ';
            for (std::size_t col = 0; col < 5; ++col) {
                Number number = static_cast<Number>(col);
                if (col > 0) {
                    os << ' ';
                }
                if (Board::isCenter(letter, number)) {
                    os << empty_row();
                    continue;
                }
                const auto& cell = board.at(letter, number);
                if (!cell.card) {
                    os << empty_row();
                } else if (!cell.faceUp) {
                    os << face_down_row();
                } else {
                    os << (*cell.card)(inner);
                }
            }
            os << '\n';
        }
    }

    os << "  ";
    for (std::size_t col = 0; col < 5; ++col) {
        if (col > 0) {
            os << ' ';
        }
        os << ' ' << number_symbol(static_cast<Number>(col)) << ' ';
    }
    os << '\n';
    return os;
}
