#include "TestDefs.h"
#include "Utils.h"
#include <algorithm>

std::string get_root_dir() {
    return "/media/rawatamit/f55c6248-0895-4d46-8d0e-1db681847773/sea/GraphMatching";
}

std::string get_resources_dir() {
    return get_root_dir() + std::string("/resources");
}

std::string get_filepath(std::string dir, std::string filename) {
    return dir + filename;
}

bool has_partners(std::shared_ptr<MatchingAlgorithm::MatchedPairListType> M, VertexPtr v, const std::vector<VertexPtr>& partners) {
    auto M_v = get_partners(M, v);

    for (auto& it : M_v) {
        if (std::find(partners.cbegin(), partners.cend(), it.vertex) == partners.cend()) {
            return false;
        }
    }

    return M_v.size() == partners.size();
}
