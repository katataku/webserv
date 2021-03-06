#include "utils.hpp"

#include <sys/stat.h>

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>

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
    // 区切り文字のあとに来る文字(空文字含む）を追加する
    // 区切り文字が存在しない場合はstrがそのまま入る
    strs.push_back(str.substr(pos));
    return strs;
}

std::string Join(std::vector<std::string> strs, std::string separator) {
    std::stringstream ss;

    std::vector<std::string>::iterator itr;
    for (itr = strs.begin(); itr != strs.end(); ++itr) {
        if (itr != strs.begin()) {
            ss << separator;
        }
        ss << *itr;
    }
    return ss.str();
}

static bool HasPlusSign(const std::string& str) {
    std::string trimmed_spaces = LeftTrim(str, " \f\n\r\t\v");

    return trimmed_spaces[0] == '+';
}

static bool HasPrefix(const std::string& str) {
    std::string trimmed_spaces = LeftTrim(str, " \f\n\r\t\v");
    return StartsWith(trimmed_spaces, "0x") || StartsWith(trimmed_spaces, "0X");
}

// 10進数と16進数での使用を想定
bool IsInteger(std::string str, int base) {
    // +記号がある場合、0xなどのprefixがついている場合は有効な数値でないと扱う
    if (HasPlusSign(str) || HasPrefix(str)) {
        return false;
    }

    char* end = NULL;
    errno = 0;
    long l = std::strtol(str.c_str(), &end, base);  // NOLINT

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

int ToInteger(std::string str, int base) {
    return std::strtol(str.c_str(), NULL, base);
}

std::string LeftTrim(const std::string& s, const std::string& chars) {
    size_t start = s.find_first_not_of(chars);
    return (start == std::string::npos) ? "" : s.substr(start);
}
std::string RightTrim(const std::string& s, const std::string& chars) {
    size_t end = s.find_last_not_of(chars);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
std::string Trim(const std::string& s, const std::string& chars) {
    return RightTrim(LeftTrim(s, chars), chars);
}

std::string ToUpper(std::string s) {
    for (std::string::size_type i = 0; i < s.size(); i++) {
        s[i] = std::toupper(s[i]);
    }
    return s;
}

std::string ToLower(std::string s) {
    for (std::string::size_type i = 0; i < s.size(); i++) {
        s[i] = std::tolower(s[i]);
    }
    return s;
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

bool IsDigit(const char c) { return std::isdigit(c) != 0; }
bool IsAlpha(const char c) { return std::isalpha(c) != 0; }
bool IsSpace(const char c) { return std::isspace(c) != 0; }

std::string ConsumeSpace(const std::string& s) {
    if (s.empty()) {
        return s;
    }
    if (IsSpace(s[0])) {
        return s.substr(1);
    }
    if (s[0] == '{') {
        return s;
    }
    throw std::runtime_error("Error: unexpected \"" + s +
                             "\", expecting space");
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
    return c == '/' || c == '_' || c == '.' || c == '-' || IsAlpha(c) ||
           IsDigit(c);
}
bool IsURIChar(const char c) { return c == ':' || IsPathChar(c); }

bool IsValueChar(const char c) { return IsURIChar(c) || c == '-' || c == '+'; }

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

bool IsExistRegularFile(std::string filepath) {
    struct stat stat_buf;

    if (stat(filepath.c_str(), &stat_buf) == -1) {
        return false;
    }

    if (S_ISREG(stat_buf.st_mode)) {
        return true;
    }
    return false;
}

bool HasPermissionToRead(std::string filepath) {
    std::ifstream ifs(filepath.c_str());

    if (!ifs) {
        return false;
    }
    return true;
}
