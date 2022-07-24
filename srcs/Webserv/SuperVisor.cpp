#include "SuperVisor.hpp"

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

SuperVisor::SuperVisor(ServerLocationFacade facade)
    : facade_(facade), logging_(Logging(__FUNCTION__)) {}

void SuperVisor::Watch() {
    IOMultiplexer iomul;
    std::vector<std::string> ports = this->facade_.GetPorts();
    iomul.Init(ports);
    this->logging_.Debug("start loop");
    while (true) {
        std::vector<Socket *> sockets = iomul.Wait();
        for (std::vector<Socket *>::iterator itr = sockets.begin();
             itr != sockets.end(); ++itr) {
            if ((*itr)->is_listening()) {
                iomul.Accept(*(*itr));
            } else {
                int fd = (*itr)->sock_fd();
                Worker worker(this->facade_);
                worker.Exec(*itr);
                if (*itr == NULL) iomul.CloseFd(fd);
            }
        }
    }
}
