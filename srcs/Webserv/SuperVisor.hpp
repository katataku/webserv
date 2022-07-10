#ifndef SRCS_WEBSERV_SUPERVISOR_HPP_
#define SRCS_WEBSERV_SUPERVISOR_HPP_

#include "Logging.hpp"
#include "ServerLocationFacade.hpp"

class SuperVisor {
 public:
    SuperVisor();
    SuperVisor(SuperVisor const &other);
    explicit SuperVisor(ServerLocationFacade);
    SuperVisor &operator=(SuperVisor const &other);
    ~SuperVisor();

    void Watch();

 private:
    ServerLocationFacade facade_;
    Logging logging_;
};
#endif  // SRCS_WEBSERV_SUPERVISOR_HPP_
