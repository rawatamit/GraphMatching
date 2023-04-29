#ifndef MATCHING_ALGORITHM_H
#define MATCHING_ALGORITHM_H

#include "BipartiteGraph.h"
#include "PartnerList.h"
#include "Matching.h"

class MatchingAlgorithm {
private:
    std::shared_ptr<BipartiteGraph> G_;
    // True if partition A vertices are proposing.
    bool A_proposing_;

protected:
    Matching map_inverse(const Matching& M) const;

public:
    explicit MatchingAlgorithm(std::shared_ptr<BipartiteGraph> G, bool A_proposing);
    virtual ~MatchingAlgorithm() = default;

    bool is_A_proposing();
    virtual Matching compute_matching() = 0;
    std::shared_ptr<BipartiteGraph> get_graph() const;
    bool is_feasible(std::shared_ptr<BipartiteGraph> G,
                     const Matching& M);
};

#endif
