#pragma once

#include "core/Hand.h"

#include <string>

class ShantenCalculator {
public:
    int calculate(const Hand& hand, Suit queSuit) const;

    // 内部向听 -1=听牌，0=一向听，1=二向听…
    // 返回距听牌还需的步数：0=听牌，1=一向听，2=二向听…
    static int stepsToWin(int shanten);
    static std::string formatLabel(int shanten);

private:
    int calcStandardShanten(int counts[kTileTypeCount]) const;
    int calcSevenPairsShanten(const int counts[kTileTypeCount]) const;
    int decompose(int index, int counts[kTileTypeCount],
                  int melds, int taatsu, bool hasPair, int isolated) const;
    void extractValidCounts(const int counts[kTileTypeCount], Suit queSuit,
                            int validCounts[kTileTypeCount], int& deadCount) const;
};