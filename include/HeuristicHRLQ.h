#ifndef HEURISTIC_HRLQ_H
#define HEURISTIC_HRLQ_H

#include "MatchingAlgorithm.h"

class HeuristicHRLQ : public MatchingAlgorithm {
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
    HeuristicHRLQ(const std::unique_ptr<BipartiteGraph>& G);
    virtual ~HeuristicHRLQ();

    bool compute_matching();
    MatchedPairListType& get_matched_pairs();
};

#endif
