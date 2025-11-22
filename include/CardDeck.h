#pragma once

#include "Card.h"
#include "DeckFactory.h"

class CardDeck : public DeckFactory<Card> {
public:
    static CardDeck& make_CardDeck();

    void reset();

private:
    CardDeck();
    void build();
};
