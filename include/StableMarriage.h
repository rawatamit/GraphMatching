#ifndef STABLE_MARRIAGE_H
#define STABLE_MARRIAGE_H

#include "MatchingAlgorithm.h"

class StableMarriage : public MatchingAlgorithm {
public:
    StableMarriage(const std::unique_ptr<BipartiteGraph>& G);
    virtual ~StableMarriage();

    void compute_matching();
};

#endif
