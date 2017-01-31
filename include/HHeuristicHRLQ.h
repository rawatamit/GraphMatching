#ifndef HHEURISTIC_HRLQ_H
#define HHEURISTIC_HRLQ_H

#include "MatchingAlgorithm.h"

class HHeuristicHRLQ : public MatchingAlgorithm {
private:
    // graph for phase 2
    std::unique_ptr<BipartiteGraph> G2_;

    // matching to hold temporary results
    MatchedPairListType M_tmp_;

    bool compute_phase2_matching(const MatchedPairListType& M,
                                 std::map<IdType, int>& def,
                                 int s);
    // augment graph phase 2
    std::unique_ptr<BipartiteGraph> augment_phase2(const MatchedPairListType& M, int s);

public:
    HHeuristicHRLQ(const std::unique_ptr<BipartiteGraph>& G, bool A_proposing=true);
    virtual ~HHeuristicHRLQ();

    bool compute_matching();
    MatchedPairListType& get_matched_pairs();
};

#endif
