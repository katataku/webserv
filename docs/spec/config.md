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

#### 相談
正規表現などには対応しない予定

#### 確認
これは何に使われる？複数設定することも可能。


### [listen]
サーバーがリクエストを受け付けるホスト名とポート番号を設定する。

Usage: 
```
Syntax: listen host:port
Default: Required
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

[server_name]: https://nginx.org/en/docs/http/ngx_http_core_module.html#server_name
[listen]: https://nginx.org/en/docs/http/ngx_http_core_module.html#listen

