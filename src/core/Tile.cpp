#include "core/Tile.h"

#include <sstream>

Tile::Tile(Suit suit, int rank) : suit_(suit), rank_(rank) {}

int Tile::index() const {
    return static_cast<int>(suit_) * 9 + (rank_ - 1);
}

Tile Tile::fromIndex(int index) {
    if (!isValidIndex(index)) {
        return {};
    }
    return {static_cast<Suit>(index / 9), (index % 9) + 1};
}

bool Tile::isValid(Suit suit, int rank) {
    return rank >= 1 && rank <= 9
        && static_cast<int>(suit) >= 0
        && static_cast<int>(suit) <= 2;
}

bool Tile::isValidIndex(int index) {
    return index >= 0 && index < kTileTypeCount;
}

std::string Tile::suitName() const {
    switch (suit_) {
    case Suit::Tong: return "筒";
    case Suit::Tiao: return "条";
    case Suit::Wan:  return "万";
    }
    return "";
}

std::string Tile::toString() const {
    std::ostringstream oss;
    oss << rank_ << suitName();
    return oss.str();
}

bool Tile::operator==(const Tile& other) const {
    return suit_ == other.suit_ && rank_ == other.rank_;
}

bool Tile::operator!=(const Tile& other) const {
    return !(*this == other);
}

bool Tile::operator<(const Tile& other) const {
    if (suit_ != other.suit_) {
        return static_cast<int>(suit_) < static_cast<int>(other.suit_);
    }
    return rank_ < other.rank_;
}
