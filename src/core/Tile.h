#pragma once

#include <string>

enum class Suit {
    Tong = 0,  // 筒
    Tiao = 1,  // 条
    Wan = 2    // 万
};

constexpr int kTileTypeCount = 27;  // 3 suits x 9 ranks
constexpr int kMaxTilesPerType = 4;
constexpr int kMaxHandSize = 14;

class Tile {
public:
    Tile() = default;
    Tile(Suit suit, int rank);

    Suit suit() const { return suit_; }
    int rank() const { return rank_; }
    int index() const;

    static Tile fromIndex(int index);
    static bool isValid(Suit suit, int rank);
    static bool isValidIndex(int index);

    std::string toString() const;
    std::string suitName() const;

    bool operator==(const Tile& other) const;
    bool operator!=(const Tile& other) const;
    bool operator<(const Tile& other) const;

private:
    Suit suit_ = Suit::Tong;
    int rank_ = 1;
};
