#include "SuperVisor.hpp"

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
void SuperVisor::Watch() { logging_.Debug("start watch"); }
