#include "Player.h"

#include <ostream>
#include <utility>

Player::Player(std::string name, Side side)
    : m_name(std::move(name)), m_side(side) {}

const std::string& Player::getName() const {
    return m_name;
}

void Player::setActive(bool active) {
    m_active = active;
}

bool Player::isActive() const {
    return m_active;
}

int Player::getNRubies() const {
    return m_rubies;
}

void Player::addRubis(const Rubis& rubis) {
    m_rubies += static_cast<int>(rubis);
}

void Player::setDisplayMode(bool endOfGame) {
    m_endOfGame = endOfGame;
}

Side Player::getSide() const {
    return m_side;
}

void Player::setSide(Side side) {
    m_side = side;
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << player.m_name << ": ";
    if (player.m_endOfGame) {
        os << player.m_rubies << (player.m_rubies == 1 ? " ruby" : " rubies");
    } else {
        os << to_string(player.m_side) << (player.m_active ? " (active)" : " (inactive)");
    }
    return os;
}
