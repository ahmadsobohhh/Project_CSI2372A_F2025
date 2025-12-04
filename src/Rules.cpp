// Rules implementation: validates matches and detects round/game termination.
#include "Rules.h"

#include <algorithm>
#include <stdexcept>

Rules::Rules(bool expertRules) : m_useExpertRules(expertRules) {}

bool Rules::isValid(const Game& game) const {
    const Card* previous = game.getPreviousCard();
    const Card* current = game.getCurrentCard();
    if (current == nullptr) {
        return false;
    }
    if (previous == nullptr) {
        return true;
    }
    FaceAnimal prevAnimal = static_cast<FaceAnimal>(*previous);
    FaceAnimal currAnimal = static_cast<FaceAnimal>(*current);
    FaceBackground prevBackground = static_cast<FaceBackground>(*previous);
    FaceBackground currBackground = static_cast<FaceBackground>(*current);
    return prevAnimal == currAnimal || prevBackground == currBackground;
}

bool Rules::gameOver(const Game& game) const {
    return game.getRound() >= 7;
}

bool Rules::roundOver(const Game& game) const {
    const auto& players = game.players();
    return std::count_if(players.begin(), players.end(), [](const Player& player) { return player.isActive(); }) <= 1;
}

const Player& Rules::getNextPlayer(const Game& game) const {
    const auto& players = game.players();
    if (players.empty()) {
        throw std::runtime_error("No players available");
    }
    std::size_t start = game.currentPlayerIndex();
    for (std::size_t offset = 1; offset <= players.size(); ++offset) {
        std::size_t index = (start + offset) % players.size();
        if (players[index].isActive()) {
            return players[index];
        }
    }
    return players[start];
}
