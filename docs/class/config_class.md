```mermaid
classDiagram
    Webserv *-- WebservConfig
    WebservConfig "1" *-- "1..*" ServerContext
    ServerContext "1" *-- "1..*" LocationContext

    Webserv *-- SuperVisor
    Webserv *-- ServerLocation
    SuperVisor --> Worker : use
    Worker --> ServerLocation : use

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

SuperVisor {
    Socket.Listen();
    IO多重化の準備
}

void Watch() {
    while (1) {
        Scoket *s = Sokcet.Accept();
        Worker w(s);
        w.Exec();
    }
}

}

Worker {
void Exec() {
    Request *reqeust = Request.Parse(Socket.read());
    ServerLocation sl = ServerLocationGateway.Choose(port, host, path);
    Reponset *reponse = Someone.Exec(request, sl);
    Socket.Write(response);
}
}
```
