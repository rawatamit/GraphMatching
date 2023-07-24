#ifndef MATCHING_ALGORITHM_H
#define MATCHING_ALGORITHM_H

#include "BipartiteGraph.h"
#include "PartnerList.h"
#include "Matching.h"
#include "VertexBookkeeping.h"
#include <stack>

class MatchingAlgorithm {
private:
    std::shared_ptr<BipartiteGraph> G_;
    // True if partition A vertices are proposing.
    bool A_proposing_;

protected:
    Matching map_inverse(const Matching& M) const;

    // typedef list for unmatched vertices
    typedef std::stack<VertexPtr> FreeListType;

    // remove arbitrary element from list of unmatched vertices
    // and update bookkeep data
    VertexPtr remove_from_free_list
    (FreeListType& free_list, std::map<VertexPtr, VertexBookkeeping>& bookkeep_data);

    void add_to_free_list(FreeListType& free_list, VertexPtr u);

    // add vertex u to free list and update proposal index if required
    void add_to_free_list(FreeListType& free_list, VertexBookkeeping& u_data, VertexPtr u);

    // add (u, v) pair to M_
    void add_matched_partners(Matching& M,
                              VertexPtr u, VertexPtr v,
                              const VertexBookkeeping& u_data,
                              const PreferenceList& v_pref_list);

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
