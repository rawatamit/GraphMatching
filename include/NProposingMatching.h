#ifndef N_PROPOSING_MATCHING_H
#define N_PROPOSING_MATCHING_H

#include "MatchingAlgorithm.h"
#include "PreferenceList.h"
#include "VertexBookkeeping.h"
#include <stack>
#include <ostream>

// simulate Gale and Shapley with n levels for proposing side
class NProposingMatching : public MatchingAlgorithm {
private:
    // maximum level that a vertex can reach
    int max_level;

public:
    NProposingMatching(std::shared_ptr<BipartiteGraph> G,
                       bool A_proposing, int max_level)
            : MatchingAlgorithm(G, A_proposing), max_level(max_level)
    {}

    ~NProposingMatching() override = default;

    Matching compute_matching() override;
};

#endif
