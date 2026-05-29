#pragma once

#include "core/Hand.h"
#include "core/Tile.h"

class Analyzer {
public:
    virtual ~Analyzer() = default;
    virtual int analyze(const Hand& hand, Suit queSuit) const = 0;
};
