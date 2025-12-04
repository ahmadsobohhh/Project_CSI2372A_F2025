#pragma once

#include <iosfwd>

class RubisDeck;

// Represents a reward card worth 1-4 rubies that can be printed or summed.
class Rubis {
public:
    Rubis(const Rubis&) = delete;
    Rubis& operator=(const Rubis&) = delete;
    Rubis(Rubis&&) noexcept = default;
    Rubis& operator=(Rubis&&) noexcept = default;

    // No parameters. Returns the stored ruby count as an int via implicit conversion.
    operator int() const;

private:
    explicit Rubis(int value);

    int m_value;

    friend class RubisDeck;
    friend std::ostream& operator<<(std::ostream&, const Rubis&);
};

std::ostream& operator<<(std::ostream& os, const Rubis& rubis);
