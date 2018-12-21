#ifndef MATCHING_ALGORITHM_H
#define MATCHING_ALGORITHM_H

#include "BipartiteGraph.h"

class MatchingAlgorithm {
protected:
    MatchedPairListType M_;

private:
    const std::unique_ptr<BipartiteGraph>& G_;

protected:
    MatchedPairListType& map_inverse(const MatchedPairListType& M);

public:
    explicit MatchingAlgorithm(const std::unique_ptr<BipartiteGraph>& G);
    virtual ~MatchingAlgorithm() = default;
    virtual bool compute_matching() = 0;
    virtual MatchedPairListType& get_matched_pairs();
    const std::unique_ptr<BipartiteGraph>& get_graph() const;
    bool is_feasible(const std::unique_ptr<BipartiteGraph>& G,
                     const MatchedPairListType& M);
};

#endif
