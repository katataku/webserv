```mermaid
classDiagram
    GetFileExecutor <|-- IExecutor
    CGIExecutor <|-- IExecutor
    ListingExecutor <|-- IExecutor

    Transaction --> IExecutor : use
    GetFileExecutor --> ResponseBuilder : use
    CGIExecutor --> ResponseBuilder : use
    ListingExecutor --> ResponseBuilder : use

    class GetFileExecutor {
        +Execute(Request, ServerLocation) Response
    }

    class CGIExecutor {
        +Execute(Request, ServerLocation) Response
    }

    class ListingExecutor {
        +Execute(Request, ServerLocation) Response
    }

    class IExecutor {
        +Execute(Request, ServerLocation) Response
    }


    class Transaction {
        +Execute(Request, ServerLocation) Response
    }

```

```cpp
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

class Transaction {
    Result Exec(request, sl) {
        if (redirect) {
            return Result.Redirect(request, sl);
        }
        if (hogehoge) {
            return GetFileExecutor(request, sl);
        } else if (
            return CGIExecutor(request, sl);
        ) else if {
            return ListingExecutor(request, sl);
        }
    }
}
```
