#ifndef MATCHING_ALGORITHM_H
#define MATCHING_ALGORITHM_H

#include "BipartiteGraph.h"
#include "PartnerList.h"

class MatchingAlgorithm {
public:
    typedef std::map<std::shared_ptr<Vertex>, PartnerList> MatchedPairListType;

private:
    std::shared_ptr<BipartiteGraph> G_;
    bool A_proposing_; // true if vertices from partition A propose, otherwise false

protected:
    std::shared_ptr<MatchedPairListType> map_inverse(std::shared_ptr<MatchedPairListType> M) const;
    void add_partner(std::shared_ptr<MatchedPairListType> M, VertexPtr u, const Partner& v, int level) const;
    void add_partner(std::shared_ptr<MatchedPairListType> M, VertexPtr u, VertexPtr v, RankType rank, int level) const;

public:
    explicit MatchingAlgorithm(std::shared_ptr<BipartiteGraph> G, bool A_proposing);
    virtual ~MatchingAlgorithm() = default;

    bool is_A_proposing();
    virtual std::shared_ptr<MatchedPairListType> compute_matching() = 0;
    std::shared_ptr<BipartiteGraph> get_graph() const;
    bool is_feasible(std::shared_ptr<BipartiteGraph> G,
                     std::shared_ptr<MatchedPairListType> M);
};

#endif
