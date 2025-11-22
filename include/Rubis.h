#pragma once

#include <iosfwd>

class RubisDeck;

class Rubis {
public:
    Rubis(const Rubis&) = delete;
    Rubis& operator=(const Rubis&) = delete;
    Rubis(Rubis&&) noexcept = default;
    Rubis& operator=(Rubis&&) noexcept = default;

    operator int() const;

private:
    explicit Rubis(int value);

    int m_value;

    friend class RubisDeck;
    friend std::ostream& operator<<(std::ostream&, const Rubis&);
};

std::ostream& operator<<(std::ostream& os, const Rubis& rubis);
