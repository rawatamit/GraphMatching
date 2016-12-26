#include "BipartiteGraph.h"
#include "Vertex.h"
#include <sstream>

BipartiteGraph::BipartiteGraph(const ContainerType& A, const ContainerType& B)
    : A_(A), B_(B)
{}

BipartiteGraph::~BipartiteGraph()
{}

const BipartiteGraph::ContainerType& BipartiteGraph::get_A_partition() {
    return A_;
}

const BipartiteGraph::ContainerType& BipartiteGraph::get_B_partition() {
    return B_;
}

std::ostream& operator<<(std::ostream& out, const std::unique_ptr<BipartiteGraph>& G) {
    auto print_vertex = [](const std::shared_ptr<Vertex>& v, std::stringstream& stmp) {
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
