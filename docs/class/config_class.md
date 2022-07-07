```mermaid
classDiagram
    Webserv *-- WebservConfig
    WebservConfig "1" *-- "1..*" ServerContext
    ServerContext "1" *-- "1..*" LocationContext

    Webserv *-- SuperVisor
    Webserv *-- ServerLocation
    SuperVisor --> Worker : use
    Worker --> ServerLocationGateway : use
    ServerLocationGateway --> ServerLocation: use

    class WebservConfig {
        +error_page
    }

    class ServerContext {
        +server_name
        +host
        +port
        +error_page
    }

    class LocationContext {
        +error_page
    }

    class Webserv {
        +Run()
    }

    %% IO多重化とソケットクラスを生成してWorkerに処理を依頼するまでを担当する%%
    class SuperVisor {
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

## idea
- WorkerがどのServerLocationを使うかのためにhostとportとpathの情報が必要
hostとpathはRequestを読まないと分からない。portはlisten_fdごとに判別するしかない？
Socketに持たせる必要があるかも。

## 擬似コード
```
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
- Watchの
