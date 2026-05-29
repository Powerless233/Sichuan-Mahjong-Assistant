#pragma once

#include "analyzer/Analyzer.h"
#include "core/ShantenCalculator.h"

class ShantenAnalyzer : public Analyzer {
public:
    int analyze(const Hand& hand, Suit queSuit) const override;

private:
    ShantenCalculator calculator_;
};
