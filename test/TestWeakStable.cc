#include <CriticalRSM.h>
#include <GraphReader.h>
#include <Utils.h>
#include "TestDefs.h"

TEST_CASE("Critical RSM with no ties/critical vertices", "") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/weak_stable.txt"));
    auto a1 = get_vertex_by_id(G, "a1");
    auto a2 = get_vertex_by_id(G, "a2");
    auto a3 = get_vertex_by_id(G, "a3");
    auto b1 = get_vertex_by_id(G, "b1");
    auto b2 = get_vertex_by_id(G, "b2");
    auto b3 = get_vertex_by_id(G, "b3");

    SECTION("residents proposing") {
        CriticalRSM crsm(G);
        auto M = crsm.compute_matching();

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
