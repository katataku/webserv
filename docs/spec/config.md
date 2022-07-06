# webserv設定ファイル
設定ファイルはwebservの起動時に読み込まれる。

設定ファイルが読み込まれるまではクライアントからのリクエストを待ち受けない。

## 設定ファイルの構成
設定ファイルは単一ディレクティブとブロックディレクティブから構成されます。

単一ディレクティブはパラメータ名と設定値が空白で分けられセミコロンで終わります。 ブロックディレクティブは波括弧の中に単一ディレクトリとブロックディレクティブを定義します。

ブロックディレクティブが波括弧の中に他のディレクティブを持つことができる場合はコンテキストと呼ばれます。 webservではhttp, server, locationの3つのコンテキストがあります。 locationの中にlocationを含めることはできません。コンテキストの外に置かれたディレクティブはhttpコンテキストに属します。

複数サーバーが設定されている場合は一番始めに定義されたサーバーをデフォルトサーバーとする。

`#`が現れるとその行末まではコメントアウトとして解釈される。

設定値に正規表現を使用することはできず通常テキストで指定する必要がある。

## 設定可能な項目一覧

### [server_name]
virtual serverの名前を設定する。

Usage: 
```
Syntax:	server_name name ...;
Default: server_name "";
Context: server
```

Example:
```
server_name example.com www.example.com;
```

#### 確認
これは何に使われる？リダイレクト？
複数設定を許可するかどうかは用途を確認した後判断。

### [listen]
サーバーがリクエストを受け付けるホスト名とポート番号を設定する。

Usage: 
```
Syntax: listen host:port
Default: listen 127.0.0.1:8000;
Context: server
```

Example:
```
listen 127.0.0.1:8000;
listen localhost:8000;
```

#### 相談
ホスト名やポートは省略可能にする？実装的には必須にするのが楽。

#### 確認
ホスト名がローカルホスト以外の場合の挙動
1つのサーバーに複数渡せる？

### [error_page]
特定のエラーに対して表示するページを設定することができる。

設定されていない場合はデフォルトのエラーページが表示される。

Usage: 
```
Syntax:	error_page code ... uri;
Default: —
Context: http, server, location
```

Example:
```
error_page 404 /404.html;
error_page 500 502 503 504 /50x.html;
```

### [client_max_body_size]
リクエストボディで許可する最大サイズを設定する（単位はバイト）。

0以下の値は設定できない。

設定値を超えるリクエストが来た場合は413(Request Entity Too Large)を返す。

Usage: 
```
Syntax:	client_max_body_size size;
Default: client_max_body_size 1024;
Context: http, server, location
```

Example:
```
client_max_body_size 1024;
```

#### 確認
ブラウザ側の挙動も確認しておく？

### [alias]
ロケーションで指定されたパスに対するエイリアスを設定できる。

Usage: 
```
Syntax: alias path;
Default: —
Context: location
```

Example:
```
location /kapouet {
    alias /tmp/www;
}
```
url  is /tmp/www/pouic/toto/pouet.

### 備考
rootには対応しない。rootとaliasの違い。

```
location /kapouet {
    alias /tmp/www;
}

location /kapouet {
    root /tmp/www;
}
```

`/kapouet/pouic/toto/pouet`のURLにアクセスした場合、それぞれ以下にアクセスされる。
- /tmp/www/pouic/toto/pouet
- /tmp/www/kapouet/pouic/toto/pouet


### [limit_except]

設定されたリクエストメソッド以外のリクエストを制限する。

許可されていないメソッドでのリクエストに対しては403(Forbidden)を返す。

Usage: 
```
Syntax:	limit_except method ... { deny all; }
Default: —
Context: location
```

Example:
```
limit_except GET POST { deny all; }
```

### [autoindex]
ディレクトリの一覧表示を行うかどうかを設定できる。

Usage: 
```
Syntax:	autoindex on | off;
Default: autoindex off;
Context: http, server, location
```

Example:
```
autoindex on;
```

### [index]
ディレクトリのデフォルトページを設定する。

Usage:
```
Syntax:	index file;
Default: index index.html;
Context: http, server, location
```

Example:
```
index index.html;
```

### [rewrite]
If the specified regular expression matches a request URI, URI is changed as specified in the replacement string. The rewrite directives are executed sequentially in order of their appearance in the configuration file. It is possible to terminate further processing of the directives using flags. If a replacement string starts with “http://”, “https://”, or “$scheme”, the processing stops and the redirect is returned to a client.

Usage: 
```
Syntax:	rewrite regex replacement [flag];
Default: —
Context: server, location
```

Example:
```
```

#### 確認
挙動の確認。
- 内部的なリダイレクト。virtual hostにリダイレクト。
- 302を返すリダイレクト。

### [server]
Sets configuration for a virtual server. There is no clear separation between IP-based (based on the IP address) and name-based (based on the “Host” request header field) virtual servers. Instead, the listen directives describe all addresses and ports that should accept connections for the server, and the server_name directive lists all server names. Example configurations are provided in the “How nginx processes a request” document.

Usage: 
```
Syntax:	server { ... }
Default: —
Context: http
```

### [location]
Sets configuration depending on a request URI.

複数マッチした場合は最長のものにする。

Usage: 
```
Syntax:	location [ = | ~ | ~* | ^~ ] uri { ... }
location @name { ... }
Default: —
Context: server
```

#### 確認
- 相対パス表記できる？
- A location can either be defined by a prefix string, or by a regular expression.

[server_name]: https://nginx.org/en/docs/http/ngx_http_core_module.html#server_name
[listen]: https://nginx.org/en/docs/http/ngx_http_core_module.html#listen
[error_page]: https://nginx.org/en/docs/http/ngx_http_core_module.html#error_page
[client_max_body_size]: https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
[root]: https://nginx.org/en/docs/http/ngx_http_core_module.html#root
[limit_except]: https://nginx.org/en/docs/http/ngx_http_core_module.html#limit_except
[autoindex]: https://nginx.org/en/docs/http/ngx_http_autoindex_module.html#autoindex
[rewrite]: https://nginx.org/en/docs/http/ngx_http_rewrite_module.html#rewrite
[location]: https://nginx.org/en/docs/http/ngx_http_core_module.html#location
[server]: https://nginx.org/en/docs/http/ngx_http_core_module.html#server
[index]: https://nginx.org/en/docs/http/ngx_http_index_module.html#index
