#include "catch.hpp"
#include "StableMarriage.h"
#include "Popular.h"
#include "Utils.h"
#include "TestDefs.h"

TEST_CASE("StableMarriage hrlq_m6", "[matching_SM_HR]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/hrlq_m6.txt"));
    auto r1 = get_vertex_by_id(G, "r1");
    auto r2 = get_vertex_by_id(G, "r2");
    auto r3 = get_vertex_by_id(G, "r3");
    auto r4 = get_vertex_by_id(G, "r4");

    auto h1 = get_vertex_by_id(G, "h1");
    auto h2 = get_vertex_by_id(G, "h2");
    auto h3 = get_vertex_by_id(G, "h3");

    SECTION("residents proposing") {
        StableMarriage sm(G);
        auto M = sm.compute_matching();

        REQUIRE(matching_size(M) == 3);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, r1) == 1);
            REQUIRE(number_of_partners(M, r2) == 1);
            REQUIRE(number_of_partners(M, r3) == 1);
            REQUIRE(number_of_partners(M, h1) == 2);
            REQUIRE(number_of_partners(M, h2) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, r1) == h1);
            REQUIRE(get_partner(M, r2) == h1);
            REQUIRE(get_partner(M, r3) == h2);
            REQUIRE(has_partners(M, h1, {r1, r2}));
            REQUIRE(has_partners(M, h2, {r3}));
        }
    }

    SECTION("hospitals proposing") {
        StableMarriage sm(G, false);
        auto M = sm.compute_matching();

        REQUIRE(matching_size(M) == 3);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, r1) == 1);
            REQUIRE(number_of_partners(M, r2) == 1);
            REQUIRE(number_of_partners(M, r3) == 1);
            REQUIRE(number_of_partners(M, h1) == 2);
            REQUIRE(number_of_partners(M, h2) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, r1) == h1);
            REQUIRE(get_partner(M, r2) == h1);
            REQUIRE(get_partner(M, r3) == h2);
            REQUIRE(has_partners(M, h1, {r1, r2}));
            REQUIRE(has_partners(M, h2, {r3}));
        }
    }
}

TEST_CASE("MaxCardPopular hrlq_m6", "[matching_MP_HR]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/hrlq_m6.txt"));
    auto r1 = get_vertex_by_id(G, "r1");
    auto r2 = get_vertex_by_id(G, "r2");
    auto r3 = get_vertex_by_id(G, "r3");
    auto r4 = get_vertex_by_id(G, "r4");

    auto h1 = get_vertex_by_id(G, "h1");
    auto h2 = get_vertex_by_id(G, "h2");
    auto h3 = get_vertex_by_id(G, "h3");

    SECTION("residents proposing") {
        MaxCardPopular mp(G);
        auto M = mp.compute_matching();

        REQUIRE(matching_size(M) == 4);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, r1) == 1);
            REQUIRE(number_of_partners(M, r2) == 1);
            REQUIRE(number_of_partners(M, r3) == 1);
            REQUIRE(number_of_partners(M, h1) == 2);
            REQUIRE(number_of_partners(M, h2) == 1);
            REQUIRE(number_of_partners(M, h3) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, r1) == h2);
            REQUIRE(get_partner(M, r2) == h3);
            REQUIRE(get_partner(M, r3) == h1);
            REQUIRE(get_partner(M, r4) == h1);
            REQUIRE(has_partners(M, h1, {r3, r4}));
            REQUIRE(has_partners(M, h2, {r1}));
            REQUIRE(has_partners(M, h3, {r2}));
        }
    }

    SECTION("hospitals proposing") {
        MaxCardPopular mp(G, false);
        auto M = mp.compute_matching();

        REQUIRE(matching_size(M) == 4);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, r1) == 1);
            REQUIRE(number_of_partners(M, r2) == 1);
            REQUIRE(number_of_partners(M, r3) == 1);
            REQUIRE(number_of_partners(M, r4) == 1);
            REQUIRE(number_of_partners(M, h1) == 2);
            REQUIRE(number_of_partners(M, h2) == 1);
            REQUIRE(number_of_partners(M, h3) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, r1) == h3);
            REQUIRE(get_partner(M, r2) == h1);
            REQUIRE(get_partner(M, r3) == h1);
            REQUIRE(get_partner(M, r4) == h2);
            REQUIRE(has_partners(M, h1, {r2, r3}));
            REQUIRE(has_partners(M, h2, {r4}));
            REQUIRE(has_partners(M, h3, {r1}));
        }
    }
}

TEST_CASE("MaxCardPopular P_mat_level_up", "[matching_MP_HR_level_up]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/P_mat_level_up.txt"));
    auto r1 = get_vertex_by_id(G, "r1");
    auto r4 = get_vertex_by_id(G, "r4");
    auto r5 = get_vertex_by_id(G, "r5");
    auto r7 = get_vertex_by_id(G, "r7");
    auto r8 = get_vertex_by_id(G, "r8");
    auto r10 = get_vertex_by_id(G, "r10");

    auto h1 = get_vertex_by_id(G, "h1");
    auto h2 = get_vertex_by_id(G, "h2");
    auto h3 = get_vertex_by_id(G, "h3");

    SECTION("residents proposing") {
        MaxCardPopular mp(G);
        auto M = mp.compute_matching();

        REQUIRE(matching_size(M) == 6);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, r1) == 1);
            REQUIRE(number_of_partners(M, r4) == 1);
            REQUIRE(number_of_partners(M, r5) == 1);
            REQUIRE(number_of_partners(M, r7) == 1);
            REQUIRE(number_of_partners(M, r8) == 1);
            REQUIRE(number_of_partners(M, r10) == 1);
            REQUIRE(number_of_partners(M, h1) == 2);
            REQUIRE(number_of_partners(M, h2) == 2);
            REQUIRE(number_of_partners(M, h3) == 2);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, r1) == h3);
            REQUIRE(get_partner(M, r4) == h2);
            REQUIRE(get_partner(M, r5) == h1);
            REQUIRE(get_partner(M, r7) == h3);
            REQUIRE(get_partner(M, r8) == h1);
            REQUIRE(get_partner(M, r10) == h2);
            REQUIRE(has_partners(M, h1, {r5, r8}));
            REQUIRE(has_partners(M, h2, {r4, r10}));
            REQUIRE(has_partners(M, h3, {r1, r7}));
        }
    }

    SECTION("hospitals proposing") {
        MaxCardPopular mp(G, false);
        auto M = mp.compute_matching();

        REQUIRE(matching_size(M) == 6);

        SECTION("size of partner list") {
            REQUIRE(number_of_partners(M, r1) == 1);
            REQUIRE(number_of_partners(M, r4) == 1);
            REQUIRE(number_of_partners(M, r5) == 1);
            REQUIRE(number_of_partners(M, r7) == 1);
            REQUIRE(number_of_partners(M, r8) == 1);
            REQUIRE(number_of_partners(M, r10) == 1);
            REQUIRE(number_of_partners(M, h1) == 2);
            REQUIRE(number_of_partners(M, h2) == 2);
            REQUIRE(number_of_partners(M, h3) == 2);
        }

        SECTION("actual partners") {
            REQUIRE(get_partner(M, r1) == h3);
            REQUIRE(get_partner(M, r4) == h2);
            REQUIRE(get_partner(M, r5) == h1);
            REQUIRE(get_partner(M, r7) == h3);
            REQUIRE(get_partner(M, r8) == h1);
            REQUIRE(get_partner(M, r10) == h2);
            REQUIRE(has_partners(M, h1, {r5, r8}));
            REQUIRE(has_partners(M, h2, {r4, r10}));
            REQUIRE(has_partners(M, h3, {r1, r7}));
        }
    }
}
