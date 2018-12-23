#ifndef STABLE_MARRIAGE_H
#define STABLE_MARRIAGE_H

#include "MatchingAlgorithm.h"
#include "NProposingMatching.h"

class StableMarriage : public MatchingAlgorithm {
public:
    explicit StableMarriage(std::shared_ptr<BipartiteGraph> G,
                            bool A_proposing=true)
    : MatchingAlgorithm(G, A_proposing)
    {}

    ~StableMarriage() override = default;

    std::shared_ptr<MatchingAlgorithm::MatchedPairListType> compute_matching() override {
        NProposingMatching npm (get_graph(), is_A_proposing(), 0);
        return npm.compute_matching();
    }
};

#endif
