#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>

class Response {
 public:
  Response() {}
  ~Response() {}
  Response(const Response& other) { *this = other; }
  Response& operator=(const Response& other) {
    if (this == &other) return *this;

    this->header_ = other.header_;
    this->body_ = other.body_;
    return *this;
  }

  void SetHeader(const std::string& header) { header_ = header; }
  void SetBody(const std::string& body) { body_ = body; }
  std::string GetHeader() const { return header_; }
  std::string GetBody() const { return body_; }

 private:
  std::string header_;
  std::string body_;
};

std::ostream& operator<<(std::ostream& out, const Response& rhs) {
  out << rhs.GetHeader() << "\r\n" << rhs.GetBody();
  return out;
}

#endif  // RESPONSE_HPP
