#ifndef SRCS_TRANSACTION_CGIRESPONSE_HPP_
#define SRCS_TRANSACTION_CGIRESPONSE_HPP_

#include <string>

#include "HTTPResponse.hpp"
#include "Logging.hpp"

class CGIResponse {
 public:
    CGIResponse();
    CGIResponse(CGIResponse const &other);
    explicit CGIResponse(std::string const &resp);
    CGIResponse &operator=(CGIResponse const &other);
    ~CGIResponse();

    HTTPResponse *ToHTTPResponse();

    void set_status_code(const std::string &status_code);

 private:
    std::string status_;
    std::string status_code_;
    std::string reason_phrase_;
    std::string body_;
};

#endif  // SRCS_TRANSACTION_CGIRESPONSE_HPP_
