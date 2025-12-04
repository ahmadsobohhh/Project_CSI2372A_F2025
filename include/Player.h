#pragma once

#include "Enums.h"
#include "Rubis.h"

#include <iosfwd>
#include <string>

class Player {
public:
    // Parameters: name (std::string), side (Side). Constructs a seated player entity.
    Player(std::string name, Side side);

    // No parameters. Returns the immutable player name string.
    const std::string& getName() const;
    // Parameters: active (bool). Marks player active/inactive for the current round.
    void setActive(bool active);
    // No parameters. Returns whether the player is currently active in the round.
    bool isActive() const;
    // No parameters. Returns the total number of rubies accumulated by the player.
    int getNRubies() const;
    // Parameters: rubis (const Rubis&). Adds 1-4 to the player score.
    void addRubis(const Rubis& rubis);
    // Parameters: endOfGame (bool). Toggles whether printing shows seat info or rubies.
    void setDisplayMode(bool endOfGame);
    // No parameters. Returns the side (top/bottom/left/right) assigned to this player.
    Side getSide() const;
    // Parameters: side (Side). Updates which side this player represents on the board.
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
