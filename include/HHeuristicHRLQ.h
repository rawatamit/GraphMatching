#ifndef HHEURISTIC_HRLQ_H
#define HHEURISTIC_HRLQ_H

#include "MatchingAlgorithm.h"

class HHeuristicHRLQ : public MatchingAlgorithm {
private:
    // graph for phase 2
    std::shared_ptr<BipartiteGraph> G2_;

    // matching to hold temporary results
    std::shared_ptr<MatchedPairListType> M_tmp_;

    bool compute_phase2_matching(std::shared_ptr<MatchedPairListType> M,
                                 std::map<IdType, PartnerList::SizeType >& def,
                                 int s);
    // augment graph phase 2
    std::shared_ptr<BipartiteGraph> augment_phase2(int s);

public:
    explicit HHeuristicHRLQ(std::shared_ptr<BipartiteGraph> G, bool A_proposing=true);
    ~HHeuristicHRLQ() override = default;

    std::shared_ptr<MatchedPairListType> compute_matching() override;
};

#endif
