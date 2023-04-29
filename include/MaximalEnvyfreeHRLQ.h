#ifndef MAXIMAL_ENVYFREE_HRLQ_H
#define MAXIMAL_ENVYFREE_HRLQ_H

#include "MatchingAlgorithm.h"

class MaximalEnvyfreeHRLQ : public MatchingAlgorithm {
private:
    // augment graph to compute matching
    std::shared_ptr<BipartiteGraph> augment_graph(const Matching& M) const;

    // M1 U M2
    // modifies M1, does not modify M2
    void matching_union(Matching& M1, const Matching& M2);

public:
    explicit MaximalEnvyfreeHRLQ(std::shared_ptr<BipartiteGraph> G, bool A_proposing=false);
    ~MaximalEnvyfreeHRLQ() override = default;

    Matching compute_matching() override;
};

#endif
