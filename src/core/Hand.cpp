#include "core/Hand.h"

Hand Hand::withoutTile(const Tile& tile) const {
    Hand result = *this;
    result.removeTile(tile);
    return result;
}

bool Hand::addTile(const Tile& tile) {
    if (!Tile::isValid(tile.suit(), tile.rank())) {
        return false;
    }
    if (totalCount() >= kMaxHandSize) {
        return false;
    }
    const int idx = tile.index();
    if (counts_[idx] >= kMaxTilesPerType) {
        return false;
    }
    ++counts_[idx];
    return true;
}

bool Hand::removeTile(const Tile& tile) {
    if (!Tile::isValid(tile.suit(), tile.rank())) {
        return false;
    }
    const int idx = tile.index();
    if (counts_[idx] <= 0) {
        return false;
    }
    --counts_[idx];
    return true;
}

void Hand::clear() {
    counts_.fill(0);
}

int Hand::totalCount() const {
    int sum = 0;
    for (int count : counts_) {
        sum += count;
    }
    return sum;
}

int Hand::countOf(const Tile& tile) const {
    return counts_[tile.index()];
}

int Hand::countOfIndex(int index) const {
    if (!Tile::isValidIndex(index)) {
        return 0;
    }
    return counts_[index];
}

std::array<int, kTileTypeCount> Hand::counts() const {
    return counts_;
}

std::vector<Tile> Hand::tiles() const {
    std::vector<Tile> result;
    for (int i = 0; i < kTileTypeCount; ++i) {
        for (int j = 0; j < counts_[i]; ++j) {
            result.push_back(Tile::fromIndex(i));
        }
    }
    return result;
}
