- Class図

``` mermaid
classDiagram
  class Socket {
    -socketfd
    +Send()
    +Recieve()
  }
```

- 疑似コード

``` cpp

// Workerはサーバーとクライアント
class Worker {

}

// SuperVisorはクライアントの接続とか色々監視するやつ
class SuperVisor {
  void Watch(Socket listener) {
    
  }
}

// Webservは設定ファイルを持っていて、それをもとに何か処理する
class Webserv {
  ServerLocationFacade facade  // 設定ファイルのインターフェース。
  // SuperVisor visor             // 監視者

  // Runは実質のmainループ
  //   Runは変更容易性は低くておkな気がする
  void Run() {
    Socket listener = Socket::MakeListener(facade.Chose().GetPort())
    
    SuperVisor::Watch(listener)
  }
}

int main(args) {
  // どっちでも良い
  // WebservConfig conf(args)
  // WebservConfig conf = WebservConfig::Parse(args)
  // Webserv serv(conf)
  Webserv serv(args)

  serv.Run()
}
```
