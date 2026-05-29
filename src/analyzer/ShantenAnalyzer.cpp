#include "analyzer/ShantenAnalyzer.h"

int ShantenAnalyzer::analyze(const Hand& hand, Suit queSuit) const {
    return calculator_.calculate(hand, queSuit);
}
