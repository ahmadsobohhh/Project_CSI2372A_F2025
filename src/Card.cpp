#include "Card.h"

#include <ostream>
#include <stdexcept>

Card::Card(FaceAnimal animal, FaceBackground background)
    : m_animal(animal), m_background(background) {}

std::size_t Card::getNRows() const {
    return 3;
}

std::string Card::operator()(std::size_t row) const {
    if (row >= getNRows()) {
        throw std::out_of_range("Card row");
    }
    const char bg = background_symbol(m_background);
    if (row == 1) {
        std::string middle(1, bg);
        middle += animal_symbol(m_animal);
        middle += bg;
        return middle;
    }
    return std::string(3, bg);
}

Card::operator FaceAnimal() const {
    return m_animal;
}

Card::operator FaceBackground() const {
    return m_background;
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    for (std::size_t row = 0; row < card.getNRows(); ++row) {
        os << card(row);
        if (row + 1 < card.getNRows()) {
            os << '\n';
        }
    }
    return os;
}
