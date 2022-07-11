#ifndef SRCS_HTTP_REQUESTFACADE_HPP_
#define SRCS_HTTP_REQUESTFACADE_HPP_

#include <map>

#include "Logging.hpp"
#include "Request.hpp"
#include "Socket.hpp"

class RequestFacade {
 public:
    RequestFacade();
    RequestFacade(RequestFacade const &other);
    RequestFacade &operator=(RequestFacade const &other);
    ~RequestFacade();

    Request &SelectRequest(Socket socket);
    void Finish(Socket socket);

 private:
    Logging logging_;
    std::map<int, Request> list_;
};

#endif  // SRCS_HTTP_REQUESTFACADE_HPP_
