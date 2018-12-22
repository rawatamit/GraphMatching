#ifndef MATCHING_ALGORITHM_H
#define MATCHING_ALGORITHM_H

#include "BipartiteGraph.h"
#include "PartnerList.h"

class MatchingAlgorithm {
public:
    typedef std::map<std::shared_ptr<Vertex>, PartnerList> MatchedPairListType;

protected:
    MatchedPairListType M_;

private:
    const std::unique_ptr<BipartiteGraph>& G_;
    bool A_proposing_; // true if vertices from partition A propose, otherwise false

protected:
    MatchedPairListType& map_inverse(const MatchedPairListType& M);

public:
    explicit MatchingAlgorithm(const std::unique_ptr<BipartiteGraph>& G, bool A_proposing);
    virtual ~MatchingAlgorithm() = default;

    bool is_A_proposing();
    virtual bool compute_matching() = 0;
    virtual MatchedPairListType& get_matched_pairs();
    const std::unique_ptr<BipartiteGraph>& get_graph() const;
    bool is_feasible(const std::unique_ptr<BipartiteGraph>& G,
                     const MatchedPairListType& M);
};

#endif
