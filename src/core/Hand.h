#pragma once

#include "core/Tile.h"

#include <array>
#include <vector>

class Hand {
public:
    Hand() = default;

    bool addTile(const Tile& tile);
    bool removeTile(const Tile& tile);
    void clear();

    int totalCount() const;
    int countOf(const Tile& tile) const;
    int countOfIndex(int index) const;

    std::array<int, kTileTypeCount> counts() const;
    std::vector<Tile> tiles() const;

    Hand withoutTile(const Tile& tile) const;

private:
    std::array<int, kTileTypeCount> counts_{};
};
