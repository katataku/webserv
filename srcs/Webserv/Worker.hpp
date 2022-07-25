#ifndef SRCS_WEBSERV_WORKER_HPP_
#define SRCS_WEBSERV_WORKER_HPP_

#include "Logging.hpp"
#include "RequestFacade.hpp"
#include "ServerLocationFacade.hpp"
#include "Socket.hpp"

class Worker {
 public:
    Worker();
    Worker(Worker const &other);
    explicit Worker(ServerLocationFacade facade);
    Worker &operator=(Worker const &other);
    ~Worker();

    void Exec(Socket **socket);

 private:
    Logging logging_;
    RequestFacade *request_facade_;
    ServerLocationFacade server_location_facade_;
};

#endif  // SRCS_WEBSERV_WORKER_HPP_
