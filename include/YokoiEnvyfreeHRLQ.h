#ifndef YOKOI_ENVYFREE_HRLQ_H
#define YOKOI_ENVYFREE_HRLQ_H

#include "MatchingAlgorithm.h"

class YokoiEnvyfreeHRLQ : public MatchingAlgorithm {
private:
    // augment graph to compute matching
    std::unique_ptr<BipartiteGraph> augment_graph();

public:
    explicit YokoiEnvyfreeHRLQ(const std::unique_ptr<BipartiteGraph>& G, bool A_proposing=true);
    ~YokoiEnvyfreeHRLQ() override = default;

    bool compute_matching() override;
    MatchedPairListType& get_matched_pairs() override;
};

#endif
