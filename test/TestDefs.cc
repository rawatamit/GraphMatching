#include "TestDefs.h"
#include "Utils.h"
#include <algorithm>

// FIXME: platform independent way of getting root directory
std::string get_root_dir() {
    return "../../";
}

std::string get_resources_dir() {
    return get_root_dir() + std::string("resources/");
}

std::string get_filepath(std::string dir, std::string filename) {
    return dir + filename;
}

bool has_partners(std::shared_ptr<MatchingAlgorithm::MatchedPairListType> M, VertexPtr v, const std::vector<VertexPtr>& partners) {
    auto M_v = get_partners(std::move(M), std::move(v));

    for (auto& it : M_v) {
        if (std::find(partners.cbegin(), partners.cend(), it.vertex) == partners.cend()) {
            return false;
        }
    }

    return M_v.size() == partners.size();
}
