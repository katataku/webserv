#include "ServerLocationFacade.hpp"

#include <map>

#include "ServerLocationKey.hpp"
#include "utils.hpp"

ServerLocationFacade::ServerLocationFacade()
    : logging_(Logging(__FUNCTION__)) {}

ServerLocationFacade::ServerLocationFacade(
    std::vector<ServerLocation> server_locations)
    : server_locations_(server_locations) {}

ServerLocationFacade::ServerLocationFacade(ServerLocationFacade const &other) {
    *this = other;
}

ServerLocationFacade &ServerLocationFacade::operator=(
    ServerLocationFacade const &other) {
    if (this != &other) {
        this->server_locations_ = other.server_locations_;
    }
    return *this;
}

ServerLocationFacade::~ServerLocationFacade() {}

// TODO(takkatao): chooseの処理を実装する。
// 現在は、2番目のSLを取得するようにする。(初めに出てくるLocationのSL)
ServerLocation *ServerLocationFacade::Choose(std::string port, std::string host,
                                             std::string path) {
    (void)port;
    (void)host;
    (void)path;

    logging_.Debug("Choose start");
    logging_.Debug("ServerLocation.size() = [" +
                   numtostr<int>(this->server_locations_.size()) + "]");

    return &this->server_locations_.at(0);
    // pathの最長のものから試していく。どれもマッチしない場合はデフォルト設定
}

std::vector<std::string> ServerLocationFacade::GetPorts() const {
    std::vector<std::string> ports;

    ports.push_back("80");
    return ports;
}
