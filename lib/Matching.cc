#include "Matching.h"
#include "Partner.h"
#include "PartnerList.h"
#include "Utils.h"
#include "Vertex.h"
#include <algorithm>
#include <cassert>
#include <sstream>

void Matching::add_partner(VertexPtr u, const Partner &v, int level) {
  _matchedPairs[u].add_partner(v.vertex, v.rank, level);
}

void Matching::add_partner(VertexPtr u, VertexPtr v, RankType rank, int level) {
  _matchedPairs[u].add_partner(v, rank, level);
}

void Matching::set_uncertain_proposal(VertexPtr u, VertexPtr v, RankType rank, int level) {
  uncertain_proposals[v].push_back(Partner(u, rank, level));
}

void Matching::remove_partner(VertexPtr u, VertexPtr v) {
  _matchedPairs.at(u).remove(v);
  _matchedPairs.at(v).remove(u);
}

const PartnerList &Matching::get_partners(VertexPtr v) const {
  auto v_it = _matchedPairs.find(v);
  return (v_it == _matchedPairs.end()) ? _emptyPartnerList : v_it->second;
}

PartnerList::SizeType Matching::number_of_partners(VertexPtr v) const {
  return get_partners(v).size();
}

bool Matching::has_partner(VertexPtr v) const {
  return number_of_partners(v) > 0;
}

bool Matching::has_partners(VertexPtr v,
                            const std::vector<VertexPtr> &partners) const {
  const auto &M_v = get_partners(v);

  for (auto it = M_v.cbegin(); it != M_v.cend(); ++it) {
    if (std::find(partners.cbegin(), partners.cend(), it->vertex) ==
        partners.cend()) {
      return false;
    }
  }

  return M_v.size() == partners.size();
}

bool Matching::check_uncertain_proposal(VertexPtr b) const {
  return (uncertain_proposals.find(b) != uncertain_proposals.end());
}

VertexPtr Matching::get_partner(VertexPtr v) const {
  const PartnerList &partners(get_partners(v));
  assert(partners.size() == 1);
  return partners.get_partner();
}

bool Matching::is_matched_to(VertexPtr u, VertexPtr v) const {
  const auto &partners = get_partners(u);
  const auto &partner_it = partners.find(v);
  // True if v is present in u's partner list.
  return partner_it != partners.cend();
}

bool Matching::is_matched_to(VertexPtr u, VertexPtr v, int level) const {
  const auto &partners = get_partners(u);
  const auto &partner_it = partners.find(v);
  // If partner is present, check level matched to this partner is equal.
  return (partner_it != partners.cend()) ? (partner_it->level == level) : false;
}

bool Matching::is_matched_lt(VertexPtr u, int level) const {
  const auto &partners = get_partners(u);
  for (const auto& v : partners) {
    if (v.level < level) {
      return true;
    }
  }
  return false;
}

bool Matching::is_matched_lt(VertexPtr u, VertexPtr v, int level) const {
  const auto &partners = get_partners(u);
  const auto &partner_it = partners.find(v);
  // If partner is present, check level matched to this partner is equal.
  return (partner_it != partners.cend()) ? (partner_it->level < level) : false;
}

int Matching::size() const {
  int size = 0;
  for (const auto &it : _matchedPairs) {
    size += it.second.size();
  }
  return size / 2;
}

bool Matching::empty() const { return size() == 0; }

const Matching::MatchedPairListType &Matching::get_matched_pairs() const {
  return _matchedPairs;
}

std::ostream& operator<<(std::ostream& out, const Matching& M) {
  std::stringstream stmp;
  stmp << '{';

  for (const auto& [u, partners] : M._matchedPairs) {
    for (const auto& partner : partners) {
      stmp << '(' << u->get_id() << ',' << partner.vertex->get_id() << ')';
    }
  }

  stmp << '}';
  return out << stmp.str();
}

void Matching::generate_M_star(const BipartiteGraph::ContainerType &A_partition,
                               const BipartiteGraph::ContainerType &B_partition,
                               std::map<VertexCopy, VertexCopy> &M_star,
                               std::map<int, std::set<VertexCopy>> &A_levels,
                               std::map<int, std::set<VertexCopy>> &B_levels) const {
  // Store mapping from vertex to next available copy.
  std::map<VertexPtr, int> nextAvailableCopy;

  // Iterate over all vertices in B partition, and find their partners in M.
  // For each matched edge (a, b) in M, add an edge (a_i, b_j) in M* where i and
  // j have not been used so far.
  for (const auto &it : B_partition) {
    auto b = it.second;
    const auto &partners = get_partners(b);

    for (const auto &partner : partners) {
      auto a = partner.vertex;
      auto b_copy{std::make_pair(b, nextAvailableCopy[b])};
      auto a_copy{std::make_pair(a, nextAvailableCopy[a])};

      M_star[b_copy] = a_copy;
      M_star[a_copy] = b_copy;

      // Update next available copy for a and b.
      nextAvailableCopy[a] += 1;
      nextAvailableCopy[b] += 1;

      // Put u_copy and v_copy in appropriate sets. If b is matched to a level-1
      // vertex a in M, these copies are in A'_1 x B'_1, otherwise they are in
      // A'_0 x B'_0.
      if (partner.level == 0) {
        A_levels[0].insert(a_copy);
        B_levels[0].insert(b_copy);
      } else {
        A_levels[1].insert(a_copy);
        B_levels[1].insert(b_copy);
      }
    }
  }

  // All unmatched vertices in A are added to A'_1.
  for (const auto &it : A_partition) {
    auto a = it.second;
    // All a copies from nextAvailableCopy[a] to cap(a) are unmatched.
    for (auto i = nextAvailableCopy[a]; i < a->get_upper_quota(); ++i) {
      auto a_copy{std::make_pair(a, i)};

      // Add unmatched copy to A'_1, and set M*[a_i] = a_i.
      A_levels[1].insert(a_copy);
      M_star[a_copy] = a_copy;
    }
  }

  // All unmatched vertices in B are added to B'_0.
  for (const auto &it : B_partition) {
    auto b = it.second;
    // All b copies from nextAvailableCopy[b] to cap(b) are unmatched.
    for (auto i = nextAvailableCopy[b]; i < b->get_upper_quota(); ++i) {
      auto b_copy{std::make_pair(b, i)};

      // Add unmatched copy to B'_0, and set M*[b_i] = b_i.
      B_levels[0].insert(b_copy);
      M_star[b_copy] = b_copy;
    }
  }
}

bool Matching::verify(std::shared_ptr<BipartiteGraph> G) const {
  // This matching may have been computed either by vertices in set A proposing
  // or vertices in set B proposing. When decomposing cloned copy of vertices in
  // sets A'_0, A'_1, B'_0 and B'_1 we assume that vertices in set A propose.
  // This keeps the decomposition algorithm easier, when looking at the level
  // of the partner of a vertex. A_partition is initialized to the proposing set
  // and B_partition is the other set.
  const BipartiteGraph::ContainerType &A_partition =
      _isAProposing ? G->get_A_partition() : G->get_B_partition();
  const BipartiteGraph::ContainerType &B_partition =
      _isAProposing ? G->get_B_partition() : G->get_A_partition();

  // Type for M*.
  typedef std::map<VertexCopy, VertexCopy> M_star_type;

  // vote_a(b1=b, b2=M(a))
  // Get vote of a when b1 is compared to b2. b2 is always M(a). b1 can be any
  // other vertex, even a itself. This is useful when a is matched to itself in
  // M*.
  auto vote_in_G = [](VertexPtr a, VertexPtr b1, VertexPtr b2) {
    const auto &a_pref_list{a->get_preference_list()};
    // If a vertex is not found in the preference list, find_index returns the
    // last index + 1 in the array. For e.g. if a has two vertices in its
    // preference list then the index for a vertex which is not present in its
    // preference list is 2.
    auto rank_b1 = a_pref_list.find_index(b1);
    auto rank_b2 = a_pref_list.find_index(b2);

    // If a prefers b2=M*(a) over b1, then vote is -1. If instead it prefers b1
    // over b2, then vote is +1. If b1=b2 then a is indifferent.
    if (rank_b2 < rank_b1) {
      return -1;
    } else if (rank_b1 < rank_b2) {
      return +1;
    } else {
      return 0;
    }
  };

  // Helper function to find vote for a in G'_M. Vertex a looks at the relative
  // rank of b1 and b2 in its original preference list in G, and votes
  // accordingly.
  auto vote_in_G_M = [vote_in_G](VertexCopy a, VertexCopy b1, VertexCopy b2) {
    return vote_in_G(a.first, b1.first, b2.first);
  };

  // Edge weight for an edge (a_i, b_j) in G'_M is defined as the sum of
  // vote of a_i between b_i and M*(a_i), and vote of b_j between a_i and
  // M*(b_j). Note that the ranking is still based on original preference list
  // in G.
  auto edge_weight = [vote_in_G_M](VertexCopy a_copy, VertexCopy b_copy,
                                   const M_star_type &M_star) {
    // If this is an (a_i, a_i) edge, then vote a_i is -1 if a_i is matched in
    // M*, otherwise its vote is 0.
    if (a_copy == b_copy) {
      if (a_copy != M_star.at(a_copy)) {
        return -1;
      }

      return 0;
    }

    return vote_in_G_M(a_copy, b_copy, M_star.at(a_copy)) +
           vote_in_G_M(b_copy, a_copy, M_star.at(b_copy));
  };

  // Decompose vertices in G into sets A'_0, A'_1, B'_0, B'_1. To keep things
  // easier to write, we omit \' from variable name itself. These sets contain a
  // numbered copy of the original vertex, and a vertex u \in (A U B) will have
  // cap(u) copies in (A' U B'). M^* is a map between vertices in A' and B'.
  // Only M* is computed, M' can be deduced from M* if needed.
  std::map<int, std::set<VertexCopy>> A_levels;
  std::map<int, std::set<VertexCopy>> B_levels;
  M_star_type M_star;

  // Compute M* alongwith decomposition of (A' U B').
  generate_M_star(A_partition, B_partition, M_star, A_levels, B_levels);

  // We don't generate the complete graph G'_M. The edges in G'_M can be
  // carefully iterated over using M* and original graph G. If an edge is
  // present in the original matching M, then it must be present in M' and M*.
  // If an edge (a_i, b_j) \in E' is not present in M', then all edges
  // (a_1, b_1) ... (a_cap(a), b_cap(b)) are in G'_M.

  // Iterate over all edges of G. If an edge (a, b) is not present in M, then we
  // need to look at all (a_i, b_j) pairs where i \in {1,..,cap(a)} and j \in
  // {1,..,cap(b)}.
  for (auto &it : A_partition) {
    auto a = it.second;
    auto &pref_list = a->get_preference_list();

    for (auto it2 : pref_list) {
      auto b = it2.vertex;

      // If (a, b) is present in M, then a (a_i, b_j) copy is present in M'. We
      // can skip this edge, as it will be iterated over later.
      if (is_matched_to(a, b)) {
        continue;
      }

      // All edges (a_1, b_1) ... (a_cap(a), b_cap(b)) are in G'_M.
      for (auto i = 0; i < a->get_upper_quota(); ++i) {
        for (auto j = 0; j < b->get_upper_quota(); ++j) {
          // Depending on which sets a_i and b_j belong to, we verify that their
          // edge weight is correct.
          auto a_copy{std::make_pair(a, i)};
          auto b_copy{std::make_pair(b, j)};
          auto wt = edge_weight(a_copy, b_copy, M_star);

          // A'_0 x B'_1 edge, weight can be one of {-2, 0, +2}.
          if ((A_levels[0].find(a_copy) != A_levels[0].end()) and
              (B_levels[1].find(b_copy) != B_levels[1].end())) {
            if (wt != -2 and wt != 0 and wt != +2) {
              return false;
            }
          } else if ((A_levels[1].find(a_copy) != A_levels[1].end()) and
                     (B_levels[0].find(b_copy) != B_levels[0].end())) {
            // A'_1 x B'_0 edge, weight can only be -2.
            if (wt != -2) {
              return false;
            }
          } else {
            // A'_0 x B'_0 or A'_1 x B'_1 edge, weight can be one of {-2, 0}.
            if (wt != -2 and wt != 0) {
              return false;
            }
          }
        }
      }
    }
  }

  // Now iterate over all the edges in M*, this will complete the iteration on
  // edges in G'_M. Keep track of the number of unmatched vertices, which will
  // allow easy computation of sum of all dual variables.
  int unmatched = 0;
  for (auto &it : M_star) {
    auto a_copy = it.first;
    auto b_copy = it.second;
    auto a = a_copy.first;
    auto b = b_copy.first;

    // a_i is matched to self in M*. a_i is unmatched in M'.
    if (a_copy == b_copy) {
      unmatched += 1;

      // Edge weight for an unmatched vertex in original graph is 0.
      auto wt = edge_weight(a_copy, b_copy, M_star);
      if (wt != 0) {
        return false;
      }
    } else {
      // Edge (a, b) is present in M. This means (a_i, b_j) is present in M'.
      // An edge (a_i, b_j) \in M* has weight 0.
      auto wt = edge_weight(a_copy, b_copy, M_star);
      if (wt != 0) {
        return false;
      }

      // All matched edges are in (A'_0 x B'_0) or (A'_1 x B'_1). As we iterate
      // over every pair in M_star, we will iterate over every matched edge
      // twice, once (a_i, b_j) and then (b_j, a_i). Look at all possible
      // combinations of A' and B' to verify that we have a correct
      // decomposition of this edge.

      bool is_correct_partition = false;
      is_correct_partition |=
          ((A_levels[0].find(a_copy) != A_levels[0].end()) and
           (B_levels[0].find(b_copy) != B_levels[0].end()));
      is_correct_partition |=
          ((A_levels[1].find(a_copy) != A_levels[1].end()) and
           (B_levels[1].find(b_copy) != B_levels[1].end()));
      is_correct_partition |=
          ((A_levels[0].find(b_copy) != A_levels[0].end()) and
           (B_levels[0].find(a_copy) != B_levels[0].end()));
      is_correct_partition |=
          ((A_levels[1].find(b_copy) != A_levels[1].end()) and
           (B_levels[1].find(a_copy) != B_levels[1].end()));

      if (!is_correct_partition) {
        return false;
      }
    }
  }

  // Each vertex u \in A' U B' has a dual variable with a value of {-1, +1}.
  // All dual variables corresponding to vertices in A'_0 and B'_1 have value of
  // +1, and all other dual variables have a value of -1. All dual variables for
  // unmatched vertices have value 0. If we add the size of A'_1 and B'_0 to
  // dual sum we will also count dual variables for umatched vertices, hence
  // dual sum is incremented by unmatched vertices.
  auto total_dual_sum = (int)A_levels[0].size() + (int)B_levels[1].size() -
                        (int)B_levels[0].size() - (int)A_levels[1].size() +
                        unmatched;
  return total_dual_sum == 0;
}
