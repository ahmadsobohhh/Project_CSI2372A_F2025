#pragma once

#include "Game.h"

class Rules {
public:
    explicit Rules(bool expertRules);

    bool isValid(const Game& game) const;
    bool gameOver(const Game& game) const;
    bool roundOver(const Game& game) const;
    const Player& getNextPlayer(const Game& game) const;

private:
    bool m_useExpertRules{false};
};
