#include "core/ShantenCalculator.h"

#include <algorithm>
#include <array>

int ShantenCalculator::stepsToWin(int shanten)
{
    return shanten + 1;
}

std::string ShantenCalculator::formatLabel(int shanten)
{
    const int steps = stepsToWin(shanten);
    if (steps <= 0)
    {
        return "听牌";
    }
    return std::to_string(steps) + "向听";
}

void ShantenCalculator::extractValidCounts(const int counts[kTileTypeCount], Suit queSuit, int validCounts[kTileTypeCount],
                                           int &deadCount) const
{
    deadCount = 0;
    const int queStart = static_cast<int>(queSuit) * 9;
    const int queEnd = queStart + 9;

    for (int i = 0; i < kTileTypeCount; ++i)
    {
        if (i >= queStart && i < queEnd)
        {
            deadCount += counts[i];
            validCounts[i] = 0;
        }
        else
        {
            validCounts[i] = counts[i];
        }
    }
}

int ShantenCalculator::calcSevenPairsShanten(const int counts[kTileTypeCount]) const
{
    int completePairs = 0;
    int partialPairs = 0;

    for (int i = 0; i < kTileTypeCount; ++i)
    {
        if (counts[i] >= 2)
        {
            ++completePairs;
        }
        else if (counts[i] == 1)
        {
            ++partialPairs;
        }
    }

    return 6 - completePairs - (partialPairs > 0 ? 1 : 0);
}

int ShantenCalculator::decompose(int index, int counts[kTileTypeCount],
                                 int melds, int taatsu, bool hasPair, int isolated) const
{
    while (index < kTileTypeCount && counts[index] == 0)
    {
        ++index;
    }
    if (index >= kTileTypeCount)
    {
        int shanten = 8 - 2 * melds - taatsu - (hasPair ? 1 : 0);
        shanten += isolated;

        // 听牌：3 面子 + 1 搭子 + 雀头，无孤张
        if (hasPair && melds == 3 && taatsu == 1 && isolated == 0)
        {
            shanten = std::min(shanten, -1);
        }
        // 听牌：4 面子，单张等雀头
        if (!hasPair && melds == 4 && taatsu == 0 && isolated == 1)
        {
            shanten = std::min(shanten, -1);
        }

        return shanten;
    }

    int minShanten = 8;
    const int rank = index % 9;

    minShanten = std::min(minShanten,
                          decompose(index + 1, counts, melds, taatsu, hasPair, isolated + 1));

    if (counts[index] >= 3)
    {
        counts[index] -= 3;
        minShanten = std::min(minShanten,
                              decompose(index, counts, melds + 1, taatsu, hasPair, isolated));
        counts[index] += 3;
    }

    if (rank <= 6 && counts[index + 1] > 0 && counts[index + 2] > 0)
    {
        counts[index]--;
        counts[index + 1]--;
        counts[index + 2]--;
        minShanten = std::min(minShanten,
                              decompose(index, counts, melds + 1, taatsu, hasPair, isolated));
        counts[index]++;
        counts[index + 1]++;
        counts[index + 2]++;
    }

    if (!hasPair && counts[index] >= 2)
    {
        counts[index] -= 2;
        minShanten = std::min(minShanten,
                              decompose(index, counts, melds, taatsu, true, isolated));
        counts[index] += 2;
    }

    if (melds + taatsu < 4)
    {
        if (rank <= 7 && counts[index + 1] > 0)
        {
            counts[index]--;
            counts[index + 1]--;
            minShanten = std::min(minShanten,
                                  decompose(index, counts, melds, taatsu + 1, hasPair, isolated));
            counts[index]++;
            counts[index + 1]++;
        }

        if (rank <= 6 && counts[index + 2] > 0)
        {
            counts[index]--;
            counts[index + 2]--;
            minShanten = std::min(minShanten,
                                  decompose(index, counts, melds, taatsu + 1, hasPair, isolated));
            counts[index]++;
            counts[index + 2]++;
        }

        if (counts[index] >= 2)
        {
            counts[index] -= 2;
            minShanten = std::min(minShanten,
                                  decompose(index, counts, melds, taatsu + 1, hasPair, isolated));
            counts[index] += 2;
        }
    }

    return minShanten;
}

int ShantenCalculator::calcStandardShanten(int counts[kTileTypeCount]) const
{
    return decompose(0, counts, 0, 0, false, 0);
}

int ShantenCalculator::calculate(const Hand &hand, Suit queSuit) const
{
    const auto counts = hand.counts();
    int validCounts[kTileTypeCount]{};
    int deadCount = 0;
    extractValidCounts(counts.data(), queSuit, validCounts, deadCount);

    int validCountsCopy[kTileTypeCount];
    for (int i = 0; i < kTileTypeCount; ++i)
    {
        validCountsCopy[i] = validCounts[i];
    }

    const int standard = calcStandardShanten(validCountsCopy);
    const int sevenPairs = calcSevenPairsShanten(validCounts);
    const int base = std::min(standard, sevenPairs);

    return std::max(base, deadCount - 1);
}
