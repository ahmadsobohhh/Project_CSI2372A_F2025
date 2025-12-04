// Utility helpers that convert between enums, symbols, and board coordinates.
#include "Enums.h"

#include <array>
#include <stdexcept>

namespace {
// Lookup tables for mapping enum values to human-readable strings and symbols.
constexpr std::array<const char*, 5> kAnimalNames{"Crab", "Penguin", "Octopus", "Turtle", "Walrus"};
constexpr std::array<const char*, 5> kBackgroundNames{"Red", "Green", "Purple", "Blue", "Yellow"};
constexpr std::array<const char*, 4> kSideNames{"top", "bottom", "left", "right"};
constexpr std::array<char, 5> kBackgroundSymbols{'r', 'g', 'p', 'b', 'y'};
constexpr std::array<char, 5> kAnimalSymbols{'C', 'P', 'O', 'T', 'W'};
constexpr std::array<char, 5> kLetterSymbols{'A', 'B', 'C', 'D', 'E'};
constexpr std::array<char, 5> kNumberSymbols{'1', '2', '3', '4', '5'};
}

std::string to_string(FaceAnimal animal) {
    return kAnimalNames.at(static_cast<std::size_t>(animal));
}

std::string to_string(FaceBackground background) {
    return kBackgroundNames.at(static_cast<std::size_t>(background));
}

std::string to_string(Side side) {
    return kSideNames.at(static_cast<std::size_t>(side));
}

char background_symbol(FaceBackground background) {
    return kBackgroundSymbols.at(static_cast<std::size_t>(background));
}

char animal_symbol(FaceAnimal animal) {
    return kAnimalSymbols.at(static_cast<std::size_t>(animal));
}

char letter_symbol(Letter letter) {
    return kLetterSymbols.at(static_cast<std::size_t>(letter));
}

char number_symbol(Number number) {
    return kNumberSymbols.at(static_cast<std::size_t>(number));
}

std::size_t to_index(Letter letter) {
    return static_cast<std::size_t>(letter);
}

std::size_t to_index(Number number) {
    return static_cast<std::size_t>(number);
}

std::vector<Position> orthogonal_neighbours(const Position& origin) {
    std::vector<Position> neighbours;
    std::size_t row = to_index(origin.letter);
    std::size_t col = to_index(origin.number);

    if (row > 0) {
        neighbours.push_back(Position{static_cast<Letter>(row - 1), origin.number});
    }
    if (row + 1 < 5) {
        neighbours.push_back(Position{static_cast<Letter>(row + 1), origin.number});
    }
    if (col > 0) {
        neighbours.push_back(Position{origin.letter, static_cast<Number>(col - 1)});
    }
    if (col + 1 < 5) {
        neighbours.push_back(Position{origin.letter, static_cast<Number>(col + 1)});
    }
    return neighbours;
}

bool is_adjacent(const Position& lhs, const Position& rhs) {
    auto neighbours = orthogonal_neighbours(lhs);
    for (const auto& pos : neighbours) {
        if (pos == rhs) {
            return true;
        }
    }
    return false;
}

bool operator==(const Position& lhs, const Position& rhs) {
    return lhs.letter == rhs.letter && lhs.number == rhs.number;
}

bool operator!=(const Position& lhs, const Position& rhs) {
    return !(lhs == rhs);
}
