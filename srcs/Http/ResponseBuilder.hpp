#ifndef SRCS_HTTP_RESPONSEBUILDER_HPP_
#define SRCS_HTTP_RESPONSEBUILDER_HPP_

#include <string>

#include "Logging.hpp"
#include "Response.hpp"
#include "ServerLocation.hpp"

class ResponseBuilder {
 public:
    ResponseBuilder();
    ResponseBuilder(ResponseBuilder const &other);
    ResponseBuilder &operator=(ResponseBuilder const &other);
    ~ResponseBuilder();

    Response *Build(std::string body);
    Response *BuildError(int status_code, ServerLocation *sl);
    Response *BuildRedirect(std::string redirect_url);

 private:
    Logging logging_;
};

#endif  // SRCS_HTTP_RESPONSEBUILDER_HPP_
