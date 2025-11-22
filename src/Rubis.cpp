#include "Rubis.h"

#include <ostream>

Rubis::Rubis(int value) : m_value(value) {}

Rubis::operator int() const {
    return m_value;
}

std::ostream& operator<<(std::ostream& os, const Rubis& rubis) {
    os << rubis.m_value << (rubis.m_value == 1 ? " ruby" : " rubies");
    return os;
}
