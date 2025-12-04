#pragma once

#include "Card.h"
#include "DeckFactory.h"

// Singleton deck factory responsible for producing the 25 animal/background cards.
class CardDeck : public DeckFactory<Card> {
public:
    // Returns the single instance used throughout gameplay.
    static CardDeck& make_CardDeck();

    // Rebuilds the deck back to a full 25-card state (used before each game).
    void reset();

private:
    CardDeck();
    // Populates the deck with all combinations of animal/background pairs.
    void build();
};
