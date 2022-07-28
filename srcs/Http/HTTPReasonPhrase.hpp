#ifndef SRCS_HTTP_HTTPREASONPHRASE_HPP_
#define SRCS_HTTP_HTTPREASONPHRASE_HPP_

#include <string>

#include "Logging.hpp"

class HTTPReasonPhrase {
 public:
    HTTPReasonPhrase();
    HTTPReasonPhrase(HTTPReasonPhrase const &other);
    HTTPReasonPhrase &operator=(HTTPReasonPhrase const &other);
    ~HTTPReasonPhrase();

    static std::string GetReasonPhrase(int);

 private:
};

#endif  // SRCS_HTTP_HTTPREASONPHRASE_HPP_
