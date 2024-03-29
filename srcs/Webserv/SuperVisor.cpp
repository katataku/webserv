#include "SuperVisor.hpp"

#include <cstdlib>
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

// 一連のConnectionの過程で確保したresourceを解放する
static void FinalizeResource(Socket *socket, IOMultiplexer &iomul) {
    // requestおよびresponseの解放
    RequestFacade *request_facade = RequestFacade::GetInstance();
    HTTPRequest *request = request_facade->SelectRequest(*socket);
    HTTPResponse *response = request->response();
    delete response;
    request_facade->Finish(socket);
    // socketの解放
    int fd = socket->sock_fd();
    iomul.CloseFd(fd);
    socket->Close();
    delete socket;
}

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

                try {
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
                        delete socket;
                    } else if (socket->is_event_out()) {
                        Worker worker(this->facade_);
                        if (worker.ExecSend(socket)) {
                            FinalizeResource(socket, iomul);
                        }
                    } else {
                        throw Socket::SocketIOException("epoll fatal event");
                    }
                } catch (Socket::SocketIOException &e) {
                    this->logging_.Debug("SocketIOException");
                    FinalizeResource(socket, iomul);
                }
            }
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}
