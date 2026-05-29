#pragma once

#include "core/EfficiencyCalculator.h"
#include "core/Hand.h"
#include "core/ShantenCalculator.h"
#include "core/Tile.h"

#include <string>
#include <vector>

struct DiscardOption {
    Tile tile;
    int shanten = 0;
    int acceptingTiles = 0;
    int acceptingTypes = 0;
    std::string acceptingDetail;
};

class DiscardAnalyzer {
public:
    DiscardAnalyzer();

    std::vector<DiscardOption> analyzeDiscards(const Hand& hand, Suit queSuit) const;
    int analyzeCurrentShanten(const Hand& hand, Suit queSuit) const;

private:
    ShantenCalculator shantenCalculator_;
    EfficiencyCalculator efficiencyCalculator_;
};
