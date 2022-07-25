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
        std::string port = numtostr<int>(itr->port());
        ServerLocationKey key(port, itr->host());
        this->server_locations_[key][itr->path()] = *itr;
        // server_locationsが.confファイルと同じ順番に並んでいることを期待している処理
        if (this->default_server_keys_.find(port) ==
            this->default_server_keys_.end()) {
            this->default_server_keys_[port] = key;
        }
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

// 仮想サーバーの決定 -> pathの決定という順番で処理を行う
// pathについてはtrailing slashがある前提で実装している
// TODO(ahayashi): 相対パスの解決
ServerLocation ServerLocationFacade::Choose(std::string port, std::string host,
                                            std::string path) {
    logging_.Debug("Choose start");
    logging_.Debug("ServerLocation.size() = [" +
                   numtostr<int>(this->server_locations_.size()) + "]");

    // 仮想サーバーの決定。マッチしない場合はポートごとのデフォルトサーバーを用いる。
    ServerLocationKey key(port, host);
    if (this->server_locations_.find(key) == this->server_locations_.end()) {
        key = this->default_server_keys_[port];
    }
    std::map<std::string, ServerLocation> server = this->server_locations_[key];
    for (std::map<std::string, ServerLocation>::iterator itr = server.begin();
         itr != server.end(); ++itr) {
        std::cout << itr->second << std::endl;
    }
    // pathの最長のものから試していく。どれもマッチしない場合はデフォルト設定
    std::string::size_type pos = path.find_last_of("/");
    for (; path != "" && pos != std::string::npos;) {
        path = path.substr(0, pos + 1);
        std::cout << "path: " << path << std::endl;
        if (server.find(path) != server.end()) {
            return server[path];
        }
        path = path.substr(0, path.size() - 1);
        pos = path.find_last_of("/");
    }
    return server[""];
}

std::vector<std::string> ServerLocationFacade::GetPorts() const {
    std::vector<std::string> ports;

    ports.push_back("80");
    return ports;
}
