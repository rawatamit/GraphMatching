#ifndef STABLE_MARRIAGE_H
#define STABLE_MARRIAGE_H

#include "MatchingAlgorithm.h"
#include "NProposingMatching.h"

class StableMarriage : public MatchingAlgorithm {
public:
    explicit StableMarriage(const std::unique_ptr<BipartiteGraph>& G,
                            bool A_proposing=true)
    : MatchingAlgorithm(G, A_proposing)
    {}

    ~StableMarriage() override = default;

    bool compute_matching() override {
        NProposingMatching npm (get_graph(), is_A_proposing(), 0);
        bool computed = npm.compute_matching();
        M_ = npm.get_matched_pairs();
        return computed;
    }
};

#endif
