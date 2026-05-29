#include "core/DiscardAnalyzer.h"

#include <algorithm>
#include <sstream>

DiscardAnalyzer::DiscardAnalyzer()
    : efficiencyCalculator_(shantenCalculator_) {}

int DiscardAnalyzer::analyzeCurrentShanten(const Hand& hand, Suit queSuit) const {
    return shantenCalculator_.calculate(hand, queSuit);
}

std::vector<DiscardOption> DiscardAnalyzer::analyzeDiscards(const Hand& hand,
                                                            Suit queSuit) const {
    std::vector<DiscardOption> options;

    if (hand.totalCount() != kMaxHandSize) {
        return options;
    }

    const auto tiles = hand.tiles();
    for (size_t i = 0; i < tiles.size(); ++i) {
        const Tile& tile = tiles[i];
        if (i > 0 && tiles[i - 1] == tile) {
            continue;
        }

        const Hand remaining = hand.withoutTile(tile);
        const int shanten = shantenCalculator_.calculate(remaining, queSuit);
        const EfficiencyResult efficiency = efficiencyCalculator_.analyze(remaining, queSuit);

        DiscardOption option;
        option.tile = tile;
        option.shanten = shanten;
        option.acceptingTiles = efficiency.acceptingTiles;
        option.acceptingTypes = efficiency.acceptingTypes;

        std::ostringstream oss;
        for (size_t j = 0; j < efficiency.acceptingLabels.size(); ++j) {
            if (j > 0) {
                oss << " ";
            }
            oss << efficiency.acceptingLabels[j];
        }
        option.acceptingDetail = oss.str();

        options.push_back(option);
    }

    std::sort(options.begin(), options.end(),
              [](const DiscardOption& a, const DiscardOption& b) {
                  if (a.shanten != b.shanten) {
                      return a.shanten < b.shanten;
                  }
                  if (a.acceptingTiles != b.acceptingTiles) {
                      return a.acceptingTiles > b.acceptingTiles;
                  }
                  return a.acceptingTypes > b.acceptingTypes;
              });

    return options;
}
