#pragma once

#include "DeckFactory.h"
#include "Rubis.h"

class RubisDeck : public DeckFactory<Rubis> {
public:
    static RubisDeck& make_RubisDeck();

    void reset();

private:
    RubisDeck();
    void build();
};
