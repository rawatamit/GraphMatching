#ifndef MAXIMAL_ENVYFREE_HRLQ_H
#define MAXIMAL_ENVYFREE_HRLQ_H

#include "MatchingAlgorithm.h"

class MaximalEnvyfreeHRLQ : public MatchingAlgorithm {
private:
    // augment graph to compute matching
    std::shared_ptr<BipartiteGraph> augment_graph(std::shared_ptr<MatchedPairListType> M);

public:
    explicit MaximalEnvyfreeHRLQ(std::shared_ptr<BipartiteGraph> G, bool A_proposing=false);
    ~MaximalEnvyfreeHRLQ() override = default;

    std::shared_ptr<MatchedPairListType> compute_matching() override;
};

#endif
