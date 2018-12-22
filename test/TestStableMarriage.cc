#include "catch.hpp"
#include "StableMarriage.h"
#include "GraphReader.h"
#include "Utils.h"

#include <iostream>

static std::string ROOT_DIR = "/media/rawatamit/f55c6248-0895-4d46-8d0e-1db681847773/sea/GraphMatching";
static std::string RESOURCES_DIR = ROOT_DIR + std::string("/resources");

static VertexPtr get_vertex_by_id(const std::unique_ptr<BipartiteGraph>& G, const IdType& id) {
    auto A_it = G->get_A_partition().find(id);

    return A_it == G->get_A_partition().end() ? G->get_B_partition().find(id)->second : A_it->second;
}

TEST_CASE("StableMarriage example paper Kavitha", "[compute_matching]") {
    std::string filepath = RESOURCES_DIR + std::string("/example_paper.txt");
    auto G = GraphReader(filepath.c_str()).read_graph();

    StableMarriage sm(G);
    sm.compute_matching();
    auto M = sm.get_matched_pairs();

    REQUIRE(matching_size(M) == 1);

    auto a1 = get_vertex_by_id(G, "a1");
    auto a2 = get_vertex_by_id(G, "a2");
    auto b1 = get_vertex_by_id(G, "b1");
    auto b2 = get_vertex_by_id(G, "b2");

    SECTION("size of partner list") {
        REQUIRE(M.at(a1).size() == 1);
        REQUIRE(M.at(b1).size() == 1);

        REQUIRE(! has_partner(M, a2));
        REQUIRE(! has_partner(M, b2));
    }

    SECTION("actual partners") {
        REQUIRE(M.at(a1).find(b1) != M.at(a1).end());
        REQUIRE(M.at(b1).find(a1) != M.at(b1).end());
    }
}
