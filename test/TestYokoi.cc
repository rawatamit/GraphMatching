#include "catch.hpp"
#include "Vertex.h"
#include "YokoiEnvyfreeHRLQ.h"
#include "GraphReader.h"
#include "Utils.h"
#include "TestDefs.h"

TEST_CASE("YokoiEnvyfreeHRLQ yokoi_diff_R_H", "[matching_yokoi]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/yokoi_diff_R_H.txt"));

    SECTION("residents proposing") {
        YokoiEnvyfreeHRLQ ye(G, true);
        auto r1 = get_vertex_by_id(G, "r1");
        auto r2 = get_vertex_by_id(G, "r2");
        auto r3 = get_vertex_by_id(G, "r3");
        auto r4 = get_vertex_by_id(G, "r4");
        auto h1 = get_vertex_by_id(G, "h1");
        auto h2 = get_vertex_by_id(G, "h2");

        auto M = ye.compute_matching();

        REQUIRE(M.size() == 2);

        SECTION("size of partner list") {
            REQUIRE(M.number_of_partners(r1) == 1);
            REQUIRE(M.number_of_partners(r2) == 0);
            REQUIRE(M.number_of_partners(r3) == 0);
            REQUIRE(M.number_of_partners(r4) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(M.get_partner(r1) == h1);
            REQUIRE(M.get_partner(r4) == h2);
            REQUIRE(M.get_partner(h1) == r1);
            REQUIRE(M.get_partner(h2) == r4);
        }
    }
}

TEST_CASE("YokoiEnvyfreeHRLQ yokoi_and_max_envy", "[matching_yokoi]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/yokoi_and_max_envy.txt"));
    auto r1 = get_vertex_by_id(G, "r1");
    auto r2 = get_vertex_by_id(G, "r2");
    auto h1 = get_vertex_by_id(G, "h1");
    auto h2 = get_vertex_by_id(G, "h2");
    auto h3 = get_vertex_by_id(G, "h3");

    SECTION("residents proposing") {
        YokoiEnvyfreeHRLQ ye(G, true);
        auto M = ye.compute_matching();

        REQUIRE(M.size() == 1);

        SECTION("size of partner list") {
            REQUIRE(M.number_of_partners(r1) == 1);
            REQUIRE(M.number_of_partners(r2) == 0);
        }

        SECTION("actual partners") {
            REQUIRE(M.get_partner(r1) == h2);
            REQUIRE(M.get_partner(h2) == r1);
        }
    }
}