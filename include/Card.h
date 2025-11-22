#pragma once

#include "Enums.h"

#include <array>
#include <iosfwd>
#include <string>

class CardDeck;

class Card {
public:
    Card(const Card&) = delete;
    Card& operator=(const Card&) = delete;
    Card(Card&&) noexcept = default;
    Card& operator=(Card&&) noexcept = default;

    std::size_t getNRows() const;
    std::string operator()(std::size_t row) const;

    operator FaceAnimal() const;
    operator FaceBackground() const;

private:
    Card(FaceAnimal animal, FaceBackground background);

    FaceAnimal m_animal;
    FaceBackground m_background;

    friend class CardDeck;
    friend std::ostream& operator<<(std::ostream&, const Card&);
};

std::ostream& operator<<(std::ostream& os, const Card& card);
