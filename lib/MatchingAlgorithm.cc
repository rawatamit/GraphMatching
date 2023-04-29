#include "MatchingAlgorithm.h"
#include "Vertex.h"
#include "PartnerList.h"
#include <set>

MatchingAlgorithm::MatchingAlgorithm(std::shared_ptr<BipartiteGraph> G, bool A_proposing)
    : G_(std::move(G)), A_proposing_(A_proposing)
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
