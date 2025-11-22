#pragma once

#include "Board.h"
#include "CardDeck.h"
#include "Enums.h"
#include "Player.h"

#include <memory>
#include <vector>

struct GameOptions {
    DisplayMode displayMode{DisplayMode::Base};
    RulesMode rulesMode{RulesMode::Base};
};

class Game {
public:
    Game(DeckFactory<Card>& cardDeck, const GameOptions& options);

    int getRound() const;
    void incrementRound();

    void addPlayer(const Player& player);
    Player& getPlayer(Side side);
    const std::vector<Player>& players() const;
    std::vector<Player>& players();

    const Card* getPreviousCard() const;
    const Card* getCurrentCard() const;
    void setCurrentCard(const Card* card);

    Card* getCard(const Letter& letter, const Number& number);
    void setCard(const Letter& letter, const Number& number, Card* card);

    Board& board();
    const Board& board() const;

    DisplayMode displayMode() const;
    RulesMode rulesMode() const;

    void setCurrentPlayerIndex(std::size_t index);
    std::size_t currentPlayerIndex() const;
    Player& currentPlayer();

    void resetTurnPointers();

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

private:
    std::vector<std::unique_ptr<Card>> m_cardStorage;
    Board m_board;
    std::vector<Player> m_players;
    int m_round{0};
    const Card* m_previousCard{nullptr};
    const Card* m_currentCard{nullptr};
    GameOptions m_options;
    std::size_t m_currentPlayer{0};
};

std::ostream& operator<<(std::ostream& os, const Game& game);
