#ifndef RHEURISTIC_HRLQ_H
#define RHEURISTIC_HRLQ_H

#include "MatchingAlgorithm.h"

class RHeuristicHRLQ : public MatchingAlgorithm {
private:
    // graphs for phase 1 and 2
    std::unique_ptr<BipartiteGraph> G1_;
    std::unique_ptr<BipartiteGraph> G2_;

    // matching to hold temporary results
    MatchedPairListType M_tmp_;

    // augment graph for phase 1 and phase 2
    std::unique_ptr<BipartiteGraph> augment_phase1();
    std::unique_ptr<BipartiteGraph> augment_phase2(MatchedPairListType& M);

public:
    // A_proposing should always be true, this is a resident proposing
    // heuristic
    RHeuristicHRLQ(const std::unique_ptr<BipartiteGraph>& G, bool A_proposing=true);
    virtual ~RHeuristicHRLQ();

    bool compute_matching();
    MatchedPairListType& get_matched_pairs();
};

#endif
