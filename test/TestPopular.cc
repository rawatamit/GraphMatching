#include <Popular.h>
#include <GraphReader.h>
#include <Utils.h>
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

        REQUIRE(M.verify(G));
        REQUIRE(M.size() == 2);

        SECTION("size of partner list") {
            REQUIRE(M.number_of_partners(a1) == 1);
            REQUIRE(M.number_of_partners(a2) == 1);
            REQUIRE(M.number_of_partners(b1) == 1);
            REQUIRE(M.number_of_partners(b2) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(M.get_partner(a1) == b2);
            REQUIRE(M.get_partner(a2) == b1);
            REQUIRE(M.get_partner(b1) == a2);
            REQUIRE(M.get_partner(b2) == a1);
        }
    }

    SECTION("hospitals proposing") {
        MaxCardPopular mp(G, false);
        auto M = mp.compute_matching();

        REQUIRE(M.verify(G));
        REQUIRE(M.size() == 2);

        SECTION("size of partner list") {
            REQUIRE(M.number_of_partners(a1) == 1);
            REQUIRE(M.number_of_partners(a2) == 1);
            REQUIRE(M.number_of_partners(b1) == 1);
            REQUIRE(M.number_of_partners(b2) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(M.get_partner(a1) == b2);
            REQUIRE(M.get_partner(a2) == b1);
            REQUIRE(M.get_partner(b1) == a2);
            REQUIRE(M.get_partner(b2) == a1);
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

        REQUIRE(M.verify(G));
        REQUIRE(M.size() == 3);

        SECTION("size of partner list") {
            REQUIRE(M.number_of_partners(a1) == 1);
            REQUIRE(M.number_of_partners(a2) == 1);
            REQUIRE(M.number_of_partners(a3) == 1);
            REQUIRE(M.number_of_partners(b1) == 1);
            REQUIRE(M.number_of_partners(b2) == 1);
            REQUIRE(M.number_of_partners(b3) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(M.get_partner(a1) == b2);
            REQUIRE(M.get_partner(a2) == b1);
            REQUIRE(M.get_partner(a3) == b3);
            REQUIRE(M.get_partner(b1) == a2);
            REQUIRE(M.get_partner(b2) == a1);
            REQUIRE(M.get_partner(b3) == a3);
        }
    }

    SECTION("hospitals proposing") {
        MaxCardPopular mp(G, false);
        auto M = mp.compute_matching();

        REQUIRE(M.verify(G));
        REQUIRE(M.size() == 3);

        SECTION("size of partner list") {
            REQUIRE(M.number_of_partners(a1) == 1);
            REQUIRE(M.number_of_partners(a2) == 1);
            REQUIRE(M.number_of_partners(a3) == 1);
            REQUIRE(M.number_of_partners(b1) == 1);
            REQUIRE(M.number_of_partners(b2) == 1);
            REQUIRE(M.number_of_partners(b3) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(M.get_partner(a1) == b2);
            REQUIRE(M.get_partner(a2) == b3);
            REQUIRE(M.get_partner(a3) == b1);
            REQUIRE(M.get_partner(b1) == a3);
            REQUIRE(M.get_partner(b2) == a1);
            REQUIRE(M.get_partner(b3) == a2);
        }
    }
}

TEST_CASE("MaxCardPopular diff_stable_diff_pop1 (residents proposing)", "[matching_resident]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/diff_stable_diff_pop1.txt"));
    MaxCardPopular sm(G);
    auto M = sm.compute_matching();

    REQUIRE(M.verify(G));
    REQUIRE(M.size() == 7);

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
        REQUIRE(M.number_of_partners(a1) == 1);
        REQUIRE(M.number_of_partners(a2) == 1);
        REQUIRE(M.number_of_partners(a3) == 1);
        REQUIRE(M.number_of_partners(a4) == 1);
        REQUIRE(M.number_of_partners(a5) == 1);
        REQUIRE(M.number_of_partners(a6) == 1);
        REQUIRE(M.number_of_partners(a7) == 1);

        REQUIRE(M.number_of_partners(b1) == 1);
        REQUIRE(M.number_of_partners(b2) == 1);
        REQUIRE(M.number_of_partners(b3) == 1);
        REQUIRE(M.number_of_partners(b4) == 1);
        REQUIRE(M.number_of_partners(b5) == 1);
        REQUIRE(M.number_of_partners(b6) == 1);
        REQUIRE(M.number_of_partners(b7) == 1);
    }

    SECTION("actual partners") {
        REQUIRE(M.get_partner(a1) == b2);
        REQUIRE(M.get_partner(a2) == b6);
        REQUIRE(M.get_partner(a3) == b5);
        REQUIRE(M.get_partner(a4) == b1);
        REQUIRE(M.get_partner(a5) == b3);
        REQUIRE(M.get_partner(a6) == b7);
        REQUIRE(M.get_partner(a7) == b4);

        REQUIRE(M.get_partner(b1) == a4);
        REQUIRE(M.get_partner(b2) == a1);
        REQUIRE(M.get_partner(b3) == a5);
        REQUIRE(M.get_partner(b4) == a7);
        REQUIRE(M.get_partner(b5) == a3);
        REQUIRE(M.get_partner(b6) == a2);
        REQUIRE(M.get_partner(b7) == a6);
    }
}

TEST_CASE("MaxCardPopular brandl_kavitha_2019_fig1 (residents proposing)", "[matching_resident]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/brandl_kavitha_2019_fig1.txt"));
    MaxCardPopular sm(G);
    auto M = sm.compute_matching();

    REQUIRE(M.verify(G));
    REQUIRE(M.size() == 2);

    auto u1 = get_vertex_by_id(G, "u1");
    auto u2 = get_vertex_by_id(G, "u2");

    auto u3 = get_vertex_by_id(G, "u3");
    auto u4 = get_vertex_by_id(G, "u4");

    SECTION("size of partner list") {
        REQUIRE(M.number_of_partners(u1) == 1);
        REQUIRE(M.number_of_partners(u2) == 1);

        REQUIRE(M.number_of_partners(u3) == 1);
        REQUIRE(M.number_of_partners(u4) == 1);
    }

    SECTION("actual partners") {
        REQUIRE(M.get_partner(u1) == u4);
        REQUIRE(M.get_partner(u2) == u3);

        REQUIRE(M.get_partner(u3) == u2);
        REQUIRE(M.get_partner(u4) == u1);
    }
}

TEST_CASE("MaxCardPopular many_many_promotion (residents proposing)", "[matching_resident]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/many_many_promotion.txt"));
    MaxCardPopular sm(G);
    auto M = sm.compute_matching();

    REQUIRE(M.verify(G));
    REQUIRE(M.size() == 3);

    auto a1 = get_vertex_by_id(G, "a1");
    auto a2 = get_vertex_by_id(G, "a2");
    auto a3 = get_vertex_by_id(G, "a3");

    auto b1 = get_vertex_by_id(G, "b1");
    auto b2 = get_vertex_by_id(G, "b2");
    auto b3 = get_vertex_by_id(G, "b3");

    SECTION("size of partner list") {
        REQUIRE(M.number_of_partners(a1) == 1);
        REQUIRE(M.number_of_partners(a2) == 1);
        REQUIRE(M.number_of_partners(a3) == 1);

        REQUIRE(M.number_of_partners(b1) == 1);
        REQUIRE(M.number_of_partners(b2) == 1);
        REQUIRE(M.number_of_partners(b3) == 1);
    }

    SECTION("actual partners") {
        REQUIRE(M.get_partner(a1) == b1);
        REQUIRE(M.get_partner(a2) == b2);
        REQUIRE(M.get_partner(a3) == b3);

        REQUIRE(M.get_partner(b1) == a1);
        REQUIRE(M.get_partner(b2) == a2);
        REQUIRE(M.get_partner(b3) == a3);
    }
}

