#include <Vertex.h>
#include <MaximalEnvyfreeHRLQ.h>
#include <GraphReader.h>
#include <Utils.h>
#include "TestDefs.h"

TEST_CASE("MaximalEnvyfreeHRLQ yokoi_and_max_envy", "[matching_yokoi]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/yokoi_and_max_envy.txt"));
    auto r1 = get_vertex_by_id(G, "r1");
    auto r2 = get_vertex_by_id(G, "r2");
    auto h1 = get_vertex_by_id(G, "h1");
    auto h2 = get_vertex_by_id(G, "h2");
    auto h3 = get_vertex_by_id(G, "h3");

    SECTION("residents proposing") {
        MaximalEnvyfreeHRLQ me(G, true);
        auto M = me.compute_matching();

        REQUIRE(M.size() == 2);

        SECTION("size of partner list") {
            REQUIRE(M.number_of_partners(r1) == 1);
            REQUIRE(M.number_of_partners(r2) == 1);
            REQUIRE(M.number_of_partners(h2) == 1);
            REQUIRE(M.number_of_partners(h3) == 1);
        }

        SECTION("actual partners") {
            REQUIRE(M.get_partner(r1) == h2);
            REQUIRE(M.get_partner(r2) == h3);
            REQUIRE(M.get_partner(h2) == r1);
            REQUIRE(M.get_partner(h3) == r2);
        }
    }
}
