#include "SuperVisor.hpp"

#include <stdlib.h>

#include <string>
#include <vector>

#include "IOMultiplexer.hpp"
#include "Socket.hpp"
#include "Worker.hpp"

SuperVisor::SuperVisor() : logging_(Logging(__FUNCTION__)) {}

SuperVisor::SuperVisor(SuperVisor const &other) { *this = other; }

SuperVisor &SuperVisor::operator=(SuperVisor const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

SuperVisor::~SuperVisor() {}

SuperVisor::SuperVisor(ServerLocationFacade *facade)
    : facade_(facade), logging_(Logging(__FUNCTION__)) {}

void SuperVisor::Watch() {
    std::vector<std::string> ports = this->facade_->GetPorts();

    try {
        IOMultiplexer iomul(ports);

        this->logging_.Debug("start loop");

        while (true) {
            std::vector<Socket *> sockets = iomul.WaitAndGetReadySockets();
            for (std::vector<Socket *>::iterator itr = sockets.begin();
                 itr != sockets.end(); ++itr) {
                Socket *socket = *itr;

                if (socket->is_event_in()) {
                    if (socket->is_listening()) {
                        iomul.Accept(*socket);
                        delete socket;
                        continue;
                    }
                    Worker worker(this->facade_);
                    if (worker.ExecReceive(socket)) {
                        int fd = socket->sock_fd();
                        iomul.ChangeEpollOutEvent(fd);
                    }
                } else if (socket->is_event_out()) {
                    Worker worker(this->facade_);
                    if (worker.ExecSend(socket)) {
                        int fd = socket->sock_fd();
                        iomul.CloseFd(fd);
                    }
                } else {
                    this->logging_.Debug("here");
                    // fdのclose
                }
            }
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}
