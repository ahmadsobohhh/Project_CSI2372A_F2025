#pragma once

#include "Game.h"

// Implements the core rule checks (matching logic, round/game termination, turn order).
class Rules {
public:
    explicit Rules(bool expertRules);

    // Parameters: game (const Game&). Returns true when current card matches previous.
    bool isValid(const Game& game) const;
    // Parameters: game (const Game&). Returns true after seven rounds are complete.
    bool gameOver(const Game& game) const;
    // Parameters: game (const Game&). Returns true if <= 1 active players remain.
    bool roundOver(const Game& game) const;
    // Parameters: game (const Game&). Returns reference to next active player.
    const Player& getNextPlayer(const Game& game) const;

private:
    bool m_useExpertRules{false};
};
