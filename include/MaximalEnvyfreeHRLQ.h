#ifndef MAXIMAL_ENVYFREE_HRLQ_H
#define MAXIMAL_ENVYFREE_HRLQ_H

#include "MatchingAlgorithm.h"

class MaximalEnvyfreeHRLQ : public MatchingAlgorithm {
private:
    // augment graph to compute matching
    std::unique_ptr<BipartiteGraph> augment_graph(MatchedPairListType& M);

public:
    MaximalEnvyfreeHRLQ(const std::unique_ptr<BipartiteGraph>& G, bool A_proposing=false);
    virtual ~MaximalEnvyfreeHRLQ();

    bool compute_matching();
    MatchedPairListType& get_matched_pairs();
};

#endif
