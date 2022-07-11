``` mermaid
classDiagram
    class CGIRequest {
      +static HTTPReqToCGIReq(HTTPRequest)* CGIRequest
      +CGIPath()
      +CGIArg()
      +CGIEnv()
    }

    class CGIResponse {
      +CGIResToHTTPRes()* HTTPResponse
    }

    class CGIExecutor {
      +Exec(HTTPRequest, ServerLocation) : HTTPResponse
    }
```

- 擬似コード

``` cpp

class CGIRequest {
  public:
    CGIpath() {}
    CGIarg() {}
    CGIenv() {}
    bool ShouldSendRequestBody() {}
    static HTTPReqToCGIReq(HTTPRequest httpreq) {
      // TBD
    }

  private:
    string CONTENT_LENGTH
    string CONTENT_TYPE
    string PATH_INFO
    string REQUEST_METHOD
    string SERVER_PROTOCOL
    string CGIpath
    string[] CGIarg
    string[] CGIenv
}

class CGIResponse {
  public:
    HTTPResponse CGIResToHTTPRes() {
      // TBD
    }
  
  public:
    string Status
    string Content-Type
    string body
}

class CGIExecutor {
  public:
    HTTPResponse Exec(HTTPRequest httpreq, ServerLocation sl) {
      CGIRequest cgireq = CGIRequest::HTTPReqToCGIReq(httpreq)

      CGIResponse cgires = ExecCGI(cgireq)
      return cgires.CGIResToHTTPRes()
    }

  private:
    CGIResponse ExecCGI(CGIRequest cgireq, ServerLocation sl) {
      pipefd[2];
      rpipefd = pipefd[0]
      wpipefd = pipefd[1]

      if fork() == 0 {
        dup2(rpipefdf, STDIN_FILENO)
        dup2(wpipefdf, STDOUT_FILENO)
        execve(cgireq.CGIpath(), cgireq.CGIarg(), cgireq.CGIenv())
        close(rpipefdf)
        close(wpipefdf)
      }
      if cgireq.ShouldSendRequestBody() {
        dprintf(wpipefd, "%s", cgireq.body())
      }
      close(wpipefd)
      WaitAndCountTime() // タイムアウト計測したい感ある
      CGIResponse cgires = CGIResponse::Receive(rpipefdf)
      return cgires
    }
}

```

# メモ
- CGIExecutor内でHTTPRequestからCGIRequestを作成
