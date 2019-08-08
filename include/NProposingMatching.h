#ifndef N_PROPOSING_MATCHING_H
#define N_PROPOSING_MATCHING_H

#include "MatchingAlgorithm.h"
#include "PreferenceList.h"
#include "VertexBookkeeping.h"
#include <stack>
#include <ostream>

// simulate Gale and Shapley with n levels for proposing side
class NProposingMatching : public MatchingAlgorithm {
private:
    // typedef list for unmatched vertices
    typedef std::stack<VertexPtr> FreeListType;

    // maximum level that a vertex can reach
    int max_level;

    // remove arbitrary element from list of unmatched vertices
    // and update bookkeep data
    static VertexPtr remove_and_update(FreeListType& free_list, std::map<VertexPtr, VertexBookkeeping>& bookkeep_data);

    static void add_to_list(FreeListType& free_list, VertexPtr u);

    // add vertex u to free list and update proposal index if required
    static void add_to_list_and_update(FreeListType& free_list, VertexBookkeeping& u_data, VertexPtr u);

    // add (u, v) pair to M_
    void add_matched_partners(std::shared_ptr<MatchedPairListType> M,
                              VertexPtr u, VertexPtr v,
                              const VertexBookkeeping& u_data,
                              const PreferenceList& v_pref_list);

public:
    explicit NProposingMatching(std::shared_ptr<BipartiteGraph> G, bool A_proposing, int max_level);
    ~NProposingMatching() override = default;

    std::shared_ptr<MatchedPairListType> compute_matching() override;
};

#endif
