#ifndef N_PROPOSING_MATCHING_H
#define N_PROPOSING_MATCHING_H

#include "MatchingAlgorithm.h"

// simulate gale and shapley with n levels for proposing side
class NProposingMatching : public MatchingAlgorithm {
private:
    bool A_proposing_; // true if vertices from partition A propose, otherwise false
    int max_level;     // maximum level that a vertex can reach

public:
    explicit NProposingMatching(const std::unique_ptr<BipartiteGraph>& G, bool A_proposing=true, int max_level=1);
    ~NProposingMatching() override = default;

    bool compute_matching() override;
};

#endif
