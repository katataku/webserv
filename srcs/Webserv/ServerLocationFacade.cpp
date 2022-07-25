#include "ServerLocationFacade.hpp"

#include <map>

#include "ServerLocationKey.hpp"
#include "utils.hpp"

ServerLocationFacade::ServerLocationFacade()
    : logging_(Logging(__FUNCTION__)) {}

ServerLocationFacade::ServerLocationFacade(
    std::vector<ServerLocation> server_locations) {
    std::vector<ServerLocation>::iterator itr = server_locations.begin();
    for (; itr != server_locations.end(); ++itr) {
        // portをstd::stringで持ったほうが良さそう
        std::stringstream ss;
        ss << itr->port();
        ServerLocationKey key(ss.str(), itr->host());
        this->server_locations_[key][itr->path()] = *itr;
    }
}

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
ServerLocation *ServerLocationFacade::Choose(std::string port, std::string host,
                                             std::string path) {
    logging_.Debug("Choose start");
    logging_.Debug("ServerLocation.size() = [" +
                   numtostr<int>(this->server_locations_.size()) + "]");

    // 仮想サーバーの決定 -> pathの決定という順番で処理を行う
    // pathの最長のものから試していく。どれもマッチしない場合はデフォルト設定
    ServerLocationKey key(port, host);
    return &this->server_locations_[key][path];
}

std::vector<std::string> ServerLocationFacade::GetPorts() const {
    std::vector<std::string> ports;

    ports.push_back("80");
    return ports;
}
