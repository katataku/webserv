#ifndef SRCS_HTTP_REQUESTFACADE_HPP_
#define SRCS_HTTP_REQUESTFACADE_HPP_

#include <map>

#include "HTTPRequest.hpp"
#include "Logging.hpp"
#include "Socket.hpp"

class RequestFacade {
 public:
    RequestFacade();
    RequestFacade(RequestFacade const &other);
    RequestFacade &operator=(RequestFacade const &other);
    ~RequestFacade();

    HTTPRequest *SelectRequest(Socket socket);
    void Finish(Socket socket);

 private:
    Logging logging_;
    std::map<int, HTTPRequest> list_;
};

#endif  // SRCS_HTTP_REQUESTFACADE_HPP_
