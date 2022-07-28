#ifndef SRCS_UTILS_UTILS_HPP_
#define SRCS_UTILS_UTILS_HPP_

#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> Split(std::string const str, std::string const delim);

bool StartsWith(const std::string& s, const std::string& prefix);

std::string Consume(const std::string& s, const std::string& keyword);

bool Expect(std::string* s, const std::string& keyword);

std::string ConsumeSpace(const std::string& s);

std::string ConsumeWithSpace(const std::string& s, const std::string& keyword);

std::string SkipLine(const std::string& s);

bool IsPathChar(const char c);

bool IsValueChar(const char c);

std::string GetValueCharacters(const std::string& s);

std::string ConsumeValueCharacters(const std::string& s);

std::string SkipString(const std::string& s, const std::string& kw);

std::string SkipSpace(const std::string& s);

template <typename T>
T strtonum(const std::string& s) {
    std::stringstream ss(s);
    T num;
    ss >> num;
    return num;
}

template <typename T>
static std::string numtostr(T num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

std::string ReadFile(std::string file_path);

#endif  // SRCS_UTILS_UTILS_HPP_
