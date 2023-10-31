#ifndef N_PROPOSING_TIES_MATCHING_H
#define N_PROPOSING_TIES_MATCHING_H

#include "MatchingAlgorithm.h"
#include "PreferenceList.h"
#include "VertexBookkeeping.h"
#include <stack>
#include <ostream>

// Simulate Gale and Shapley with n levels for proposing side when ties are present
// in the preference lists in order to get a weak stable matching
class NProposingTiesMatching : public MatchingAlgorithm {
private:
    // maximum level that a vertex can reach
    int max_level_;

public:
    NProposingTiesMatching(std::shared_ptr<BipartiteGraph> G,
                       bool A_proposing, int max_level)
            : MatchingAlgorithm(G, A_proposing), max_level_(max_level)
    {}

    ~NProposingTiesMatching() override = default;

    Matching compute_matching() override;
};

#endif
