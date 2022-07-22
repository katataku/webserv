#ifndef SRCS_UTILS_UTILS_HPP_
#define SRCS_UTILS_UTILS_HPP_

#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> Split(std::string const str, std::string const delim);

template <typename T>
static std::string numtostr(T num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}
#endif  // SRCS_UTILS_UTILS_HPP_
