- Class 図

// クラス図に型を追記する

```mermaid
classDiagram
  class Socket {
    -socketfd
    +Send()
    +Recieve()
    +IsListening()
    <!-- +CreateListener() -->
  }

  class IOMultiplexer {
    -Socket[]
    +Init()
    +Wait()
    -AddSocket()
    -CreateListenerSocket()
  }

  IOMultiplexer "1" *-- "0..n" Socket
```

- 疑似コード

// メソッドの処理の流れをかく

```cpp

class Socket {
  int socketfd

  // Responseを受け取りソケットに書き込む
  void Send(Response)

  // ソケットを読み込み、何かしらのデータとして返す
  string Recieve()
  Request Recieve()

  // リスニング状態のソケットを返す
  // Socket CreateListener(WebservConfig)
  // Socket CreateListener(port)
  // void CreateListener(port)

  // ソケットがリスニング状態か確認(getsockoptで情報を取得できる)
  bool IsListening()
}

// I/O多重化の債務をやってくれるやつ（インターフェースにしても良いけど、select使う気はほぼ0なのでやる意味はあんまりない）
class IOMultiplexer {
    // publicにして直接扱うようにしたい（コピーしたくない）
    Socket[] sockets // クラスでこれを持つかは悩み中

    GetterSocket(index)

    //
    CreateListenerSocket()

    // listenソケットをセットするやつ
    AddSocket(Socket)

    // listenソケットがreadyになるとソケット群にクライアントとのソケットを追加したいのでそれを担うやつ
    AddNewClient()

    // socket群を初期化するやつ
    Init() // 他のクラスと命名を統一化するためにもSetupでも良い

    // ソケット群がready状態になるまで待機
    Wait()
}

// ポートのリストをとる
// ServerLocationFacade.Getpostlist

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
        IOMultiplexer iomul()

        iomul.Init()
          //iomul.CreateListenerSocket()
          //iomul.Init(listener)

        while (1) {
            List socketlist = iomul.Wait()
            for socket in socketlist
            {
              if (socket is accept必要)
                iomul.Accept(socket)
              else
                Worker.Exec(socket)
            }
        }
    }
}

```

## メモ

listen ソケットを作るにはポート番号を知る必要がある
ポート番号は Config から取れる
Config は facade 経由で取ろうとしている
つまり、SuperVisor で facade を持つ必要がある

なんか SuperVisor の責務多くない？

I/O 多重化をクラス化するの難しい
select も epoll も生の fd を何かしらの構造体にセットする
クラス化した時のコピーとか代入するときに、fd を dup しているけどそれが使えるかはわからない気がする

select も epoll も、処理手順に共通項があるはず

1. fd 集合の初期化

- select
  - fd_set の初期化
- epoll
  - listen 状態のソケットを作成し、epoll インスタンスへの参照

2. Wait（ready 状態になるまで待つ）

- select
  - select システムコールを呼ぶ
- epoll
  - epoll_wait システムコールを呼ぶ

3. fd 集合をループで回す

- それはこいつがやらなくて良い

4. 新しくコネクションが確立されたら accept してクライアントとのソケットを作成し、そうじゃなかったらすでにあるソケットへ何かしらする

- epoll
  - epoll_event とかをよしなにして epoll_ctl でそのソケットを epoll インスタンスに参照させる必要がある
- select
  　　- おんなじ感じ
