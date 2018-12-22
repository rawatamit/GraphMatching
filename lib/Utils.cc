#include "Utils.h"
#include "Vertex.h"
#include "GraphReader.h"
#include <set>
#include <sstream>

int to_integer(const std::string& s) {
    return (int) std::strtol(s.c_str(), nullptr, 10);
}

PartnerList get_partners(const MatchingAlgorithm::MatchedPairListType& M, VertexPtr v) {
    auto v_it = M.find(v);
    return v_it == M.end() ? PartnerList() : v_it->second;
}

PartnerList::SizeType number_of_partners(const MatchingAlgorithm::MatchedPairListType& M, VertexPtr v) {
    return get_partners(M, v).size();
}

bool has_partner(const MatchingAlgorithm::MatchedPairListType& M, VertexPtr v) {
    return number_of_partners(M, v) > 0;
}

VertexPtr get_partner(const MatchingAlgorithm::MatchedPairListType& M, VertexPtr v) {
    return get_partner(get_partners(M, v));
}

VertexPtr get_partner(const PartnerList& partner_list) {
    if (partner_list.empty()) {
        return nullptr;
    } else {
        return partner_list.get_least_preferred().vertex;
    }
}

int matching_size(const MatchingAlgorithm::MatchedPairListType& M) {
    int size = 0;

    for (auto& it : M) {
        size += it.second.size();
    }

    return size / 2;
}

VertexPtr get_vertex_by_id(const std::unique_ptr<BipartiteGraph>& G, const IdType& id) {
    auto A_it = G->get_A_partition().find(id);

    return A_it == G->get_A_partition().end() ? G->get_B_partition().find(id)->second : A_it->second;
}

std::unique_ptr<BipartiteGraph> read_graph(const std::string& filepath) {
    return GraphReader(filepath.c_str()).read_graph();
}

// a new id is of the form id^k
IdType get_vertex_id(const IdType& id, int k) {
    std::stringstream stmp;
    stmp << id << '^' << k;
    return stmp.str();
}

int get_vertex_level(const IdType& id) {
    return to_integer(id.substr(id.find('^')+1));
}

// a dummy id is of the form d^k_id
IdType get_dummy_id(const IdType& id, int k) {
    std::stringstream stmp;
    stmp << "d^" << k << '_' << id;
    return stmp.str();
}

// return the dummy level from the given id
int get_dummy_level(const IdType& id) {
    auto caret_pos = id.find('^');
    auto underscore_pos = id.find('_');
    return to_integer(id.substr(caret_pos+1, underscore_pos-caret_pos-1));
}

RankType compute_rank(VertexPtr u, const PreferenceList& pref_list, int level) {
    auto index = pref_list.find_index(u);
    return (RankType) ((index + 1) + (level * pref_list.size()));
}

void print_matching(const std::unique_ptr<BipartiteGraph>& G,
                    MatchingAlgorithm::MatchedPairListType& M, std::ostream& out)
{
    std::stringstream stmp;
    std::set<VertexPtr> printed;

    for (auto it : G->get_A_partition()) {
        auto u = it.second;
        auto M_u = M.find(u);

        if (M_u != M.end()) {
            auto& partners = M_u->second;

            for (auto i = partners.cbegin(), e = partners.cend(); i != e; ++i) {
                auto v = i->vertex;
                printed.emplace(v);

                stmp << u->get_id() << ','
                     << v->get_id() << ','
                     << i->rank << '\n';
            }
        }
    }

#if 0
    for (auto it : G->get_B_partition()) {
        auto u = it.second;
        auto M_u = M.find(u);

        if (M_u != M.end()) {
            auto& partners = M_u->second;

            for (auto pit = partners.cbegin(), pie = partners.cend(); pit != pie; ++pit) {
                auto v = partners.get_vertex(pit);
                printed.emplace(v);

                stmp << u->get_id() << ','
                     << v->get_id() << ','
                     << partners.get_rank(pit) << '\n';
            }
        }
    }
#endif

    out << stmp.str();
}
