#ifndef CRITICAL_RSM_H
#define CRITICAL_RSM_H

#include "MatchingAlgorithm.h"

// Critical relaxed stable matching is critical as well as relaxed stable
// A matching M is critical if there is no other matching that matches 
// more critical vertices than M
// A matching M is relaxed stable if for every blocking pair (a, b) 
// w.r.t. M one of the following holds:
// 1. a is matched and b' = M(a) is critical, or
// 2. b is matched and a′ = M(b) is critical
class CriticalRSM : public MatchingAlgorithm {
private:
    void ties_propose(FreeListType& free_list, VertexPtr a, const PreferenceList& a_pref_list, std::map<VertexPtr, VertexBookkeeping>& bookkeep_data, Matching& M, const int t);
    void critical_propose(FreeListType& free_list, VertexPtr a, const PreferenceList& a_pref_list, std::map<VertexPtr, VertexBookkeeping>& bookkeep_data, Matching& M);

public:
    VertexPtr favourite_neighbour(VertexPtr u, const PreferenceList& u_pref_list, VertexBookkeeping &u_data, const Matching& M);
    explicit CriticalRSM(std::shared_ptr<BipartiteGraph> G, bool A_proposing=true);
    ~CriticalRSM() override = default;

    Matching compute_matching() override;
};

#endif
