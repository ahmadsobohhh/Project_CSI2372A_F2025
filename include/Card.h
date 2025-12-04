#pragma once

#include "Enums.h"

#include <array>
#include <iosfwd>
#include <string>

class CardDeck;

// Represents a 3x3 ASCII depiction of a Memoarrr! card with animal + background.
class Card {
public:
    Card(const Card&) = delete;
    Card& operator=(const Card&) = delete;
    Card(Card&&) noexcept = default;
    Card& operator=(Card&&) noexcept = default;

    // Number of drawable rows (always 3) for board rendering loops.
    std::size_t getNRows() const;
    // Returns the ASCII row string at the requested index (0-2).
    std::string operator()(std::size_t row) const;

    // Implicit conversion exposing the animal printed on this card.
    operator FaceAnimal() const;
    // Implicit conversion exposing the card background colour.
    operator FaceBackground() const;

private:
    Card(FaceAnimal animal, FaceBackground background);

    FaceAnimal m_animal;
    FaceBackground m_background;

    friend class CardDeck;
    friend std::ostream& operator<<(std::ostream&, const Card&);
};

std::ostream& operator<<(std::ostream& os, const Card& card);
