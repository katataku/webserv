#ifndef SRCS_WEBSERV_UTILS_HPP_
#define SRCS_WEBSERV_UTILS_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string>

inline std::string Consume(const std::string& s, const std::string& sep) {
  std::size_t sep_at = s.find(sep);
  if (sep_at == std::string::npos) return "";

  return s.substr(sep_at + sep.length());
}

inline std::string GetToken(const std::string& s, const std::string& sep) {
  std::size_t sep_at = s.find(sep);
  if (sep_at == std::string::npos) return "";

  return s.substr(0, sep_at);
}

inline std::string GetFileExt(const std::string& path) {
  std::size_t dot_at = path.rfind(".");
  if (dot_at == std::string::npos) {
    return std::string("");
  }
  return path.substr(dot_at+1);
}

inline void error(const char *msg) {
  perror(msg);
  exit(0);
}

#endif  // SRCS_WEBSERV_UTILS_HPP_
