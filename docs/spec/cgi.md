# CGIの仕様

## CGIとは

> Common Gateway Interface（CGI）\[22\]は、HTTP \[1\]、\[4\]サーバーとCGIスクリプトがクライアント要求に応答する責任を共有できます。

- CGIプログラムは、次のメタ変数（環境変数）と同時に起動される。SCRIPT_NAME、PATH_INFO、QUERY_STRING

```
script-URI = <scheme> "://" <server-name> ":" <server-port> <script-path> <extra-path> "?" <query-string>
```

## CGIへのリクエストまとめ

### pdf+testerに準拠した仕様まとめ

|メタ変数名|説明|期待される挙動|値(BNF表記)|
|-|-|-|-|
|CONTENT_LENGTH|コンテンツのバイト長。POSTメソッドが来た時必要。|HTTPリクエストにbodyがあればセットし、なかったらセットしない。|CONTENT_LENGTH = "" | 1\*digit|
|CONTENT_TYPE|コンテンツの種類。|HTTPリクエストにbodyがあればセットされる。なかったら空文字列をセット。<br>HTTPリクエストヘッダーにtypeが設定されていたら、このメタ変数を設定する。|CONTENT_TYPE = "" | media-type<br>|
|PATH_INFO|extra pathのこと|非US-ASCII文字が含まれている場合の処理はシステム依存。<br>nginxではURIにextra pathがないときは空文字列が入ってそう。<br>|PATH_INFO = "" | ( "/" path )|
|REQUEST_METHOD|HTTPリクエストで指定されたメソッド|大文字小文字を区別|REQUEST_METHOD   = method|
|SERVER_PROTOCOL|サーバー・CGI間のプロトコル？|HTTP/1.1に固定で良い|SERVER_PROTOCOL   = HTTP-Version|

## CGIからのレスポンスまとめ

### pdf+testerに準拠した仕様まとめ

```
generic-response = 1*header-field NL [ response-body ]
```

|フィールド値名|説明|期待される挙動|値(BNF表記)|
|-|-|-|-|
|Status|ステータスコード|サーバーはきたものをそのまま返すだけ|Status         = "Status:" status-code SP reason-phrase NL<br>status-code    = "200" | "302" | "400" | "501" | extension-code|
|Content-Type|bodyのコンテンツの種類|bodyが返されるとき、必ずセットされている。<br>種類がtext/htmlかつセットされていなければISO-8859-1、それ以外のtextはUS-ASCIIとして処理|Content-Type = "Content-Type:" media-type NL|
|body|コンテンツボディ|ヘッダーの後、改行がありそれ以降がbody。<br>標準出力に出力されるのでサーバーはそれを読む|-|

## CGI周りの期待される挙動まとめ

### pdf+testerに準拠した仕様まとめ

- 拡張子に基づいて？CGIを実行（例 phpならhoge.php）
- CGIは絶対パスで実行。
  - > Because you won’t call the CGI directly, use the full path as PATH_INFO
- （多分）chunkedなHTTPリクエストでも、CGIへのリクエストは一度に行い、CGIリクエストボディにはEOF？をつける
  - > Just remember that, for chunked request, your server needs to unchunked it and the CGI will expect EOF as end of the body
- CGIからのレスポンスにCONTENT_LENGTHがなかったら、EOFをつける？
  - > Same things for the output of the CGI. If no content_length is returned from the CGI, EOF will mark the end of the returned data.
- （多分）argv\[0\]に（絶対パス）CGIプログラム名を入れる
  - > Your program should call the CGI with the file requested as first argument
- 相対パスでCGIを起動できるように
- POSTでCGIにbodyを送るときは標準入力に
- CGIからのレスポンスは標準出力に書き込む
- CGIレスポンスヘッダーからHTTPレスポンスヘッダーに変換する必要がある
  - 改行とか
    - US-ASCII LFはHTTPだと、US-ASCII CR+LF
- CGIレスポンスボディは全て読む
  - 多分、EOFがくるまでread
- CGIのタイムアウト処理は多分やった方が良い
