#ifndef STABLE_MARRIAGE_H
#define STABLE_MARRIAGE_H

#include "MatchingAlgorithm.h"
#include "NProposingMatching.h"

class StableMarriage : public MatchingAlgorithm {
private:
    bool A_proposing_; // true if vertices from partition A propose, otherwise false

public:
    StableMarriage(const std::unique_ptr<BipartiteGraph>& G,
                   bool A_proposing=true)
    : MatchingAlgorithm(G), A_proposing_(A_proposing)
    {}

    virtual ~StableMarriage()
    {}

    bool compute_matching() {
        NProposingMatching npm (get_graph(), A_proposing_, 1);
        bool computed = npm.compute_matching();
        M_ = npm.get_matched_pairs();
        return computed;
    }
};

#endif
