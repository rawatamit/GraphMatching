#include "MatchingAlgorithm.h"
#include "Vertex.h"

MatchingAlgorithm::MatchingAlgorithm(const std::unique_ptr<BipartiteGraph>& G)
    : G_(G)
{}

MatchingAlgorithm::~MatchingAlgorithm()
{}

void MatchingAlgorithm::compute_matching() {
    matched_pairs_.clear();

    for (auto m : G_->get_A_partition()) {
        if (m->is_matched()) {
            matched_pairs_.emplace_back(m, m->get_partner());
        }
    }
}

const std::unique_ptr<BipartiteGraph>& MatchingAlgorithm::get_graph() const {
    return G_;
}

MatchingAlgorithm::MatchedPairListType const& MatchingAlgorithm::get_matched_pairs() const {
    return matched_pairs_;
}
