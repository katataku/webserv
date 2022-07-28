#include "utils.hpp"

#include <sys/stat.h>

#include <cctype>
#include <fstream>
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

static bool IsDigit(const char c) { return std::isdigit(c) != 0; }
static bool IsAlpha(const char c) { return std::isalpha(c) != 0; }
static bool IsSpace(const char c) { return std::isspace(c) != 0; }

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

std::string ReadFile(std::string file_path) {
    std::ifstream ifs(file_path.c_str());
    std::ostringstream oss;

    if (!ifs) {
        // TODO(takkatao):
        // オープンできないときはここに入る。
        throw std::runtime_error("ReadFile ifs open fail");
    }

    oss << ifs.rdbuf();

    return oss.str();
}

bool isExistRegularFile(std::string filepath) {
    struct stat stat_buf;

    if (stat(filepath.c_str(), &stat_buf) == -1) {
        return false;
    }

    if (S_ISREG(stat_buf.st_mode)) {
        return true;
    }
    return false;
}

bool hasPermissionToRead(std::string filepath) {
    std::ifstream ifs(filepath.c_str());

    if (!ifs) {
        return false;
    }
    return true;
}
