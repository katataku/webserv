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
        +vector<ServerContext> server_contexts;
        +map<int, string> error_pages
        +int client_max_body_size
        +bool autoindex
        +string index_page
        +CreateServerLocations()
    }

    %% TODO: listenディレクトリは複数指定できるかを確認 %%
    %% TODO: redirectの持ち方を検討 %%
    class ServerContext {
        +vector<LocationContext> location_contexts;
        +map<int, string> error_pages
        +int client_max_body_size
        +bool autoindex
        +string index_page
        +redirect : server & location
        +vector<string> server_names : only server
        +host : only server
        +port : only server
    }

    class LocationContext {
        +map<int, string> error_pages
        +int client_max_body_size
        +bool autoindex
        +string index_page
        +redirect
        +vector<string> allow_methods
        +string path
        +string alias
    }

    class Webserv {
        +Run()
    }

    %% IO多重化とソケットクラスを生成してWorkerに処理を依頼するまでを担当する%%
    class SuperVisor {
        -ServerLocationFacade facade_
        +Watch()
    }

    %% Socketを元に具体的な処理を担当する。 %%
    %% Requestを受け付けてResponseの返却をする %%
    class Worker {
        -Socket socket;
        +Exec() : void
    }

    %% どのServerLocationを使用するかを決定する責務 %%
    class ServerLocationFacade {
    }

    %% configを元に各locationごとの設定 %%
    class ServerLocation {
        +int port
        +string host
        +string path
    }
```

## 擬似コード
```
Webserv {
config = WebserverConfig();
serverLocations = config.CreateServerLocations();
ServerLocationFacade facade(serverLocations);
SuperVisor sv(facade);
sv.Run();
}

SuperVisor {
fd_set mask;
fd_set copy_mask;

SuperVisor {
    // 待ち受けるソケットの分だけListenerを生成する
    Listener()
    // IO多重化の準備(maskの初期化)
}

// listend_fd
// socke_fd

void Watch() {
    Socket sockets[];
    while (1) {
        copy_mask = mask;
        select(copy_mask, ...);
        for(;) {
            if (IS_SET(listend_fd)) {
                Scoket *s = Listener.Accept();
                sockets.add(s);
            } else if (IS_SET(socke_fd)) {
                Worker w(s);
                w.Exec();
            }
        }
    }
}
}

Worker {
void Exec() {
    Request *reqeust = Request.Parse(Socket.read());
    ServerLocation sl = ServerLocationGateway.Choose(port, host, path);
    Reponse *reponse = Someone.Exec(request, sl);
    Socket.Write(response);
}
}
```

## メモ
- 実際の処理の実行とResponseを生成する部分。
- 全体的にメンバ変数とメンバ関数を追加していきたい
- Configのparseする責務のクラスが必要。Lexer, Parse用のクラス。
- ServerLocationの持ち方はこれでいい？結局ServerとLocationで別に持ったほうがいいかも。デフォルトサーバー。
- WorkerがどのServerLocationを使うかのためにhostとportとpathの情報が必要
hostとpathはRequestを読まないと分からない。portはlisten_fdごとに判別するしかない？
Socketに持たせる必要があるかも。