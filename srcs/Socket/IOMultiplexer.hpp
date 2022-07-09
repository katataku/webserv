#ifndef SRCS_SOCKET_IOMULTIPLEXER_HPP_
#define SRCS_SOCKET_IOMULTIPLEXER_HPP_

#include <string>
#include <vector>

#include "Logging.hpp"
#include "Socket.hpp"

class IOMultiplexer {
 public:
    IOMultiplexer();
    IOMultiplexer(IOMultiplexer const &other);
    IOMultiplexer &operator=(IOMultiplexer const &other);
    ~IOMultiplexer();

    void Init(std::vector<std::string>);
    std::vector<Socket> Wait();
    void Accept(Socket &);

 private:
    Logging logging_;
};

#endif  // SRCS_SOCKET_IOMULTIPLEXER_HPP_
