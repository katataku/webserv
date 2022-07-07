#ifndef SRCS_WEBSERV_RESPONSE_HPP_
#define SRCS_WEBSERV_RESPONSE_HPP_

#include <iostream>
#include <string>

class Response {
 public:
  Response() {}
  Response(const Response& other) { *this = other; }
  Response& operator=(const Response& other) {
    if (this != &other) {
      this->header_ = other.header_;
      this->body_ = other.body_;
    }
    return *this;
  }
  ~Response() {}

  void SetHeader(const std::string& header) { header_ = header; }
  void SetBody(const std::string& body) { body_ = body; }
  std::string GetHeader() const { return header_; }
  std::string GetBody() const { return body_; }

 private:
  std::string header_;
  std::string body_;
};

inline std::ostream& operator<<(std::ostream& out, const Response& rhs) {
  out << rhs.GetHeader() << "\r\n";  // << rhs.GetBody();
  return out;
}

#endif  // SRCS_WEBSERV_RESPONSE_HPP_
