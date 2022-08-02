#ifndef SRCS_TRANSACTION_CGIRESPONSE_HPP_
#define SRCS_TRANSACTION_CGIRESPONSE_HPP_

#include <string>

#include "HTTPResponse.hpp"
#include "Logging.hpp"

class CGIResponse {
 public:
    CGIResponse();
    CGIResponse(CGIResponse const &other);
    CGIResponse &operator=(CGIResponse const &other);
    ~CGIResponse();

    HTTPResponse *ToHTTPResponse();

    void set_status_code(const std::string &status_code);

    static CGIResponse Parse(const std::string &resp);

 private:
    std::string status_code_;
    std::string reason_phrase_;
    std::string body_;

    void ParseStatusLine(const std::string &line);
};

#endif  // SRCS_TRANSACTION_CGIRESPONSE_HPP_
