#include "MatchingAlgorithm.h"
#include "Vertex.h"
#include "PartnerList.h"
#include <set>

MatchingAlgorithm::MatchingAlgorithm(const std::unique_ptr<BipartiteGraph>& G)
    : G_(G)
{}

MatchingAlgorithm::~MatchingAlgorithm()
{}

MatchedPairListType& MatchingAlgorithm::get_matched_pairs() {
    return M_;
}

const std::unique_ptr<BipartiteGraph>& MatchingAlgorithm::get_graph() const {
    return G_;
}

MatchedPairListType& MatchingAlgorithm::map_inverse(const MatchedPairListType& M) {
    // already computed matching, just return
    if (M_.size() > 0) { return M_; }

    const std::unique_ptr<BipartiteGraph>& G = get_graph();
    auto& A = G->get_A_partition();
    auto& B = G->get_B_partition();

    for (auto it : M) {
        auto a = it.first;

        // if a is not a dummy vertex
        if (not a->is_dummy()) {
            auto& partners = it.second;
            const auto& ac_id = a->get_cloned_for_id(); // vertex from which a was cloned
            // is A the partition to which a belongs in the original graph G
            bool is_a_partition_A = (A.find(ac_id) != A.end()) ? true : false;

            for (auto i = partners.begin(), e = partners.end(); i != e; ++i) {
                auto b = partners.get_vertex(i);

                // do not add a dummy partner to the matching
                if (not b->is_dummy()) {
                    const auto& bc_id = b->get_cloned_for_id();

                    // get the vertex from the original graph G
                    auto orig_a = is_a_partition_A ? A.at(ac_id) : B.at(ac_id);
                    auto orig_b = is_a_partition_A ? B.at(bc_id) : A.at(bc_id);

                    // get the rank of b in a's preference list
                    auto orig_a_pl = orig_a->get_preference_list();
                    auto b_rank = orig_a_pl.get_rank(orig_a_pl.find(orig_b));

                    // add to the matching
                    M_[orig_a].add_partner(std::make_pair(b_rank, orig_b));
                }
            }
        }
    }

    return M_;
}

