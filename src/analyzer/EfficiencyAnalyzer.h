#pragma once

#include "analyzer/Analyzer.h"
#include "core/EfficiencyCalculator.h"
#include "core/ShantenCalculator.h"

class EfficiencyAnalyzer : public Analyzer {
public:
    EfficiencyAnalyzer();

    int analyze(const Hand& hand, Suit queSuit) const override;
    EfficiencyResult analyzeDetail(const Hand& hand, Suit queSuit) const;

private:
    ShantenCalculator shantenCalculator_;
    EfficiencyCalculator efficiencyCalculator_;
};
