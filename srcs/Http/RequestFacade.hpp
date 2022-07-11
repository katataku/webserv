#ifndef SRCS_HTTP_REQUESTFACADE_HPP_
#define SRCS_HTTP_REQUESTFACADE_HPP_

#include "Logging.hpp"

class RequestFacade {
 public:
    RequestFacade();
    RequestFacade(RequestFacade const &other);
    RequestFacade &operator=(RequestFacade const &other);
    ~RequestFacade();

 private:
    Logging logging_;
};

#endif  // SRCS_HTTP_REQUESTFACADE_HPP_
