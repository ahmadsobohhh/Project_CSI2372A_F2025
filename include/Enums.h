#pragma once

#include <cstddef>
#include <string>
#include <vector>

enum class FaceAnimal { Crab, Penguin, Octopus, Turtle, Walrus };
enum class FaceBackground { Red, Green, Purple, Blue, Yellow };
enum class Side { Top, Bottom, Left, Right };
enum class Letter { A = 0, B, C, D, E };
enum class Number { One = 0, Two, Three, Four, Five };

enum class DisplayMode { Base, Expert };

enum class RulesMode { Base, Expert };

struct Position {
    Letter letter;
    Number number;
};

std::string to_string(FaceAnimal animal);
std::string to_string(FaceBackground background);
std::string to_string(Side side);
char background_symbol(FaceBackground background);
char animal_symbol(FaceAnimal animal);
char letter_symbol(Letter letter);
char number_symbol(Number number);
std::size_t to_index(Letter letter);
std::size_t to_index(Number number);
std::vector<Position> orthogonal_neighbours(const Position& origin);
bool is_adjacent(const Position& lhs, const Position& rhs);
bool operator==(const Position& lhs, const Position& rhs);
bool operator!=(const Position& lhs, const Position& rhs);
