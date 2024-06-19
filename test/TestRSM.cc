#include <CriticalRSM.h>
#include <GraphReader.h>
#include <MatchingAlgorithm.h>
#include <Partner.h>
#include <Utils.h>
#include <Vertex.h>
#include "TestDefs.h"

TEST_CASE("Verify method of critical RSM", "[matching_critical_rsm]") {
    auto G = read_graph(get_filepath(get_resources_dir(), "/critical_rsm.txt"));
    auto a1 = get_vertex_by_id(G, "a1");
    auto a2 = get_vertex_by_id(G, "a2");
    auto a3 = get_vertex_by_id(G, "a3");
    auto b1 = get_vertex_by_id(G, "b1");
    auto b2 = get_vertex_by_id(G, "b2");
    auto b3 = get_vertex_by_id(G, "b3");
    auto b4 = get_vertex_by_id(G, "b4");
    CriticalRSM crsm(G);
    Matching M = crsm.compute_matching();
    bool verify = crsm.is_rsm(M);
    REQUIRE(verify == true);

    // M = = {(a1, b2), (a2, b1), (a3, b3)} is critical but not rsm
    M.remove_partner(a2, b4);
    Partner p(b1, 1, 0);
    M.add_partner(a2, p, 0);
    Partner p1(a2, 1, 0);
    M.add_partner(b1, p1, 0);
    verify = crsm.is_rsm(M);
    REQUIRE(verify == false);
}