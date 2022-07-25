#ifndef SRCS_TRANSACTION_CGIREQUEST_HPP_
#define SRCS_TRANSACTION_CGIREQUEST_HPP_

#include <map>
#include <string>
#include <vector>

#include "HTTPRequest.hpp"
#include "Logging.hpp"
#include "ServerLocation.hpp"

class CGIRequest {
 public:
    CGIRequest();
    CGIRequest(CGIRequest const &other);
    CGIRequest(HTTPRequest const &http, ServerLocation const &sl);
    CGIRequest &operator=(CGIRequest const &other);
    ~CGIRequest();

    std::string path() const;
    std::vector<std::string> arg() const;
    std::map<std::string, std::string> env() const;
    std::string body() const;

    bool ShouldSendRequestBody() const;

 private:
    Logging logging_;
    std::string path_;
    std::vector<std::string> arg_;
    std::map<std::string, std::string> env_;
    std::string body_;

    void PreparePath(HTTPRequest const &http, ServerLocation const &sl);
    void PrepareArgs();
    void PrepareEnvs(HTTPRequest const &http);
};

#endif  // SRCS_TRANSACTION_CGIREQUEST_HPP_
