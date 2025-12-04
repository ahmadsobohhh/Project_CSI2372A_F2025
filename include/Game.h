#pragma once

#include "Board.h"
#include "CardDeck.h"
#include "Enums.h"
#include "Player.h"

#include <memory>
#include <vector>

// Configuration flags chosen at startup indicating display and rules variants.
struct GameOptions {
    DisplayMode displayMode{DisplayMode::Base};
    RulesMode rulesMode{RulesMode::Base};
};

class Game {
public:
    // Parameters: cardDeck (DeckFactory<Card>&), options (GameOptions). Owns board/players.
    Game(DeckFactory<Card>& cardDeck, const GameOptions& options);

    // No parameters. Returns current round number (int).
    int getRound() const;
    // No parameters. Increments the internal round counter.
    void incrementRound();

    // Parameters: player (const Player&). Copies player into roster.
    void addPlayer(const Player& player);
    // Parameters: side (Side). Returns reference to player seated on that side.
    Player& getPlayer(Side side);
    // No parameters. Returns const view of player vector.
    const std::vector<Player>& players() const;
    // No parameters. Returns mutable view of player vector.
    std::vector<Player>& players();

    // No parameters. Returns pointer to previous/ current cards for rule checks.
    const Card* getPreviousCard() const;
    const Card* getCurrentCard() const;
    // Parameters: card (const Card*). Updates current and previous pointers.
    void setCurrentCard(const Card* card);

    // Parameters: letter (Letter), number (Number). Forwards to Board::getCard.
    Card* getCard(const Letter& letter, const Number& number);
    // Parameters: letter (Letter), number (Number), card (Card*). Forwards to Board::setCard.
    void setCard(const Letter& letter, const Number& number, Card* card);

    // No parameters. Returns references to the owned board instance.
    Board& board();
    const Board& board() const;

    // No parameters. Returns the display and rules modes that were selected.
    DisplayMode displayMode() const;
    RulesMode rulesMode() const;

    // Parameters: index (std::size_t). Assigns/reads the active player index.
    void setCurrentPlayerIndex(std::size_t index);
    std::size_t currentPlayerIndex() const;
    // No parameters. Returns Player& for currently tracked player.
    Player& currentPlayer();

    // No parameters. Clears previous/current card pointers and player index.
    void resetTurnPointers();

    // Prints the board (or expert row) followed by player summaries.
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
