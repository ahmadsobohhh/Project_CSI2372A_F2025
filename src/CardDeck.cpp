#include "CardDeck.h"

#include <memory>

CardDeck& CardDeck::make_CardDeck() {
    static CardDeck deck;
    return deck;
}

CardDeck::CardDeck() {
    build();
}

void CardDeck::reset() {
    clear();
    build();
}

void CardDeck::build() {
    for (int animal = 0; animal < 5; ++animal) {
        for (int background = 0; background < 5; ++background) {
            push(std::unique_ptr<Card>(
                new Card(static_cast<FaceAnimal>(animal), static_cast<FaceBackground>(background))));
        }
    }
}
