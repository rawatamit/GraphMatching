#ifndef YOKOI_ENVYFREE_HRLQ_H
#define YOKOI_ENVYFREE_HRLQ_H

#include "MatchingAlgorithm.h"

class YokoiEnvyfreeHRLQ : public MatchingAlgorithm {
private:
    // augment graph to compute matching
    std::shared_ptr<BipartiteGraph> augment_graph() const;

public:
    explicit YokoiEnvyfreeHRLQ(std::shared_ptr<BipartiteGraph> G, bool A_proposing=true);
    ~YokoiEnvyfreeHRLQ() override = default;

    std::shared_ptr<MatchedPairListType> compute_matching() override;
};

#endif
