// RubisDeck implementation: builds/reset the deck with assignment ruby counts.
#include "RubisDeck.h"

#include <memory>

RubisDeck& RubisDeck::make_RubisDeck() {
    static RubisDeck deck;
    return deck;
}

RubisDeck::RubisDeck() {
    build();
}

void RubisDeck::reset() {
    clear();
    build();
}

void RubisDeck::build() {
    auto push_value = [this](int value, int count) {
        while (count-- > 0) {
            push(std::unique_ptr<Rubis>(new Rubis(value)));
        }
    };

    push_value(1, 3);
    push_value(2, 2);
    push_value(3, 1);
    push_value(4, 1);
}
