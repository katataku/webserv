#ifndef SRCS_WEBSERV_WORKER_HPP_
#define SRCS_WEBSERV_WORKER_HPP_

#include "Logging.hpp"
#include "Socket.hpp"

class Worker {
 public:
    Worker();
    Worker(Worker const &other);
    Worker &operator=(Worker const &other);
    ~Worker();

    void Exec(Socket &);

 private:
    Logging logging_;
};

#endif  // SRCS_WEBSERV_WORKER_HPP_
