#pragma once

#include "DeckFactory.h"
#include "Rubis.h"

// Singleton deck factory for distributing random ruby rewards after each round.
class RubisDeck : public DeckFactory<Rubis> {
public:
    // Provides the single RubisDeck instance for the duration of the program.
    static RubisDeck& make_RubisDeck();

    // Rebuilds the deck with the assignment-specified ruby distribution.
    void reset();

private:
    RubisDeck();
    // Pushes the configured counts of 1-4 ruby rewards onto the deck storage.
    void build();
};
