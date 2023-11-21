#include <Vertex.h>
#include <GraphReader.h>
#include <CriticalRSM.h>
#include <Utils.h>
#include "TestDefs.h"

TEST_CASE("Prefers method of PreferenceList", "[pref_list]") {
    std::shared_ptr<Vertex> b1 = std::make_shared<Vertex>("b1");
    std::shared_ptr<Vertex> b2 = std::make_shared<Vertex>("b2");
    std::shared_ptr<Vertex> b3 = std::make_shared<Vertex>("b3");

    PreferenceList a1_pref_list;
    a1_pref_list.emplace_back(b1);
    a1_pref_list.set_tie(1, b2);
    a1_pref_list.set_tie(1, b3);

    auto preference1 = a1_pref_list.prefers(b1, b3);
    REQUIRE(preference1 == cBetter);

    auto preference3 = a1_pref_list.prefers(b2, b3);
    REQUIRE(preference3 == cEqual);

    auto preference2 = a1_pref_list.prefers(b3, b1);
    REQUIRE(preference2 == cWorse);

}