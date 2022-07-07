- Class図

``` mermaid
classDiagram
  class Socket {
    -socketfd
    +Send()
    +Recieve()
    +CreateListener()
    +IsListening()
  }

  class IOMultiplexer {
    +Socket[]
    +AddSocket()
    +Init()
    +Wait()
  }
```

- 疑似コード

``` cpp

class Socket {
  int socketfd

  // Responseを受け取りソケットに書き込む
  void Send(Response)

  // ソケットを読み込み、何かしらのデータとして返す
  string Recieve()
  Request Recieve()

  // リスニング状態のソケットを返す
  Socket CreateListener(WebservConfig)
  Socket CreateListener(port)
  void CreateListener(port)

  // ソケットがリスニング状態か確認(getsockoptで情報を取得できる)
  bool IsListening()
}

// I/O多重化の債務をやってくれるやつ（インターフェースにしても良いけど、select使う気はほぼ0なのでやる意味はあんまりない）
class IOMultiplexer {
    // publicにして直接扱うようにしたい（コピーしたくない）
    Socket[] sockets // クラスでこれを持つかは悩み中

    // listenソケットをセットするやつ
    AddSocket(Socket)

    // listenソケットがreadyになるとソケット群にクライアントとのソケットを追加したいのでそれを担うやつ
    AddNewClient()

    // socket群を初期化するやつ
    Init() // 他のクラスと命名を統一化するためにもSetupでも良い

    // ソケット群がready状態になるまで待機
    Wait()
}

class SuperVisor {
    // Socket Acceptor
    // Socket Listenerでは？
    ServerLocationFacade facade

    // Constructorでやる？
    void Setup() {
        // 待ち受けるポートの数だけlisten fdを用意する
        // Acceptor.CreateListner()
        Listener.CreateListner(facade.Chose().port())
    }

    void Watch() {
        IOMultiplexer iomul
        iomul.AddSocket(Listener)
        iomul.Init()

        while (1) {
            numofready = iomul.Wait()
            for i = 0; i < numofready; ++i {
                if iomul.sockets[i].IsListening() {
                    iomul.AddNewClient()
                    continue
                }
                StartTransact(iomul.sockets[i]) // 適当。Workerに移譲するとか
            }
        }
    }
}

```

## メモ
listenソケットを作るにはポート番号を知る必要がある
ポート番号はConfigから取れる
Configはfacade経由で取ろうとしている
つまり、SuperVisorでfacadeを持つ必要がある

なんかSuperVisorの責務多くない？

I/O多重化をクラス化するの難しい
selectもepollも生のfdを何かしらの構造体にセットする
クラス化した時のコピーとか代入するときに、fdをdupしているけどそれが使えるかはわからない気がする

selectもepollも、処理手順に共通項があるはず
1. fd集合の初期化
  - select
    - fd_setの初期化
  - epoll
    - listen状態のソケットを作成し、epollインスタンスへの参照
2. Wait（ready状態になるまで待つ）
  - select
    - selectシステムコールを呼ぶ
  - epoll
    - epoll_waitシステムコールを呼ぶ
3. fd集合をループで回す
  - それはこいつがやらなくて良い
4. 新しくコネクションが確立されたらacceptしてクライアントとのソケットを作成し、そうじゃなかったらすでにあるソケットへ何かしらする
  - epoll
    - epoll_eventとかをよしなにしてepoll_ctlでそのソケットをepollインスタンスに参照させる必要がある
  - select
  　　- おんなじ感じ
