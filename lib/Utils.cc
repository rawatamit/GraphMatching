#include "Utils.h"
#include "Vertex.h"
#include "PartnerList.h"
#include <set>
#include <sstream>

int to_integer(const std::string& s) {
    return std::strtol(s.c_str(), nullptr, 10);
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

void print_matching(const std::unique_ptr<BipartiteGraph>& G,
                    MatchedPairListType& M, std::ostream& out)
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
