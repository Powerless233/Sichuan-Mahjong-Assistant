#pragma once

#include "core/Hand.h"
#include "core/ShantenCalculator.h"
#include "core/Tile.h"

#include <string>
#include <vector>

struct EfficiencyResult {
    int acceptingTiles = 0;
    int acceptingTypes = 0;
    std::vector<std::string> acceptingLabels;
};

class EfficiencyCalculator {
public:
    explicit EfficiencyCalculator(const ShantenCalculator& shantenCalculator);

    EfficiencyResult analyze(const Hand& hand, Suit queSuit) const;

private:
    const ShantenCalculator& shantenCalculator_;
};
