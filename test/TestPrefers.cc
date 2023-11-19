#include <Vertex.h>
#include <GraphReader.h>
#include <CriticalRSM.h>
#include <Utils.h>
#include "TestDefs.h"

TEST_CASE("Helper method of CriticalRSM", "[matching_critical_rsm]") {
    std::shared_ptr<Vertex> a1 = std::make_shared<Vertex>("a1");
    std::shared_ptr<Vertex> b1 = std::make_shared<Vertex>("b1");
    std::shared_ptr<Vertex> b2 = std::make_shared<Vertex>("b2");
    std::shared_ptr<Vertex> b3 = std::make_shared<Vertex>("b3");

    PreferenceList a1_pref_list;
    a1->get_preference_list().emplace_back(b1);
    a1->get_preference_list().set_tie(1, b2);
    a1->get_preference_list().set_tie(1, b3);

    auto M = Matching();
    auto G = read_graph(get_filepath(get_resources_dir(), "/weak_stable.txt")); // required only for critical RSM constructor, doesn't affect output of the function
    CriticalRSM rsm(G);

    auto preference1 = rsm.prefers(a1, b1, b3);
    REQUIRE(preference1 == 0);

    auto preference3 = rsm.prefers(a1, b2, b3);
    REQUIRE(preference3 == 1);

    auto preference2 = rsm.prefers(a1, b3, b1);
    REQUIRE(preference2 == 2);

}