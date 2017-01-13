#include "MatchingAlgorithm.h"
#include "Vertex.h"
#include <sstream>

MatchingAlgorithm::MatchingAlgorithm(const std::unique_ptr<BipartiteGraph>& G)
    : G_(G)
{}

MatchingAlgorithm::~MatchingAlgorithm()
{}

MatchingAlgorithm::MatchedPairListType& MatchingAlgorithm::get_matched_pairs() {
    // no need to recompute pairs if already done
    if (matched_pairs_.size() > 0) {
        return matched_pairs_;
    }

    for (auto it : G_->get_A_partition()) {
        auto& m = it.second;
        auto& partner_list = m->get_partners();
        for (auto i = partner_list.begin(), e = partner_list.end(); i != e; ++i) {
            matched_pairs_.emplace_back(m, partner_list.get_vertex(i));
        }
    }

    return matched_pairs_;
}

const std::unique_ptr<BipartiteGraph>& MatchingAlgorithm::get_graph() const {
    return G_;
}

std::ostream& operator<<(std::ostream& out,
        MatchingAlgorithm::MatchedPairListType& matched_pairs) {
    std::stringstream stmp;

    stmp << '{';
    for (MatchingAlgorithm::MatchedPairListType::iterator
            i = matched_pairs.begin(),
            e = matched_pairs.end();
         i != e; ++i)
    {
        auto mpair = *i;
        stmp << "(" << mpair.first->get_id() << ", "
             << mpair.second->get_id() << "}";
        if (i + 1 != e) {
            stmp << ",";
        } else {
            stmp << "}";
        }
    }

    return out << stmp.str();
}
