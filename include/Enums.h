#pragma once

#include <cstddef>
#include <string>
#include <vector>

// Enumerations representing animals, backgrounds, seating sides, and board axes.
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

// Parameters: animal (FaceAnimal). Returns std::string name (e.g., "Crab").
std::string to_string(FaceAnimal animal);
// Parameters: background (FaceBackground). Returns std::string of its colour.
std::string to_string(FaceBackground background);
// Parameters: side (Side). Returns lowercase description of seating side.
std::string to_string(Side side);
// Parameters: background (FaceBackground). Returns the char symbol (r/g/p/b/y).
char background_symbol(FaceBackground background);
// Parameters: animal (FaceAnimal). Returns capital letter symbol.
char animal_symbol(FaceAnimal animal);
// Parameters: letter (Letter). Returns board letter label (A-E).
char letter_symbol(Letter letter);
// Parameters: number (Number). Returns board number label (1-5).
char number_symbol(Number number);
// Parameters: letter (Letter). Returns zero-based row index.
std::size_t to_index(Letter letter);
// Parameters: number (Number). Returns zero-based column index.
std::size_t to_index(Number number);
// Parameters: origin (const Position&). Returns vector of orthogonal neighbour positions.
std::vector<Position> orthogonal_neighbours(const Position& origin);
// Parameters: lhs (const Position&), rhs (const Position&). Returns true if orthogonally adjacent.
bool is_adjacent(const Position& lhs, const Position& rhs);
// Parameters: lhs/rhs (const Position&). Returns true if both coordinates match.
bool operator==(const Position& lhs, const Position& rhs);
// Parameters: lhs/rhs (const Position&). Returns true if positions differ.
bool operator!=(const Position& lhs, const Position& rhs);
