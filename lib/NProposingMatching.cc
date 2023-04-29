#include "NProposingMatching.h"
#include "Vertex.h"
#include "Partner.h"
#include "Utils.h"
#include <set>

NProposingMatching::NProposingMatching(std::shared_ptr<BipartiteGraph> G,
                                       bool A_proposing, int max_level)
    : MatchingAlgorithm(G, A_proposing), max_level(max_level)
{}

VertexPtr NProposingMatching::remove_from_free_list
(FreeListType& free_list, std::map<VertexPtr, VertexBookkeeping>& bookkeep_data) {
    auto ret = free_list.top();
    free_list.pop(); // remove u from free_list
    bookkeep_data[ret].in_free_list = false;
    return ret;
}

void NProposingMatching::add_to_free_list(FreeListType& free_list, VertexPtr u) {
    free_list.push(u);
}

void NProposingMatching::add_to_free_list(NProposingMatching::FreeListType &free_list,
                                          VertexBookkeeping &u_data, VertexPtr u) {
    if (not u_data.in_free_list) {
        u_data.in_free_list = true;
        add_to_free_list(free_list, u);
    }
}

void NProposingMatching::add_matched_partners(std::shared_ptr<MatchedPairListType> M,
                                              VertexPtr u, VertexPtr v,
                                              const VertexBookkeeping& u_data,
                                              const PreferenceList& v_pref_list) {
    // v is the first vertex on u's current preference list
    // invariant for non proposing vertices: rank = index + 1
    // v is always at level 0
    add_partner(M, u, v, (RankType) u_data.begin + 1, 0);
    add_partner(M, v, u, compute_rank(u, v_pref_list), u_data.level);
}

std::shared_ptr<MatchingAlgorithm::MatchedPairListType> NProposingMatching::compute_matching() {
    FreeListType free_list;
  std::map<VertexPtr, VertexBookkeeping> bookkeep_data;
  std::shared_ptr<BipartiteGraph> G = get_graph();
  auto M = std::make_shared<MatchingAlgorithm::MatchedPairListType>();
  
  // choose the partitions from which the vertices will propose
  const auto& proposing_partition = is_A_proposing() ? G->get_A_partition()
                                                     : G->get_B_partition();

  // set the level of every vertex in the proposing partition to 0
  // mark all proposing vertices free (by pushing into the free_list)
  // and vertices from the opposite partition implicitly free
  for (auto &it : proposing_partition) {
    auto v = it.second;
    free_list.push(v);
    int pref_list_size = v->get_preference_list().size();
    int residual = v->get_upper_quota();
    bookkeep_data[v] = VertexBookkeeping(0, pref_list_size, residual);
  }

  // there is at least one vertex in the free list
  while (not free_list.empty()) {
    // arbitrary vertex in free list
    auto u = remove_from_free_list(free_list, bookkeep_data);
    const auto &u_pref_list = u->get_preference_list();
    auto &u_data = bookkeep_data[u];

    // if u^l can have a partner and hasn't exhausted its preference list
    while (u_data.residual > 0 and not u_data.is_exhausted()) {
      // Highest ranked vertex to whom u has not yet proposed.
      auto v = u_pref_list.at(u_data.begin).vertex;
      u_data.begin += 1;
      const auto &v_pref_list = v->get_preference_list();

      if (u_data.level > 0 and is_matched_to(M, v, u, u_data.level - 1)) {
        // Remove u^(level-1) and v from the matching.
        M->at(u).remove(v);
        M->at(v).remove(u);

        // Add u^level and v to the matching. Note that residual(u) doesn't
        // change, as we are replacing one vertex with another.
        add_matched_partners(M, u, v, u_data, v_pref_list);
      } else if (number_of_partners(M, v) == v->get_upper_quota()) {
        // |M[v]| >= upper_quota(v)
        auto v_all_partners = get_partners(M, v);
        auto v_worst_partner = v_all_partners.get_least_preferred();
        auto possible_partner =
            Partner(u, compute_rank(u, v_pref_list), u_data.level);

        if (v_worst_partner < possible_partner) {
          auto &v_worst_partner_data = bookkeep_data[v_worst_partner.vertex];

          // Increase residual for v's worst partner.
          v_worst_partner_data.residual += 1;

          // remove M[v_worst_partner] from M[v], and M[v] from
          // M[v_worst_partner]
          M->at(v).remove_least_preferred();
          M->at(v_worst_partner.vertex).remove(v);

          // add u and v to the matching
          u_data.residual -= 1;
          add_matched_partners(M, u, v, u_data, v_pref_list);

          // add v_worst_partner to free_list
          add_to_free_list(free_list, v_worst_partner_data,
                           v_worst_partner.vertex);
        }
      } else {
        u_data.residual -= 1;
        add_matched_partners(M, u, v, u_data, v_pref_list);
      }
    }

    // Activate u^(level+1).
    if (u_data.residual > 0 and u_data.level < max_level) {
      u_data.level += 1;
      u_data.begin = 0; // reset proposal index
      u_data.in_free_list = true;
      add_to_free_list(free_list, u);
    }
  }

  return M;
}
