# webserv設定ファイル

設定ファイルはwebservの起動時に読み込まれる。

設定ファイルが読み込まれるまではクライアントからのリクエストを待ち受けない。

## 設定ファイルの構成

設定ファイルは単一ディレクティブとブロックディレクティブから構成される。

単一ディレクティブはパラメータ名と設定値が空白で分けられセミコロンで終わる。 ブロックディレクティブは波括弧の中に単一ディレクトリとブロックディレクティブを定義する。

ブロックディレクティブが波括弧の中に他のディレクティブを持つことができる場合はコンテキストと呼ばれる。 webservではhttp, server, locationの3つのコンテキストがある。 locationの中にlocationを含めることはできません。コンテキストの外に置かれたディレクティブはhttpコンテキストに属する。

複数サーバーが設定されている場合は一番始めに定義されたサーバーをデフォルトサーバーとする。

`#`が現れるとその行末まではコメントアウトとして解釈される。

設定値に正規表現を使用することはできず通常テキストで指定する必要がある。

## nginxとの相違

webservの設定ファイルはnginxのサブセットを目指しているが、以下の点が異なっている。

- nginxではlilmit_exceptはブロックディレクトリだがシンプルディレクティブにしている
- CGI周り

## 設定可能な項目一覧

### [server_name]

仮想サーバーの名前を設定する。

server_nameは同一コンテキストに複数存在する場合、エラーとする。

名前で使える文字種は[このサイト](https://suu-g.hateblo.jp/entry/2019/09/19/232913)を見るに、RFCで定義されている。
`_`の扱いは曖昧だが、webservはこの文字をserver_nameに設定しても良いとする。
server_nameが`.`のみの場合、エラーとする。

同一のポートで複数の仮想サーバーがリクエストを待ち受けている場合、HTTPリクエストのHostヘッダを見てどの仮想サーバーの処理を振り分けるかを決定する。

Hostヘッダがどのサーバー名ともマッチしない場合はデフォルトサーバーに処理が振り分けられる。

Usage:

```
Syntax: server_name name;
Default: server_name "";
Context: server
```

Example:

```
server_name example.com;
```

### [listen]

サーバーがリクエストを受け付けるポート番号を設定する。

listenは同一コンテキストに複数存在する場合、エラーとする。

ポート番号の範囲は1から65535[参考](https://www.ibm.com/docs/ja/i/7.3?topic=ssw_ibm_i_73/cl/addtcpport.htm)。

アドレスは固定でlocalhost(127.0.0.1)が用いられる。

Usage:

```
Syntax: listen port
Default: listen 80;
Context: server
```

Example:

```
listen 8080;
```

### [error_page]

特定のエラーに対して表示するページを設定することができる。
uriには絶対パス、あるいはwebservプログラム実行ディレクトリからの相対パスを指定する。

設定されていない場合はデフォルトのエラーページが生成され、表示される。

同一のコンテキスト内で特定のエラーコードに対するエラーページが複数設定されている場合は最初に定義された値が使用される。

Usage:

```
Syntax: error_page code ... uri;
Default: —
Context: http, server, location
```

Example:

```
error_page 404 ./404.html;
error_page 500 501 505 ./50x.html;
```

### [client_max_body_size]

リクエストボディで許可する最大サイズを設定する（単位はバイト）。

client_max_body_sizeは同一コンテキストに複数存在する場合、エラーとする。

0以下の値は設定できない。

設定値を超えるリクエストが来た場合は413(Request Entity Too Large)を返す。

Usage:

```
Syntax: client_max_body_size size;
Default: client_max_body_size 1024;
Context: http, server, location
```

Example:

```
client_max_body_size 1024;
```

### [alias]

ロケーションで指定されたパスに対するエイリアスを設定できる。

pathの末尾は/で終了している必要がある。

aliasは同一コンテキストに複数存在する場合、エラーとする。

Usage:

```
Syntax: alias path;
Default: —
Context: location
```

Example:

```
location /kapouet {
    alias /tmp/www/;
}
```

url  is /tmp/www/pouic/toto/pouet.

#### 備考

rootには対応しない。rootとaliasの違い。

```
location /kapouet {
    alias /tmp/www/;
}

location /kapouet {
    root /tmp/www/;
}
```

`/kapouet/pouic/toto/pouet`のURLにアクセスした場合、それぞれ以下にアクセスされる。

- /tmp/www/pouic/toto/pouet
- /tmp/www/kapouet/pouic/toto/pouet

### [limit_except]

設定されたリクエストメソッド以外のリクエストを制限する。

limit_exceptは同一コンテキストに複数存在する場合、エラーとする。

メソッド名は大文字のみ設定できる。

許可されていないメソッドでのリクエストに対しては403(Forbidden)を返す。

Usage:

```
Syntax: limit_except method ...;
Default: —
Context: location
```

Example:

```
limit_except GET POST;
```

### [autoindex]

ディレクトリの一覧表示を行うかどうかを設定できる。

autoindexは同一コンテキストに複数存在する場合、エラーとする。

値は小文字のみ設定できる。

Usage:

```
Syntax: autoindex on | off;
Default: autoindex off;
Context: http, server, location
```

Example:

```
autoindex on;
```

### [index]

ディレクトリのデフォルトページを設定する。

indexは同一コンテキストに複数存在する場合、エラーとする。

Usage:

```
Syntax: index file;
Default: index index.html;
Context: http, server, location
```

Example:

```
index index.html;
```

### [return]

一時的なリダイレクト(302)を設定する。

returnは同一コンテキストに複数存在する場合、エラーとする。

URLは"http://"もしくは"https://"で始まる必要がある。

Usage:

```
Syntax: return URL;
Default: —
Context: server, location
```

Example:

```
return https://www.google.com;
```

### [server]

Usage:

```
Syntax: server { ... }
Default: —
Context: http
```

### [location]

リクエストのURIごとの設定を行う。

URIの末尾は/で終了している必要がある。

複数マッチした場合は最長一致のものにする。

Usage:

```
Syntax: location uri { ... }
Default: —
Context: server
```

### cgi_extension

cgi_extensionのディレクティブがある場合に、拡張子が値とマッチするファイルをCGIプログラムとしてを起動する。

Usage:

```
Syntax: cgi_extension extension ...;
Default: —
Context: location
```

Example:

```
cgi_extension py sh;
```

[alias]: https://nginx.org/en/docs/http/ngx_http_core_module.html#alias
[autoindex]: https://nginx.org/en/docs/http/ngx_http_autoindex_module.html#autoindex
[client_max_body_size]: https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
[error_page]: https://nginx.org/en/docs/http/ngx_http_core_module.html#error_page
[index]: https://nginx.org/en/docs/http/ngx_http_index_module.html#index
[limit_except]: https://nginx.org/en/docs/http/ngx_http_core_module.html#limit_except
[listen]: https://nginx.org/en/docs/http/ngx_http_core_module.html#listen
[location]: https://nginx.org/en/docs/http/ngx_http_core_module.html#location
[return]: https://nginx.org/en/docs/http/ngx_http_rewrite_module.html#return
[server]: https://nginx.org/en/docs/http/ngx_http_core_module.html#server
[server_name]: https://nginx.org/en/docs/http/ngx_http_core_module.html#server_name
