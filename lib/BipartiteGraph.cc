#include "BipartiteGraph.h"
#include "Vertex.h"
#include <set>
#include <queue>
#include <sstream>

BipartiteGraph::BipartiteGraph(const ContainerType& A, const ContainerType& B)
    : A_(A), B_(B)
{}

BipartiteGraph::~BipartiteGraph()
{}

const BipartiteGraph::ContainerType& BipartiteGraph::get_A_partition() const {
    return A_;
}

const BipartiteGraph::ContainerType& BipartiteGraph::get_B_partition() const {
    return B_;
}

bool BipartiteGraph::has_augmenting_path() const {
    // the second entry of the pair is true if
    // the vertex belongs to partition A, otherwise false
    std::queue<std::pair<VertexPtr, bool>> Q;
    std::set<VertexPtr> visited;

    // populate the queue with vertices from A
    // which can participate in an augmenting path
    // these are precisely the vertices that are unmatched
    for (auto it : get_A_partition()) {
        auto u = it.second;

        if (u->get_partners().empty()) {
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
        auto& pl = u->get_preference_list();
        auto& partners = u->get_partners();

        // if this is a vertex from partition A
        // then the augmenting path cannot end here, otherwise yes
        bool aug = it.second ? false : true;

        // go through the neigbours of this vertex
        for (auto i = pl.all_begin(), e = pl.all_end(); i != e; ++i) {
            auto v = pl.get_vertex(*i);

            // if this vertex has already been visited
            // do not add to the queue
            if (visited.find(v) != visited.end()) {
            } else if (it.second) {
                // u is a vertex belonging to partition A
                // insert outgoing edges which are not matched
                if (partners.find(v) == partners.end()) {
                    // v is a vertex from partition B
                    Q.emplace(std::make_pair(v, false));

                    // we have seen this vertex
                    visited.emplace(v);
                }
            } else {
                // u is a vertex from partition B
                // insert outgoing edges which are matched
                if (partners.find(v) != partners.end()) {
                    // v is a vertex from partition A
                    Q.emplace(std::make_pair(v, true));

                    // we have seen this vertex
                    visited.emplace(v);

                    // we added at least one vertex
                    // so the augmenting path cannot end here
                    aug = false;
                }
            }
        }

        // did we find an augmenting path
        if (aug) {
            return true;
        }
    }

    return false;
}

std::ostream& operator<<(std::ostream& out, const std::unique_ptr<BipartiteGraph>& G) {
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
