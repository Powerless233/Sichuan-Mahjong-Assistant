#include "analyzer/EfficiencyAnalyzer.h"

EfficiencyAnalyzer::EfficiencyAnalyzer()
    : efficiencyCalculator_(shantenCalculator_) {}

int EfficiencyAnalyzer::analyze(const Hand& hand, Suit queSuit) const {
    return analyzeDetail(hand, queSuit).acceptingTiles;
}

EfficiencyResult EfficiencyAnalyzer::analyzeDetail(const Hand& hand, Suit queSuit) const {
    return efficiencyCalculator_.analyze(hand, queSuit);
}
