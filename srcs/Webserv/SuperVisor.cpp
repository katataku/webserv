#include "SuperVisor.hpp"

#include <string>
#include <vector>

#include "IOMultiplexer.hpp"
#include "Socket.hpp"
#include "Worker.hpp"

SuperVisor::SuperVisor() { Logging logging_ = Logging(__FUNCTION__); }

SuperVisor::SuperVisor(SuperVisor const &other) { *this = other; }

SuperVisor &SuperVisor::operator=(SuperVisor const &other) {
    if (this != &other) {
        (void)other;
    }
    return *this;
}

SuperVisor::~SuperVisor() {}

SuperVisor::SuperVisor(ServerLocationFacade facade) : facade_(facade) {}

void SuperVisor::Watch() {
    IOMultiplexer mux;
    std::vector<std::string> ports = facade_.GetPorts();
    mux.Init(ports);
    while (true) {
        this->logging_.Debug("start loop");
        std::vector<Socket> sockets = mux.Wait();
        std::vector<Socket>::iterator it = sockets.begin();
        for (; it != sockets.end(); it++) {
            Socket &socket = *it;
            if (it->is_listening()) {
                mux.Accept(socket);
            } else {
                Worker worker;
                worker.Exec(socket);
            }
        }
        break;
    }
}
