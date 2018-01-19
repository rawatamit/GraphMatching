#ifndef ENVYFREE_HRLQ_H
#define ENVYFREE_HRLQ_H

#include "MatchingAlgorithm.h"

class EnvyfreeHRLQ : public MatchingAlgorithm {
private:
    // augment graph to compute matching
    std::unique_ptr<BipartiteGraph> augment_graph();

public:
    EnvyfreeHRLQ(const std::unique_ptr<BipartiteGraph>& G, bool A_proposing=true);
    virtual ~EnvyfreeHRLQ();

    bool compute_matching();
    MatchedPairListType& get_matched_pairs();
};

#endif
