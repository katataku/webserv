#include "utils.hpp"

std::vector<std::string> Split(std::string const str, std::string const delim) {
    std::vector<std::string> strs;
    std::string::size_type pos = 0;

    for (;;) {
        std::string::size_type found = str.find(delim, pos);
        if (found != std::string::npos) {
            strs.push_back(str.substr(pos, found - pos));
            pos = found + delim.size();
        } else {
            if (pos < str.size()) {
                strs.push_back(str.substr(pos));
            }
            break;
        }
    }
    return strs;
}
