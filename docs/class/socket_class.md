- Class 図

```mermaid
classDiagram
  class Socket {
    -int socketfd
    -bool is_listening
    +Send()* void
    +Recieve()* string
    +IsListening()* bool
    <!-- +CreateListener() -->
  }

  class IOMultiplexer {
    -Socket[] sockets
    -CreateListenerSocket(string port)* void
    +Init()* void
    +Wait()* Socket[]
    +Accept(Socket)* void
  }

  IOMultiplexer "1" *-- "0..n" Socket
```

- 疑似コード

```cpp

class Socket {
  private:
      int socketfd
      bool is_listening

  public:
      // Responseを受け取りソケットに書き込む
      void Send(string)

      // ソケットを読み込み、何かしらのデータとして返す
      string Recieve()

      // ソケットがリスニング状態か確認
      bool IsListening()
}

// I/O多重化の債務をやってくれるやつ
class IOMultiplexer {
    private:
        // ソケット群
        Socket[] sockets

        // listen状態のソケット
        CreateListenerSocket(string port)

    public:
        // socket群を初期化するやつ
        void Init(string[] port_list) {
            for port in port_list {
                CreateListenerSocket(port)
            }
            ... // epoll固有の実装なので省略
        }

        // ソケット群がready状態になるまで待機
        Socket[] Wait() {
          ... // epoll固有の実装なので省略
        }

        // listen状態のソケットを受け取り、acceptして新しいクライアントとのソケットをソケット群に追加
        void Accept(Socket listener) {
          Socket client = listener.Accept()
          ... // epoll固有の実装なので省略
          sockets.Add(client)
        }
}

// Socket、IOMultiplexerの使い方について記述するためSuperVisorの擬似コードを記載
class SuperVisor {
    private:
        ServerLocationFacade facade

    public:
        void Watch() {
            IOMultiplexer iomul
            string[] port_list = ServerLocationFacade.Getpostlist()
            iomul.Init(port_list)
            while (1) {
                Socket[] sockets = iomul.Wait()
                for socket in sockets {
                    if socket.IsListening
                        iomul.Accept(socket)
                    else
                        Worker.Exec(socket)
                }
            }
        }
}

```
