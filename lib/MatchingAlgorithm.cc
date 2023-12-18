#include "MatchingAlgorithm.h"
#include "NProposingMatching.h"
#include "NProposingTiesMatching.h"
#include "CriticalRSM.h"
#include "Popular.h"
#include "Partner.h"
#include "Utils.h"
#include "Vertex.h"
#include "PartnerList.h"
#include <iterator>
#include <set>

MatchingAlgorithm::MatchingAlgorithm(std::shared_ptr<BipartiteGraph> G, bool A_proposing)
    : G_(G), A_proposing_(A_proposing)
{}

std::shared_ptr<BipartiteGraph> MatchingAlgorithm::get_graph() const {
    return G_;
}

bool MatchingAlgorithm::is_A_proposing() {
    return A_proposing_;
}

bool MatchingAlgorithm::is_feasible(std::shared_ptr<BipartiteGraph> G,
                                    const Matching& M) {
  auto feasible_for_vertices = [&M] (const BipartiteGraph::ContainerType& vertices) {
      for (auto& it : vertices) {
          auto v = it.second;
          unsigned uq = v->get_upper_quota();
          unsigned lq = v->get_lower_quota();
          auto nmatched = M.number_of_partners(v);

          if (nmatched < lq or nmatched > uq) {
              return false;
          }
      }

      return true;
    };

    return feasible_for_vertices(G->get_A_partition()) and
           feasible_for_vertices(G->get_B_partition());
}

Matching MatchingAlgorithm::map_inverse(const Matching& M) const {
    std::shared_ptr<BipartiteGraph> G = get_graph();
    auto& A = G->get_A_partition();
    auto& B = G->get_B_partition();
    auto M_ = Matching();

    for (auto& it : M.get_matched_pairs()) {
        auto a = it.first;

        // if a is not a dummy vertex
        if (not a->is_dummy()) {
            const auto& partners = it.second;
            const auto& ac_id = a->get_cloned_for_id(); // vertex from which a was cloned
            // is A the partition to which a belongs in the original graph G
            bool is_a_partition_A = A.find(ac_id) != A.end();

            for (auto i = partners.cbegin(), e = partners.cend(); i != e; ++i) {
                auto b = i->vertex;

                // do not add a dummy partner to the matching
                if (not b->is_dummy()) {
                    const auto& bc_id = b->get_cloned_for_id();

                    // get the vertex from the original graph G
                    auto orig_a = is_a_partition_A ? A.at(ac_id) : B.at(ac_id);
                    auto orig_b = is_a_partition_A ? B.at(bc_id) : A.at(bc_id);

                    // get the rank of b in a's preference list
                    auto orig_a_pl = orig_a->get_preference_list();
                    auto b_rank = orig_a_pl.find(orig_b)->rank;

                    // add to the matching
                    M_.add_partner(orig_a, orig_b, b_rank, 0);
                }
            }
        }
    }

    return M_;
}

VertexPtr MatchingAlgorithm::remove_from_free_list
(FreeListType& free_list, std::map<VertexPtr, VertexBookkeeping>& bookkeep_data) {
    auto ret = free_list.top();
    free_list.pop(); // remove u from free_list
    bookkeep_data[ret].in_free_list = false;
    return ret;
}

void MatchingAlgorithm::add_to_free_list(FreeListType& free_list, VertexPtr u) {
    free_list.push(u);
}

void MatchingAlgorithm::add_to_free_list(NProposingMatching::FreeListType &free_list,
                                          VertexBookkeeping &u_data, VertexPtr u) {
    if (not u_data.in_free_list) {
        u_data.in_free_list = true;
        add_to_free_list(free_list, u);
    }
}

void MatchingAlgorithm::add_matched_partners(Matching& M,
                                              VertexPtr u, VertexPtr v,
                                              const VertexBookkeeping& u_data,
                                              const PreferenceList& v_pref_list) {
    // v is the first vertex on u's current preference list
    // invariant for non proposing vertices: rank = index + 1
    // v is always at level 0
    M.add_partner(u, v, (RankType) u_data.begin + 1, 0);
    M.add_partner(v, u, compute_rank(u, v_pref_list), u_data.level);
}

Matching NProposingMatching::compute_matching() {
  FreeListType free_list;
  std::map<VertexPtr, VertexBookkeeping> bookkeep_data;
  std::shared_ptr<BipartiteGraph> G = get_graph();
  auto M = Matching(is_A_proposing());
  
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

      if (u_data.level > 0 and M.is_matched_to(v, u, u_data.level - 1)) {
        // Remove u^(level-1) and v from the matching.
        M.remove_partner(u, v);

        // Add u^level and v to the matching. Note that residual(u) doesn't
        // change, as we are replacing one vertex with another.
        add_matched_partners(M, u, v, u_data, v_pref_list);
      } else if (M.number_of_partners(v) == v->get_upper_quota()) {
        // |M[v]| >= upper_quota(v)
        const auto& v_all_partners = M.get_partners(v);
        auto v_worst_partner = v_all_partners.get_least_preferred();
        auto possible_partner =
            Partner(u, compute_rank(u, v_pref_list), u_data.level);

        if (v_worst_partner < possible_partner) {
          auto &v_worst_partner_data = bookkeep_data[v_worst_partner.vertex];

          // Increase residual for v's worst partner.
          v_worst_partner_data.residual += 1;

          // remove M[v_worst_partner] from M[v], and M[v] from
          // M[v_worst_partner]
          M.remove_partner(v, v_worst_partner.vertex);

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

Matching MaxCardPopularLQ::compute_matching() {
  FreeListType free_list;
  std::map<VertexPtr, VertexBookkeeping> bookkeep_data;
  std::shared_ptr<BipartiteGraph> G = get_graph();
  auto M = Matching(is_A_proposing());
  
  // choose the partitions from which the vertices will propose
  const auto& proposing_partition = is_A_proposing() ? G->get_A_partition()
                                                     : G->get_B_partition();

  const auto& non_proposing_partition = is_A_proposing() ? G->get_B_partition()
                                                         : G->get_A_partition();

  // s is sum of lower quotas of proposing vertices.
  auto s = 0;
  for (const auto& [_, v] : proposing_partition) {
    s += v->get_lower_quota();
  }

  // t is sum of lower quotas of non-proposing vertices.
  auto t = 0;
  for (const auto& [_, v] : non_proposing_partition) {
    t += v->get_lower_quota();
  }

  // set the level of every vertex in the proposing partition to 0
  // mark all proposing vertices free (by pushing into the free_list)
  // and vertices from the opposite partition implicitly free
  for (const auto& [_, v] : proposing_partition) {
    //free_list.push(v);
    int pref_list_size = v->get_preference_list().size();
    int pref_list_lq_size = v->get_preference_list_lq().size();
    auto vertex_data{VertexBookkeeping(0, pref_list_size,
                                       0, pref_list_lq_size)};
    bookkeep_data[v] = vertex_data;
    add_to_free_list(free_list, vertex_data, v);
  }

  while (!free_list.empty()) {
    // arbitrary vertex in free list
    auto u = remove_from_free_list(free_list, bookkeep_data);
    auto &u_data = bookkeep_data[u];
    auto l = u_data.level;

    if (l < t) {
      if (!u_data.is_exhausted_lq()) {
        const auto &pref_list_lq = u->get_preference_list_lq();
        auto v = pref_list_lq.at(u_data.begin_lq).vertex;
        u_data.begin_lq += 1;
        const auto& v_pref_list = v->get_preference_list();
        decide_accept_reject(u, u->get_upper_quota(),
                             v, v->get_lower_quota(),
                             u_data, v_pref_list, free_list,
                             bookkeep_data, M);
      } else {
        u_data.level += 1;
        // reset proposal index
        u_data.begin = 0;
        u_data.begin_lq = 0;
        u_data.in_free_list = true;
        add_to_free_list(free_list, u);
      }
    } else {
      auto cap_u = 0;
      if ((l == t) or (l == t+1)) {
        cap_u = u->get_upper_quota();
      } else {
        cap_u = u->get_lower_quota();
      }

      if (!u_data.is_exhausted()) {
        const auto &pref_list = u->get_preference_list();
        auto v = pref_list.at(u_data.begin).vertex;
        u_data.begin += 1;
        const auto& v_pref_list = v->get_preference_list();

        if (M.number_of_partners(v) < v->get_lower_quota()) {
          decide_accept_reject(u, cap_u, v, v->get_lower_quota(),
              u_data, v_pref_list, free_list,
              bookkeep_data, M);
        } else if (M.number_of_partners(v) == v->get_lower_quota()) {
          if (M.is_matched_lt(v, t)) {
            decide_accept_reject(u, cap_u, v, v->get_lower_quota(),
                u_data, v_pref_list, free_list,
                bookkeep_data, M);
          } else {
            decide_accept_reject(u, cap_u, v, v->get_upper_quota(),
                u_data, v_pref_list, free_list,
                bookkeep_data, M);
          }
        } else {
          decide_accept_reject(u, cap_u, v, v->get_upper_quota(),
              u_data, v_pref_list, free_list,
              bookkeep_data, M);
        }
      } else if (((l < s+t+1) and
                  (M.number_of_partners(u) < u->get_lower_quota())) or
                 (l == t)) {
        u_data.level += 1;
        // reset proposal index
        u_data.begin = 0;
        u_data.begin_lq = 0;
        u_data.in_free_list = true;
        add_to_free_list(free_list, u);
      }
    }
  }

  return M;
}

#define DEBUG 0
#include <sstream>

void MaxCardPopularLQ::decide_accept_reject
  (VertexPtr u, int cap_u, VertexPtr v, int cap_v,
   VertexBookkeeping& u_data,
   const PreferenceList& v_pref_list, FreeListType& free_list,
   std::map<VertexPtr, VertexBookkeeping>& bookkeep_data,
   Matching& M)
{
#if DEBUG
std::stringstream stmp;
stmp << u->get_id() << '^' << u_data.level << ':' << cap_u
     << "->" << v->get_id() << ':' << cap_v << " :: ";
#endif

  auto v_num_matched = M.number_of_partners(v);

  if (M.is_matched_lt(v, u, u_data.level)) {
    // Remove u^x and v from the matching.
    M.remove_partner(u, v);

    // Add u^level and v to the matching. Note that residual(u) doesn't
    // change, as we are replacing one vertex with another.
    add_matched_partners(M, u, v, u_data, v_pref_list);
  } else if (v_num_matched < cap_v) {
    add_matched_partners(M, u, v, u_data, v_pref_list);
  } else if (v_num_matched == cap_v) {
    const auto& v_all_partners = M.get_partners(v);
    auto v_worst_partner = v_all_partners.get_least_preferred();
    auto possible_partner =
        Partner(u, compute_rank(u, v_pref_list), u_data.level);

    if (v_worst_partner < possible_partner) {
      auto &v_worst_partner_data = bookkeep_data[v_worst_partner.vertex];

      // remove M[v_worst_partner] from M[v], and M[v] from
      // M[v_worst_partner]
      M.remove_partner(v, v_worst_partner.vertex);

      // add u and v to the matching
      add_matched_partners(M, u, v, u_data, v_pref_list);

#if DEBUG
stmp << "rej " << v_worst_partner.vertex->get_id() << " :: ";
#endif

      // add v_worst_partner to free_list
      add_to_free_list(free_list, v_worst_partner_data,
                       v_worst_partner.vertex);
    }
  }

  auto u_num_matched = M.number_of_partners(u);
  if ((u_num_matched < cap_u) and !u_data.in_free_list) {
#if DEBUG
stmp << "toqueue " << u->get_id() << " :: ";
#endif
    u_data.in_free_list = true;
    add_to_free_list(free_list, u);
  }

#if DEBUG
FreeListType debug_list;
stmp << " flist [";
while (!free_list.empty()) {
  debug_list.push(free_list.top());
  stmp << debug_list.top()->get_id() << ',';
  free_list.pop();
}
while (!debug_list.empty()) {
  free_list.push(debug_list.top());
  debug_list.pop();
}
stmp << "] ";
stmp << M << '\n';
std::cout << stmp.str();
#endif
}

Matching NProposingTiesMatching::compute_matching() {
  FreeListType free_list; 
  std::map<VertexPtr, VertexBookkeeping> bookkeep_data;
  std::shared_ptr<BipartiteGraph> G = get_graph();
  auto M = Matching(is_A_proposing());
  
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
    bookkeep_data[v] = VertexBookkeeping(0, pref_list_size, 0, residual);
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
      VertexPtr v;
      if (u_pref_list.is_tied(u_data.begin)) {
        auto ties = u_pref_list.get_ties(u_data.begin);
        v = ties[u_data.tied_index].vertex;
      } else {
        v = u_pref_list.at(u_data.begin).vertex;
      }
      const auto &v_pref_list = v->get_preference_list();

      if (u_data.level > 0 and M.is_matched_to(v, u, u_data.level - 1)) {
        // Remove u^(level-1) and v from the matching.
        M.remove_partner(u, v);

        // Add u^level and v to the matching. Note that residual(u) doesn't
        // change, as we are replacing one vertex with another.
        add_matched_partners(M, u, v, u_data, v_pref_list);
      } else if (M.number_of_partners(v) == v->get_upper_quota()) {
        // |M[v]| >= upper_quota(v)
        const auto& v_all_partners = M.get_partners(v);
        auto v_worst_partner = v_all_partners.get_least_preferred();
        auto possible_partner =
            Partner(u, compute_rank(u, v_pref_list), u_data.level);

        if (v_worst_partner < possible_partner) {
          auto &v_worst_partner_data = bookkeep_data[v_worst_partner.vertex];

          // Increase residual for v's worst partner.
          v_worst_partner_data.residual += 1;

          // remove M[v_worst_partner] from M[v], and M[v] from
          // M[v_worst_partner]
          M.remove_partner(v, v_worst_partner.vertex);

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
      if (u_pref_list.is_tied(u_data.begin)) {
        if(u_data.tied_index < u_pref_list.get_ties(u_data.begin).size() - 1) {
          u_data.tied_index += 1;
        } else {
          u_data.begin += 1;
          u_data.tied_index = 0;
        }
      } else {
        u_data.begin += 1;
      }
    }

    // Activate u^(level+1).
    if (u_data.residual > 0 and u_data.level < max_level_) {
      u_data.level += 1;
      u_data.begin = 0; // reset proposal index
      u_data.tied_index = 0;
      u_data.in_free_list = true;
      add_to_free_list(free_list, u);
    }
  }

  return M;
}

VertexPtr CriticalRSM::favourite_neighbour(VertexPtr u, const PreferenceList& u_pref_list, VertexBookkeeping &u_data, const Matching& M) {
  // k is the best rank at which some
  // unproposed or marked neighbours of u exist
  // u_data.begin gives the highest rank at which an unproposed vertex exists
  auto k = u_data.begin;

  // check if a marked neighbour of u exists at a rank <= k
  bool flag = false;
  for (int i = 0; i <= k; i++) {
    if (!u_pref_list.is_tied(i)) {  // if the ith rank is not tied
      auto v = (u_pref_list.at(i)).vertex;
      if (u_data.marked[v]) {
        k = i;
        break;
      }
    } else {
      auto tied_list_i = u_pref_list.get_ties(i);
      for (auto j: tied_list_i) {
        auto v = j.vertex;
        if (u_data.marked[v]) {
          k = i;
          flag = true;
          break;
        }
      }
      if(flag) break;
    }
  }

  // Case 1: there exists at least one unmatched neighbour of a at the kth rank 
  // and b has the lowest index among all such unmatched neighbours
  if (!u_pref_list.is_tied(k)) {  // case 1.1: kth rank is not tied
    auto v = (u_pref_list.at(k)).vertex;
    if (!M.has_partner(v)) {
      return v;
    }
  } else {                        // case 1.2: kth rank is tied
    auto tied_list_k = u_pref_list.get_ties(k);
    for (auto i: tied_list_k){
      auto v = i.vertex;
      if (!M.has_partner(v)) {
        return v;
      }
    }
  }

  // Case 2: all the kth-ranked neighbours of u are matched and v is 
  // the lowest index among all such neighbours which are unproposed by u
  if (!u_pref_list.is_tied(k)) {  // case 1.1: kth rank is not tied
    auto v = (u_pref_list.at(k)).vertex;
    if (k >= u_data.begin) {
      return v;
    }
  } else {                        // case 1.2: kth rank is tied
    auto tied_list_k = u_pref_list.get_ties(k);
    int d = 0;
    for (auto i: tied_list_k) {
      auto v = i.vertex;
      if (k == u_data.begin && d >= u_data.tied_index) {
        return v;
      }
      d += 1;
    }
  }

  // Case 3: all the kth-ranked neighbours are already proposed by u and 
  // v has the lowest index among all the vertices which are marked by u
  if (!u_pref_list.is_tied(k)) {  // case 1.1: kth rank is not tied
    auto v = (u_pref_list.at(k)).vertex;
    if (u_data.marked[v]) {
      return v;
    }
  } else {                        // case 1.2: kth rank is tied
    auto tied_list_k = u_pref_list.get_ties(k);
    for (auto i: tied_list_k) {
      auto v = i.vertex;
      if (u_data.marked[v]) {
        return v;
      }
    }
  }

  return nullptr;
}

void CriticalRSM::ties_propose(FreeListType& free_list, VertexPtr a, PreferenceList& a_pref_list, std::map<VertexPtr, VertexBookkeeping>& bookkeep_data, Matching M, int t) {
  auto a_data = bookkeep_data[a];
  auto b = favourite_neighbour(a, a_pref_list, a_data, M);
  auto k = compute_rank(b, a_pref_list);
  if (a_data.marked[b]) {
    a_data.marked.erase(b);
  }
  auto b_pref_list = b->get_preference_list();
  if (!M.has_partner(b)) {
    add_matched_partners(M, a, b, a_data, b_pref_list);
    bool check = false;
    if (a_pref_list.is_tied(k)) {
      auto tied_list = a_pref_list.get_ties(k);
      for (auto i: tied_list) {
        if (!M.has_partner(i.vertex)) {
          check = true;
        }
      }
    }
    if (check) M.set_uncertain_proposal(a, b, compute_rank(a, b_pref_list), a_data.level);
  } else if (M.check_uncertain_proposal(b)) {
    auto aj = M.get_partner(b);
    M.remove_partner(aj, b);
    auto aj_data = bookkeep_data[aj];
    aj_data.marked[b] = true;
    add_to_free_list(free_list, aj);
  } else {
    auto aj = M.get_partner(b);
    auto aj_data = bookkeep_data[aj];
    auto y = aj_data.level;
    auto star = aj_data.star;
    auto pref_list_b = b->get_preference_list();
    auto preference = pref_list_b.prefers(a, aj);

    if (a_data.level == t) {
      bool isBetter = (preference == cBetter);
      bool tStar = (y == t) && star;
      if (y < t || (((y == t) || tStar) && isBetter)) {
        M.remove_partner(aj, b);
        add_matched_partners(M, a, b, a_data, b_pref_list);
        add_to_free_list(free_list, aj);
      } else {
        add_to_free_list(free_list, a);
      }
    }

    if (a_data.level == t && a_data.star) {
      bool isBetter = (preference == cBetter);
      bool isBetterOrEqual = isBetter || (preference == cEqual);
      bool tStar = (y == t) && star;
      if (y < t || (y == t && isBetterOrEqual) || (tStar && isBetter)) {
        M.remove_partner(aj, b);
        add_matched_partners(M, a, b, a_data, b_pref_list);
        add_to_free_list(free_list, aj);
      } else {
        add_to_free_list(free_list, a);
      }
    }
  }
}

