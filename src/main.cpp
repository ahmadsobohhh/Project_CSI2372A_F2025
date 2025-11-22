#include "CardDeck.h"
#include "Game.h"
#include "RubisDeck.h"
#include "Rules.h"

#include <iostream>

int main() {
    try {
        CardDeck& cardDeck = CardDeck::make_CardDeck();
        cardDeck.reset();
        cardDeck.shuffle();

        GameOptions options;
        Game game(cardDeck, options);

        RubisDeck& rubisDeck = RubisDeck::make_RubisDeck();
        rubisDeck.reset();
        rubisDeck.shuffle();

        Rules rules(options.rulesMode == RulesMode::Expert);

        std::cout << "Memoarrr! engine initialized. Add gameplay logic next." << std::endl;
        std::cout << game;
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
