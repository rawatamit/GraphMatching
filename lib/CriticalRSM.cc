#include "CriticalRSM.h"
#include "MatchingAlgorithm.h"
#include "VertexBookkeeping.h"
#include "Utils.h"
#include "Vertex.h"
#include <iostream>

CriticalRSM::CriticalRSM(std::shared_ptr<BipartiteGraph> G, bool A_proposing)
    : MatchingAlgorithm(G, A_proposing)
{}

Matching CriticalRSM::compute_matching() {
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
    int pref_list_size = v->get_preference_list().size();
    int pref_list_lq_size = v->get_preference_list_lq().size();
    auto vertex_data{VertexBookkeeping(0, pref_list_size,
                                       0, pref_list_lq_size)};
    bookkeep_data[v] = vertex_data;
    free_list.push(v);
  }

  // Make prefSC and prefS for each vertex
  std::unordered_map<VertexPtr, PreferenceList> prefS;
  std::unordered_map<VertexPtr, PreferenceList> prefSC;
  std::unordered_map<VertexPtr, RankType> prefS_proposal_index;
  std::unordered_map<VertexPtr, RankType> prefSC_proposal_index;
  
  for (auto &it: proposing_partition) {
    auto a = it.second;
    auto pref_list_a = a->get_preference_list();
    prefS[a] = pref_list_a.get_prefS();
    prefSC[a] = pref_list_a.get_prefSC();
    prefS_proposal_index[a] = 0;
    prefSC_proposal_index[a] = 0;
  }

  for (auto &it: non_proposing_partition) {
    auto b = it.second;
    auto pref_list_b = b->get_preference_list();
    prefS[b] = pref_list_b.get_prefS();
    prefSC[b] = pref_list_b.get_prefSC();
    prefS_proposal_index[b] = 0;
    prefSC_proposal_index[b] = 0;
  }

  while (not free_list.empty()) {
    auto u = remove_from_free_list(free_list, bookkeep_data);
    const auto &u_pref_list = u->get_preference_list();
    auto &u_data = bookkeep_data[u];
    auto l = u_data.level;
    bool star = u_data.star;
    if (l < t) {
      // if u^l has not exhausted PrefSC(u) then CriticalPropose(u^l, PrefSC(u),M,Q)
      if (prefSC_proposal_index[u] < prefSC[u].size()) {
        critical_propose(free_list, u, prefSC[u], bookkeep_data, M);
        prefSC_proposal_index[u] += 1;
      }
      else {
        u_data.level += 1;
        u_data.begin = 0;
        prefS_proposal_index[u] = 0;
        prefSC_proposal_index[u] = 0;
        add_to_free_list(free_list, u);
      }
    } else if (l == t) {
      // if there exists a b' that is marked or unproposed by a^l
      if ((u_data.marked.size() > 0) || (!u_data.is_exhausted())) {
        ties_propose(free_list, u, u_pref_list, bookkeep_data, M, t);
        if (!u_data.is_exhausted()) {
          u_data.begin += 1;
        }
      } else {
        if (l == t && !star) {
          u_data.star = true;
          add_to_free_list(free_list, u);
        } else {
          bool is_critical = (u->get_lower_quota() > 0);
          if (((l == t) && star) && is_critical) {
            u_data.level = t + 1;
            u_data.begin = 0;
            prefS_proposal_index[u] = 0;
            prefSC_proposal_index[u] = 0;
            add_to_free_list(free_list, u);
          }
        }
      }
    } else {
      // u is critical
      // if u^l has not exhausted PrefS(u) then CriticalPropose(u^l, PrefSC(u),M,Q)
      if (prefS_proposal_index[u] < prefS[u].size()) {
        critical_propose(free_list, u, prefS[u], bookkeep_data, M);
        prefS_proposal_index[u] += 1;
      }
      else {
        bool is_critical = (u->get_lower_quota() > 0);
        if (l < (s + t) && is_critical) {
          u_data.level += 1;
          u_data.begin = 0;
          prefS_proposal_index[u] = 0;
          prefSC_proposal_index[u] = 0;
          add_to_free_list(free_list, u);
        }
      }
    }
  }
  bool check = verify_if_rsm(M);
  if (!check) {
    std::cout << "Error in generating a critical RSM\n";
  }

  return M;
}

void CriticalRSM::critical_propose(FreeListType& free_list, VertexPtr a, const PreferenceList& a_pref_list_s, std::map<VertexPtr, VertexBookkeeping>& bookkeep_data, Matching& M) {
  auto a_data = bookkeep_data[a];
  // b is the most preferred unproposed vertex by a^l in preference_list_a_s
  // a_pref_list_s is always strict 
  VertexPtr b = a_pref_list_s.at(a_data.begin).vertex;
  auto l = a_data.level;
  auto b_pref_list = b->get_preference_list();
  // if b is unmatched in M
  if (!M.has_partner(b)) {
    add_matched_partners(M, a, b, a_data, b_pref_list);
  } else {
    auto aj = M.get_partner(b);
    auto aj_data = bookkeep_data[aj];
    auto y =  aj_data.level;
    if((l > y) || ((l == y) && b_pref_list.prefers(a, aj))) {
      M.remove_partner(aj, b);
      add_matched_partners(M, a, b, a_data, b_pref_list);
      add_to_free_list(free_list, aj);
    } else {
      add_to_free_list(free_list, a);
    }
  }
}

void CriticalRSM::ties_propose(FreeListType& free_list, VertexPtr a, const PreferenceList& a_pref_list, std::map<VertexPtr, VertexBookkeeping>& bookkeep_data, Matching& M, const int t) {
  auto a_data = bookkeep_data[a];
  auto b = favourite_neighbour(a, a_pref_list, a_data, M);
  auto k = compute_rank(b, a_pref_list) - 1;
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
    if (check) {
      M.set_uncertain_proposal(a, b, compute_rank(a, b_pref_list), a_data.level);
    }
  } else if (M.check_uncertain_proposal(b)) {
    auto aj = M.get_partner(b);
    M.remove_partner(aj, b);
    auto aj_data = bookkeep_data[aj];
    aj_data.marked[b] = true;
    add_matched_partners(M, a, b, a_data, b_pref_list);
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

bool CriticalRSM::verify_if_rsm(Matching& M) {
  std::shared_ptr<BipartiteGraph> G = get_graph();
  // choose the partitions from which the vertices will propose
  const auto& proposing_partition = is_A_proposing() ? G->get_A_partition()
                                                     : G->get_B_partition();

  // check every edge
  for (auto &it: proposing_partition) {
    auto a = it.second;
    auto pref_list_a = a->get_preference_list();
    auto prefS = pref_list_a.get_prefS();
    // check if a is matched
    bool matched = M.has_partner(a);
    VertexPtr a_partner = nullptr;
    if (matched) {
      a_partner = M.get_partner(a);
    }
    for (auto b: prefS) {
      auto pref_list_b = b.vertex->get_preference_list();
      // check if a-b is a blocking edge
      matched = M.has_partner(b.vertex);
      VertexPtr b_partner = nullptr;
      if (matched) {
        b_partner = M.get_partner(b.vertex);
      }
      if (a_partner == b.vertex) {
        continue;
      } else {
        bool blocking_a = ((a_partner == nullptr) || (compute_rank(b.vertex, pref_list_a) < compute_rank(a_partner, pref_list_a)));
        bool blocking_b = ((b_partner == nullptr) || (compute_rank(a, pref_list_b) < compute_rank(b_partner, pref_list_b)));
        if (blocking_a && blocking_b) {
          if (a_partner != nullptr && a_partner->get_lower_quota() == 1) {
            continue;
          }
          if (b_partner != nullptr && b_partner->get_lower_quota() == 1) {
            continue;
          }
          return false;
        }
      }
    }
  }
  return true;
}