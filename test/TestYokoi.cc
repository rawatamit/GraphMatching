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

        REQUIRE(matching_size(M) == 2);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, r1) == 1);
            REQUIRE(number_of_partners(M, r2) == 0);
            REQUIRE(number_of_partners(M, r3) == 0);
            REQUIRE(number_of_partners(M, r4) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, r1) == h1);
            REQUIRE(get_partner(M, r4) == h2);
            REQUIRE(get_partner(M, h1) == r1);
            REQUIRE(get_partner(M, h2) == r4);
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

        REQUIRE(matching_size(M) == 1);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, r1) == 1);
            REQUIRE(number_of_partners(M, r2) == 0);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, r1) == h2);
            REQUIRE(get_partner(M, h2) == r1);
        }
    }
}