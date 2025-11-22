#pragma once

#include "Enums.h"
#include "Rubis.h"

#include <iosfwd>
#include <string>

class Player {
public:
    Player(std::string name, Side side);

    const std::string& getName() const;
    void setActive(bool active);
    bool isActive() const;
    int getNRubies() const;
    void addRubis(const Rubis& rubis);
    void setDisplayMode(bool endOfGame);
    Side getSide() const;
    void setSide(Side side);

private:
    std::string m_name;
    Side m_side;
    bool m_active{true};
    bool m_endOfGame{false};
    int m_rubies{0};

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};

std::ostream& operator<<(std::ostream& os, const Player& player);
