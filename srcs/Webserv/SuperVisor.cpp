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
    : facade_(facade), logging_(Logging(__FUNCTION__)) {
  std::cout << "SuperVisor server_locations_ : " << facade_.get_server_locations_() << std::endl;
}

void SuperVisor::Watch() {
    IOMultiplexer iomul;
    std::vector<std::string> ports = facade_.GetPorts();
    iomul.Init(ports);
    this->logging_.Debug("start loop");
    while (true) {
        std::vector<Socket> sockets = iomul.Wait();
        for (std::vector<Socket>::iterator itr = sockets.begin(); itr != sockets.end(); ++itr) {
            if ((*itr).is_listening()) {
                iomul.Accept(*itr);
            } else {
                Worker worker(facade_);
                worker.Exec(*itr);
            }
        }
    }
}
