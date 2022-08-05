#ifndef SRCS_HTTP_RESPONSEBUILDER_HPP_
#define SRCS_HTTP_RESPONSEBUILDER_HPP_

#include <string>

#include "HTTPResponse.hpp"
#include "Logging.hpp"
#include "ServerLocation.hpp"

class ResponseBuilder {
 public:
    ResponseBuilder();
    ResponseBuilder(ResponseBuilder const &other);
    ResponseBuilder &operator=(ResponseBuilder const &other);
    ~ResponseBuilder();

    static HTTPResponse *Build(std::string body);
    static HTTPResponse *BuildNoBody(int status_code);
    static HTTPResponse *BuildError(int status_code, ServerLocation *sl);
    static HTTPResponse *BuildRedirect(std::string redirect_url);

 private:
    Logging logging_;
};

#endif  // SRCS_HTTP_RESPONSEBUILDER_HPP_
