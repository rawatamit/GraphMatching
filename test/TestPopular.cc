#include "catch.hpp"
#include "Popular.h"
#include "GraphReader.h"
#include "Utils.h"
#include "TestDefs.h"

TEST_CASE("MaxCardPopular example_paper", "[matching_max_card_popular]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/example_paper.txt"));
    auto a1 = get_vertex_by_id(G, "a1");
    auto a2 = get_vertex_by_id(G, "a2");
    auto b1 = get_vertex_by_id(G, "b1");
    auto b2 = get_vertex_by_id(G, "b2");

    SECTION("residents proposing") {
        MaxCardPopular mp(G);
        auto M = mp.compute_matching();

        REQUIRE(matching_size(M) == 2);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, a1) == 1);
            REQUIRE(number_of_partners(M, a2) == 1);
            REQUIRE(number_of_partners(M, b1) == 1);
            REQUIRE(number_of_partners(M, b2) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, a1) == b2);
            REQUIRE(get_partner(M, a2) == b1);
            REQUIRE(get_partner(M, b1) == a2);
            REQUIRE(get_partner(M, b2) == a1);
        }
    }

    SECTION("hospitals proposing") {
        MaxCardPopular mp(G, false);
        auto M = mp.compute_matching();

        REQUIRE(matching_size(M) == 2);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, a1) == 1);
            REQUIRE(number_of_partners(M, a2) == 1);
            REQUIRE(number_of_partners(M, b1) == 1);
            REQUIRE(number_of_partners(M, b2) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, a1) == b2);
            REQUIRE(get_partner(M, a2) == b1);
            REQUIRE(get_partner(M, b1) == a2);
            REQUIRE(get_partner(M, b2) == a1);
        }
    }
}

TEST_CASE("MaxCardPopular 2pop_matchings", "[matching_max_card_popular]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/2pop_matchings.txt"));
    auto a1 = get_vertex_by_id(G, "a1");
    auto a2 = get_vertex_by_id(G, "a2");
    auto a3 = get_vertex_by_id(G, "a3");
    auto b1 = get_vertex_by_id(G, "b1");
    auto b2 = get_vertex_by_id(G, "b2");
    auto b3 = get_vertex_by_id(G, "b3");

    SECTION("residents proposing") {
        MaxCardPopular mp(G);
        auto M = mp.compute_matching();

        REQUIRE(matching_size(M) == 3);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, a1) == 1);
            REQUIRE(number_of_partners(M, a2) == 1);
            REQUIRE(number_of_partners(M, a3) == 1);
            REQUIRE(number_of_partners(M, b1) == 1);
            REQUIRE(number_of_partners(M, b2) == 1);
            REQUIRE(number_of_partners(M, b3) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, a1) == b2);
            REQUIRE(get_partner(M, a2) == b1);
            REQUIRE(get_partner(M, a3) == b3);
            REQUIRE(get_partner(M, b1) == a2);
            REQUIRE(get_partner(M, b2) == a1);
            REQUIRE(get_partner(M, b3) == a3);
        }
    }

    SECTION("hospitals proposing") {
        MaxCardPopular mp(G, false);
        auto M = mp.compute_matching();

        REQUIRE(matching_size(M) == 3);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, a1) == 1);
            REQUIRE(number_of_partners(M, a2) == 1);
            REQUIRE(number_of_partners(M, a3) == 1);
            REQUIRE(number_of_partners(M, b1) == 1);
            REQUIRE(number_of_partners(M, b2) == 1);
            REQUIRE(number_of_partners(M, b3) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, a1) == b2);
            REQUIRE(get_partner(M, a2) == b3);
            REQUIRE(get_partner(M, a3) == b1);
            REQUIRE(get_partner(M, b1) == a3);
            REQUIRE(get_partner(M, b2) == a1);
            REQUIRE(get_partner(M, b3) == a2);
        }
    }
}

TEST_CASE("MaxCardPopular diff_stable_diff_pop1 (residents proposing)", "[matching_resident]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/diff_stable_diff_pop1.txt"));
    MaxCardPopular sm(G);
    auto M = sm.compute_matching();

    REQUIRE(matching_size(M) == 7);

    auto a1 = get_vertex_by_id(G, "a1");
    auto a2 = get_vertex_by_id(G, "a2");
    auto a3 = get_vertex_by_id(G, "a3");
    auto a4 = get_vertex_by_id(G, "a4");
    auto a5 = get_vertex_by_id(G, "a5");
    auto a6 = get_vertex_by_id(G, "a6");
    auto a7 = get_vertex_by_id(G, "a7");

    auto b1 = get_vertex_by_id(G, "b1");
    auto b2 = get_vertex_by_id(G, "b2");
    auto b3 = get_vertex_by_id(G, "b3");
    auto b4 = get_vertex_by_id(G, "b4");
    auto b5 = get_vertex_by_id(G, "b5");
    auto b6 = get_vertex_by_id(G, "b6");
    auto b7 = get_vertex_by_id(G, "b7");

    SECTION("size of partner list") {
        REQUIRE(number_of_partners(M, a1) == 1);
        REQUIRE(number_of_partners(M, a2) == 1);
        REQUIRE(number_of_partners(M, a3) == 1);
        REQUIRE(number_of_partners(M, a4) == 1);
        REQUIRE(number_of_partners(M, a5) == 1);
        REQUIRE(number_of_partners(M, a6) == 1);
        REQUIRE(number_of_partners(M, a7) == 1);

        REQUIRE(number_of_partners(M, b1) == 1);
        REQUIRE(number_of_partners(M, b2) == 1);
        REQUIRE(number_of_partners(M, b3) == 1);
        REQUIRE(number_of_partners(M, b4) == 1);
        REQUIRE(number_of_partners(M, b5) == 1);
        REQUIRE(number_of_partners(M, b6) == 1);
        REQUIRE(number_of_partners(M, b7) == 1);
    }

    SECTION("actual partners") {
        REQUIRE(get_partner(M, a1) == b2);
        REQUIRE(get_partner(M, a2) == b6);
        REQUIRE(get_partner(M, a3) == b5);
        REQUIRE(get_partner(M, a4) == b1);
        REQUIRE(get_partner(M, a5) == b3);
        REQUIRE(get_partner(M, a6) == b7);
        REQUIRE(get_partner(M, a7) == b4);

        REQUIRE(get_partner(M, b1) == a4);
        REQUIRE(get_partner(M, b2) == a1);
        REQUIRE(get_partner(M, b3) == a5);
        REQUIRE(get_partner(M, b4) == a7);
        REQUIRE(get_partner(M, b5) == a3);
        REQUIRE(get_partner(M, b6) == a2);
        REQUIRE(get_partner(M, b7) == a6);
    }
}