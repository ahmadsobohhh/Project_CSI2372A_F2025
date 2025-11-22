#include "Game.h"

#include <algorithm>
#include <ostream>
#include <stdexcept>

Game::Game(DeckFactory<Card>& cardDeck, const GameOptions& options)
    : m_cardStorage(), m_board(cardDeck, m_cardStorage), m_options(options) {}

int Game::getRound() const {
    return m_round;
}

void Game::incrementRound() {
    ++m_round;
}

void Game::addPlayer(const Player& player) {
    m_players.push_back(player);
}

Player& Game::getPlayer(Side side) {
    auto it = std::find_if(m_players.begin(), m_players.end(), [side](const Player& p) { return p.getSide() == side; });
    if (it == m_players.end()) {
        throw std::runtime_error("Player with specified side not found");
    }
    return *it;
}

const std::vector<Player>& Game::players() const {
    return m_players;
}

std::vector<Player>& Game::players() {
    return m_players;
}

const Card* Game::getPreviousCard() const {
    return m_previousCard;
}

const Card* Game::getCurrentCard() const {
    return m_currentCard;
}

void Game::setCurrentCard(const Card* card) {
    m_previousCard = m_currentCard;
    m_currentCard = card;
}

Card* Game::getCard(const Letter& letter, const Number& number) {
    return m_board.getCard(letter, number);
}

void Game::setCard(const Letter& letter, const Number& number, Card* card) {
    m_board.setCard(letter, number, card);
}

Board& Game::board() {
    return m_board;
}

const Board& Game::board() const {
    return m_board;
}

DisplayMode Game::displayMode() const {
    return m_options.displayMode;
}

RulesMode Game::rulesMode() const {
    return m_options.rulesMode;
}

void Game::setCurrentPlayerIndex(std::size_t index) {
    m_currentPlayer = index;
}

std::size_t Game::currentPlayerIndex() const {
    return m_currentPlayer;
}

Player& Game::currentPlayer() {
    return m_players.at(m_currentPlayer);
}

void Game::resetTurnPointers() {
    m_previousCard = nullptr;
    m_currentCard = nullptr;
    m_currentPlayer = 0;
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    if (game.m_options.displayMode == DisplayMode::Base) {
        os << game.m_board;
    } else {
        const auto faceUp = game.m_board.faceUpCards();
        if (faceUp.empty()) {
            os << "No cards are currently face up." << '\n';
        } else {
            for (std::size_t row = 0; row < 3; ++row) {
                for (std::size_t idx = 0; idx < faceUp.size(); ++idx) {
                    if (idx > 0) {
                        os << ' ';
                    }
                    os << faceUp[idx].second->operator()(row);
                }
                os << '\n';
            }
            for (std::size_t idx = 0; idx < faceUp.size(); ++idx) {
                if (idx > 0) {
                    os << ' ';
                }
                os << letter_symbol(faceUp[idx].first.letter) << number_symbol(faceUp[idx].first.number);
            }
            os << '\n';
        }
    }

    for (const auto& player : game.m_players) {
        os << player << '\n';
    }
    return os;
}
