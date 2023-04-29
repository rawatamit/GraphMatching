#include "TestDefs.h"
#include "Utils.h"

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

