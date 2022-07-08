```mermaid
classDiagram

   class Request_facade{
        +select_request(socket) Request
    }


   class Request{
        string method
        string URI
        map~string, string~ header
        string request_body
        bool is_finish_to_read
        
        Request()
        Parse(socker) socket
        +get_port() int
        +get_host() string
        +get_path() string
        +is_finish_to_read() bool
    }

    class Response{
        Status_code status_code
        map~string, string~ header

        Response()
        Response(int)
        string response_body
        +Write(Socket) void
    }

    class Status_code{
        string status_code

        Status_code(int)
        +get_status_code() string
        +get_status_message() string
    }

Request <-- Request_facade : generate

Request  <--  Worker :call
Response  <--  Worker :call


Response "1" <-- "1" Status_code
```

## 擬似コード

```cpp
Worker {
    Worker(){
        Request_facade request_facade = new Request_facade()
    }

    void Exec() {
        for socket in socket_list
        {
            Request request  = request_facade(socket)
            try {
                request.Parse(socket_);

                if (request. is_finish_to_read())
                {
                    int port = request.get_port();
                    string host = request.get_host();
                    string path = request.get_path();
                    ServerLocation sl = facade_.Choose(port, host, path);

                    Response response = Someone.Exec(request_message, sl);
                    Response.Write(socket_), ;
                }
            }
            catch(400 error的な)
            {
                Response response = new response(400);
                Response.Write(socket_), ;
            }
            catch(500 error的な)
            {
                Response response = new response(500);
                Response.Write(socket_), ;
            }
            ...

        }
    }
};
```
