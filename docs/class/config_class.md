```mermaid
classDiagram
    Webserv *-- WebservConfig
    WebservConfig "1" *-- "1..*" ServerContext
    ServerContext "1" *-- "1..*" LocationContext

    Webserv *-- SuperVisor
    Webserv *-- ServerLocation
    SuperVisor --> Worker : use
    Worker --> ServerLocationFacade : use
    ServerLocationFacade --> ServerLocation: use

    class WebservConfig {
        +vector~ServerContext~ contexts_
        +map~int, string~ error_pages
        +int client_max_body_size
        +string autoindex
        +string index_page
        +CreateServerLocations()
        +Parse(string)
    }

    class ServerContext {
        +vector~LocationContext~ contexts_
        +map~int, string~ error_pages
        +int client_max_body_size
        +string autoindex
        +string index_page
        +string redirect_url
        +string server_name
        +int port
    }

    class LocationContext {
        -map~int, string~ error_pages
        -int client_max_body_size
        -string autoindex
        -string index_page
        -string path
        -string redirect_url
        -set~string~ allow_methods
        -string alias
        -string cgi_extension
    }

    class Webserv {
        +Run()
    }

    %% IO多重化とソケットクラスを生成してWorkerに処理を依頼するまでを担当する%%
    class SuperVisor {
        -ServerLocationFacade facade_
        +Watch() void
    }

    %% Socketを元に具体的な処理を担当する。 %%
    %% Requestを受け付けてResponseの返却をする %%
    class Worker {
        +Exec(Socket socket) void
    }

    %% どのServerLocationを使用するかを決定する責務 %%
    class ServerLocationFacade {
        +Choose(port, host, path) ServerLocation*
        +GetPorts() vector~string~
    }

    %% configを元に各locationごとの設定 %%
    class ServerLocation {
        -int port
        -string host
        -string path
        -map~int, string~ error_pages
        -int client_max_body_size
        -string autoindex
        -string index_page
        -string redirect_url
        -set~string~ allow_methods
        -string path
        -string alias
        -string cgi_extension
        +IsRedirect() bool
        +IsCGI(string path) bool
        +ResolveAlias(string path_info) string;
    }
```

## 擬似コード

```cpp
Webserv {
    // IDEA: 別途Lexer&Parserの責務を持ったクラスを定義しても良いかもしれない。
    void Run() {
        WebservConfig config = WebservConfig.Parse();
        vector<ServerLocation> locations = config.CreateServerLocations();
        ServerLocationFacade facade(locations);
        SuperVisor sv(facade);
        sv.Watch();
    }
};

// ソケット周りのクラスが固まっていないので雰囲気で書いている。
SuperVisor {
    // Constructorでやる？
    void Setup() {
        // 待ち受けるポートの数だけlisten fdを用意する
        Acceptor.CreateListner();
    }

    void Watch() {
        while (1) {
            vector<Socket> sockets = Acceptor.Select();
            for (socket in sockets) {
                if (socket.isListner()) {
                    // acceptしてfdを生成してselectの対象にする
                    Acceptor.AddListner(socket);
                } else {
                    Worker w(socket);
                    w.Exec();
                }
            }
        }
    }
};
```

## メモ

- ServerLocationはServerとLocationに分けたほうがいい可能性もある。
- is_defaultみたいなどれがデフォルトサーバーなのかを示すパラメータが必要そう
