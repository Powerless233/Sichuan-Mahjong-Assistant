#include "core/EfficiencyCalculator.h"

#include "core/ShantenCalculator.h"

EfficiencyCalculator::EfficiencyCalculator(const ShantenCalculator& shantenCalculator)
    : shantenCalculator_(shantenCalculator) {}

EfficiencyResult EfficiencyCalculator::analyze(const Hand& hand, Suit queSuit) const {
    EfficiencyResult result;
    const int currentShanten = shantenCalculator_.calculate(hand, queSuit);
    const auto counts = hand.counts();

    for (int i = 0; i < kTileTypeCount; ++i) {
        const int suitIndex = i / 9;
        if (static_cast<Suit>(suitIndex) == queSuit) {
            continue;
        }
        if (counts[i] >= kMaxTilesPerType) {
            continue;
        }

        Hand next = hand;
        if (!next.addTile(Tile::fromIndex(i))) {
            continue;
        }

        const int nextShanten = shantenCalculator_.calculate(next, queSuit);
        if (nextShanten < currentShanten) {
            const int remaining = kMaxTilesPerType - counts[i];
            result.acceptingTiles += remaining;
            ++result.acceptingTypes;
            result.acceptingLabels.push_back(Tile::fromIndex(i).toString());
        }
    }

    return result;
}
