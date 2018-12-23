#include "BipartiteGraph.h"
#include "Vertex.h"
#include "PartnerList.h"
#include <set>
#include <queue>
#include <sstream>

BipartiteGraph::BipartiteGraph(const ContainerType& A, const ContainerType& B)
    : A_(A), B_(B)
{}

const BipartiteGraph::ContainerType& BipartiteGraph::get_A_partition() const {
    return A_;
}

const BipartiteGraph::ContainerType& BipartiteGraph::get_B_partition() const {
    return B_;
}

/*
bool BipartiteGraph::has_augmenting_path(const MatchedPairListType& M) const {
    // the second entry of the pair is true if
    // the vertex belongs to partition A, otherwise false
    std::queue<std::pair<VertexPtr, bool>> Q;
    std::set<VertexPtr> visited;

    // populate the queue with vertices from A
    // which can participate in an augmenting path
    // these are precisely the vertices that are unmatched
    for (auto it : get_A_partition()) {
        auto u = it.second;
        auto uit = M.find(u);

        if (uit == M.cend() or uit->second.size() == 0) {
            // u is a vertex from partition A
            Q.emplace(std::make_pair(u, true));

            // mark u as visited
            visited.emplace(u);
        }
    }

    while (not Q.empty()) {
        // remove a vertex from the queue
        auto it = Q.front();
        Q.pop();

        // get the preference list and the matched partners
        auto u = it.first;
        auto uit = M.find(u);
        auto& pl = u->get_preference_list();
        auto& partners = uit == M.end() ? PartnerList() : uit->second;

        if (it.second) {
            // u is a vertex belonging to partition A
            // go through the neigbours of this vertex
            for (auto i = pl.begin(), e = pl.end(); i != e; ++i) {
                auto v = i->vertex;

                // only add if this vertex has not been visited
                // and is not matched to u
                if (visited.find(v) == visited.end() and
                    partners.find(v) == partners.cend())
                {
                    // v is a vertex from partition B
                    Q.emplace(std::make_pair(v, false));

                    // we have seen this vertex
                    visited.emplace(v);
                }
            }
        } else {
            // u is a vertex from partition B
            // if u is matched to less than upper quota vertices
            // or u is unmatched |M(u)| = 0
            // this is an augmenting path
            if (partners.size() < u->get_upper_quota()) {
                return true;
            } else {
                // add all the matched neighbours
                for (auto i = partners.cbegin(), e = partners.cend(); i != e; ++i) {
                    auto v = (*i).vertex;

                    // only add if this vertex has not been visited
                    if (visited.find(v) == visited.end()) {
                        // v is a vertex from partition A
                        Q.emplace(std::make_pair(v, true));

                        // we have seen this vertex
                        visited.emplace(v);
                    }
                }
            }
        }
    }

    return false;
}
*/

std::ostream& operator<<(std::ostream& out, std::shared_ptr<BipartiteGraph> G) {
    auto print_vertex = [](const VertexPtr& v, std::stringstream& stmp) {
        int lq = v->get_lower_quota();
        int uq = v->get_upper_quota();

        stmp << v->get_id();
        if (lq > 0 and uq > 0) {
            stmp << ' ' << '(' << lq << ',' << uq << ')';
        } else if (uq > 1) {
            stmp << ' ' << '(' << uq << ')';
        }
    };

    std::stringstream stmp;

    // print the partitions first
    stmp << "@PartitionA\n";
    for (const auto& it : G->get_A_partition()) {
        print_vertex(it.second, stmp);
        stmp << ", ";
    }
    stmp.seekp(-2, stmp.cur);
    stmp << ";\n@End\n";

    stmp << "\n@PartitionB\n";
    for (const auto& it : G->get_B_partition()) {
        print_vertex(it.second, stmp);
        stmp << ", ";
    }
    stmp.seekp(-2, stmp.cur);
    stmp << ";\n@End\n";

    // and then the preference lists
    stmp << "\n@PreferenceListsA\n";
    for (const auto& it : G->get_A_partition()) {
        stmp << it.first << ": " << it.second->get_preference_list() << '\n';
    }
    stmp << "@End\n";

    stmp << "\n@PreferenceListsB\n";
    for (const auto& it : G->get_B_partition()) {
        stmp << it.first << ": " << it.second->get_preference_list() << '\n';
    }
    stmp << "@End";

    return out << stmp.str();
}
