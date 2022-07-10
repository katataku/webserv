```mermaid
classDiagram
   class RequestFacade{
        map~socket, Request~ list
        +SelectRequest(Socket) Request
        +Finish(Socket) void
    }

   class Request{
        string unparsed_string
        string method
        string URI
        string host
        string content-length
        string transfer-encoding
        string request_body
        bool IsFinishToRead
        
        +Parse(Socket) void
        +IsFinishToRead() bool
    }

    class Response{
        int status_code
        string connection
        string allow
        string location
        string response_body

        +Response(int)
        +Write(Socket) void
    }


Request <-- RequestFacade : generate

Worker <-- RequestFacade : generateしたもの or すでにあるものを返す
Worker --> RequestFacade : Requestクラスのインスタンスを依頼


Request  <--  Worker :call
Response  <--  Worker :call
```

## 擬似コード

```cpp
/*
requestを選択するためのFacade.
Requestの途中でrcve終了→再度epoll→途中から継続して読み込みののちに処理を開始するための仕組み。
*/
RequestFacade{
    private:
        map<socket, Request> list
    Request select_request(socket)
    {
        if (list not in socket)
            list[socket] = new Request;
        return list[socket];
    };
}
Request{
    request(){
        unparsed_string = "";
        IsFinishToReadHeader = false;
        IsReady = false;
    }

    //他チームのアドバイスを参考に追加
    Parse(string str){
        str = unparsed_string + str;
        if (not IsFinishToReadHeader)
        {
            if ("\n\n" is in str)
            {
                header, body = split(str, "\n\n")
                ParseHeader(header);
                unparsed_string = body
                IsFinishToReadHeader = true;
            }
        }
        if (IsFinishToReadHeader)
        {
            if (str.size() >= content-length)
            {
                ParseBody(str)
                IsReady = true; 
            } 
            else if (transfer-encoding = 'chunked' && str is in 最後のチャンク)
            {
                str = unchunk(str)
                ParseBody(str)
                IsReady = true; 
            }
            else if (ヘッダーだけ)
            {
                IsReady = true; 
            } 
            else
            }
            {
                unparsed_string += str
            }
        }
    }
}

Worker {
    Worker(){
        RequestFacade RequestFacade = new RequestFacade()
    }

    void Exec(socket) {
        Request& request  = RequestFacade(socket_)
        try {
            string str = socket.read();
            request.Parse(str);

            if (request.IsReady())
            {
                ServerLocation sl = facade_.Choose(request.get_port(), request.get_host(), request.get_path());

                Result response = Transaction.Exec(request, sl);

                Response.Write(socket_);
                RequestFacade.Finish(socket_);
                socket.should_close_socket = true;
            }
        }
        catch(400 error的な)
        {
            Response response = new response(400);
            Response.Write(socket_), ;
            RequestFacade.Finish(socket_);
            socket.should_close_socket = true;
        }
        catch(500 error的な)
        {
            Response response = new response(500);
            Response.Write(socket_), ;
            RequestFacade.Finish(socket_);
            socket.should_close_socket = true;
        }
        ...
    }
};
```
