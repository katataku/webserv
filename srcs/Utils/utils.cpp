#include "utils.hpp"

#include <cctype>
#include <cstdlib>
#include <iostream>

std::vector<std::string> Split(std::string const str, std::string const delim) {
    std::vector<std::string> strs;
    std::string::size_type pos = 0;

    for (;;) {
        std::string::size_type found = str.find(delim, pos);
        if (found != std::string::npos) {
            strs.push_back(str.substr(pos, found - pos));
            pos = found + delim.size();
        } else {
            break;
        }
    }
    // 区切り文字のあとに文字が残っていればstrsにつめる
    if (pos < str.size()) {
        strs.push_back(str.substr(pos));
    }
    return strs;
}

bool IsInteger(std::string str) {
    char* end;
    errno = 0;
    long l = std::strtol(str.c_str(), &end, 10);

    // empty string
    if (str == end) {
        return false;
    }
    // non-numeric char is presents. for example, 1a
    if (*end != '\0') {
        return false;
    }
    // overflow long value
    if (errno == ERANGE) {
        return false;
    }
    // overflow int value
    if (l < std::numeric_limits<int>::min() ||
        l > std::numeric_limits<int>::max()) {
        return false;
    }
    return true;
}

int ToInteger(std::string str) { return std::atoi(str.c_str()); }

bool StartsWith(const std::string& s, const std::string& prefix) {
    return s.find(prefix, 0) == 0;
}

std::string Consume(const std::string& s, const std::string& keyword) {
    if (StartsWith(s, keyword)) {
        return s.substr(keyword.size());
    }
    throw std::runtime_error("Failed to Consume " + s + " " + keyword);
}

bool Expect(std::string* s, const std::string& keyword) {
    if (StartsWith(*s, keyword)) {
        *s = Consume(*s, keyword);
        return true;
    }
    return false;
}

bool IsDigit(const char c) { return std::isdigit(c) != 0; }
bool IsAlpha(const char c) { return std::isalpha(c) != 0; }
bool IsSpace(const char c) { return std::isspace(c) != 0; }

std::string ConsumeSpace(const std::string& s) {
    if (IsSpace(s[0])) {
        return s.substr(1);
    }
    throw std::runtime_error("Failed to Consume. Expected space, but got " + s);
}

std::string ConsumeWithSpace(const std::string& s, const std::string& keyword) {
    return ConsumeSpace(Consume(s, keyword));
}

std::string SkipLine(const std::string& s) {
    std::string::size_type nl_at = s.find("\n");
    if (nl_at == std::string::npos) {
        return "";
    }
    return s.substr(nl_at + 1);
}

// TODO(iyamada) ファイルパスとして扱うべき文字を追加
bool IsPathChar(const char c) {
    return c == '/' || c == '_' || c == '.' || IsAlpha(c);
}
bool IsURIChar(const char c) { return c == ':' || IsPathChar(c); }

bool IsValueChar(const char c) {
    return IsURIChar(c) || IsPathChar(c) || IsDigit(c);
}

std::string GetValueCharacters(const std::string& s) {
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (!IsValueChar(s[i])) {
            return s.substr(0, i);
        }
    }
    throw std::runtime_error("Failed to GetValueCharacters");
}

std::string ConsumeValueCharacters(const std::string& s) {
    return s.substr(GetValueCharacters(s).size());
}

std::string SkipString(const std::string& s, const std::string& kw) {
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (StartsWith(s.substr(i), kw)) {
            return s.substr(i + kw.size());
        }
    }
    return "";
}

std::string SkipSpace(const std::string& s) {
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (!IsSpace(s[i])) {
            return s.substr(i);
        }
    }
    return "";
}
