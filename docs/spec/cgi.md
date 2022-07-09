# CGIの仕様

## CGIとは

> Common Gateway Interface（CGI）[22]は、HTTP [1]、[4]サーバーとCGIスクリプトがクライアント要求に応答する責任を共有できます。

## 前提
- 　CGIプログラムは、次のメタ変数（環境変数と読み替えて良いはず）と同時に起動される。SCRIPT_NAME、PATH_INFO、QUERY_STRING

> From the meta-variables thus generated, a URI, the 'Script-URI', can be constructed. This MUST have the property that if the client had accessed this URI instead, then the script would have been executed with the same values for the SCRIPT_NAME, PATH_INFO and QUERY_STRING meta-variables. 

- Script-URIは次のように定義される。SCRIPT_NAMEはscript-path、PATH_INFOはextra-path。

```
script-URI = <scheme> "://" <server-name> ":" <server-port> <script-path> <extra-path> "?" <query-string>
```

> The SCRIPT_NAME and PATH_INFO values, URL-encoded with ";", "=" and "?" reserved, give <script-path> and <extra-path>.

## CGIへのリクエスト

- 以下はMUSTって書いてないけどそんな雰囲気を感じる

```
meta-variable-name = "AUTH_TYPE" | "CONTENT_LENGTH" |
                      "CONTENT_TYPE" | "GATEWAY_INTERFACE" |
                      "PATH_INFO" | "PATH_TRANSLATED" |
                      "QUERY_STRING" | "REMOTE_ADDR" |
                      "REMOTE_HOST" | "REMOTE_IDENT" |
                      "REMOTE_USER" | "REQUEST_METHOD" |
                      "SCRIPT_NAME" | "SERVER_NAME" |
                      "SERVER_PORT" | "SERVER_PROTOCOL" |
                      "SERVER_SOFTWARE" | scheme |
                      protocol-var-name | extension-var-name
protocol-var-name  = ( protocol | scheme ) "_" var-name
scheme             = alpha *( alpha | digit | "+" | "-" | "." )
var-name           = token
extension-var-name = token
```

- `http://host/script`や`http://host/script?`はQUERY_STRINGがNULLになるべき（空文字なのか定義しないのか）

- 対応していないメタ変数はNULL

- 大文字と小文字は区別して考える(MUST)

### CONTENT_LENGTH

- CONTENT_LENGTHがなかったらNULLかセットしない（つまりNULLは空文字）

- HTTPリクエストにbodyがあればセットし、なかったらセットしない(MUST)。

```
CONTENT_LENGTH = "" | 1*digit
```

### CONTENT_TYPE

- HTTPリクエストにbodyがあればセットされる
- type、subtype、attributeは大文字と小文字を区別しない
- parameterは大文字と小文字を区別する
- もし、このメタ変数がセットされていなかったら、CGIプログラムはコンテンツの種類を推測し、できなかったらapplication/octet-streamとかいうやつから選ぶ（MAYなのでこれはエラーにしとけば良い気がする）。

```
CONTENT_TYPE = "" | media-type
media-type   = type "/" subtype *( ";" parameter )
type         = token
subtype      = token
parameter    = attribute "=" value
attribute    = token
value        = token | quoted-string
```

- media-typeのデフォルトのcharset(文字エンコード方式っぽい)値は以下のように決められている。
  - いくつかのmedia-typeはシステムにより定義されている（MAY）
  - `text`はISO-8859-1（ASCIIの拡張みたいなっぽい。8bitフルで使ってそう）
  - デフォ値は仕様で決まっているよ
  - （多分上記以外のケースは)デフォ値はUS-ASCII
- HTTPリクエストヘッダーにtypeが設定されていたら、このメタ変数を設定する（MUST）。
  - 設定されていなかったら適切な値を設定しようとする（MAY）

### GATEWAY_INTERFACE
- これを設定する（MUST）
- CGIのバージョンは1.1で良い（RFC3875が1.1）

```
GATEWAY_INTERFACE = "CGI"/1.1"
```

### PATH_INFO

- CGIスクリプト以降のパスのこと
  - ex. `http://hoge.com/cgi-bin/hoge/somepath` -> `/somepath`
- URLエンコーディング(URIで使えない文字を%を使って表現)されていない
- `/`は単一のvoid path segment？
- 非US-ASCII文字が含まれている場合はその処理はシステム依存（エラーにする？

```
PATH_INFO = "" | ( "/" path )
path      = lsegment *( "/" lsegment )
lsegment  = *lchar
lchar     = <any TEXT or CTL except "/">
```

### PATH_TRANSLATED

- PATH_INFOを解釈し、ローカルのリソースにアクセスできるようなパスの形にしたもの
  - ex
    - `http://somehost.com/cgi-bin/somescript/this%2eis%2epath%3binfo`
    - PATH_INFOは`/this.is.the.path;info`
    - 内部URIみたいなのが生成されるらしい。今の例だと、`http://somehost.com/this.is.the.path%3binfo`
    - これを変換し、`/usr/local/www/htdocs/this.is.the.path;info`
- 変換した後のパスが有効なパスかどうかはこいつの責務ではない

```
PATH_TRANSLATED = *<any character>
```

- 何を言ってるんでしょうか
- 多分、大文字小文字を区別しないなら、extra pathも区別しない、みたいな
  - つまり、サーバー内のパスをどう区別するかに依存しますよということのはず

> The value is derived in this way irrespective of whether it maps to a valid repository location. The server MUST preserve the case of the extra-path segment unless the underlying repository supports case-insensitive names. If the repository is only case-aware, case-preserving, or case-blind with regard to document names, the server is not required to preserve the case of the original segment through the translation.

- 翻訳アルゴリズムは実装依存
- PATH_INFOがNULLの場合、PATH_TRANSLATEDはNULLかセットされていない

### QUERY_STRING
- URLエンコードされたクエリ文字列が入っている（%とかはいった文字列）
- クエリ文字列をパースした結果はコンテキストによって変化する（エンコーディング方法）
  - HTMLのformだとapplication/x-www-form-urlencodedを使い、`+`、`&`、`=`は予約語に
  - 非US-ASCII文字はISO 8859-1でエンコーディングされる
- メタ変数に設定(MUST)
  - 設定されていなかったら空文字列として処理

```
QUERY_STRING = query-string
query-string = *uric
uric         = reserved | unreserved | escaped
```

### REMOTE_ADDR
- クライアントのネットワークアドレス
- メタ変数に設定（MUST）
- ipv6についてはRFC 4291

```
REMOTE_ADDR  = hostnumber
hostnumber   = ipv4-address | ipv6-address
ipv4-address = 1*3digit "." 1*3digit "." 1*3digit "." 1*3digit
ipv6-address = hexpart [ ":" ipv4-address ]
hexpart      = hexseq | ( [ hexseq ] "::" [ hexseq ] )
hexseq       = 1*4hex *( ":" 1*4hex )
```

### REQUEST_METHOD
- MUST
- 大文字小文字を区別

```
REQUEST_METHOD   = method
method           = "GET" | "POST" | "HEAD" | extension-method
extension-method = "PUT" | "DELETE" | token
```

### SCRIPT_NAME
- MUST
- CGIスクリプトを識別できる名前（URLエンコーディングされていない）
- 先頭の`/`はパスの一部でない
- パスがNULLだったら任意（の値）？。ただメタ変数のセットはMUST

```
SCRIPT_NAME = "" | ( "/" path )
```

### SERVER_NAME
- サーバーのホスト名
- 大文字小文字を区別しないホスト名、もしくはネットワークアドレスがはいる
- 複数ある場合は正しいホスト名をセットする

```
SERVER_NAME = server-name server-name = hostname | ipv4-address | ( "[" ipv6-address "]" )
```

### SERVER_PORT
- MUST
- サーバーのポート

```
SERVER_PORT = server-port server-port = 1*digit
```

### SERVER_PROTOCOL
- MUST
- CGIリクエストに使われるアプリケーションのプロトコル？
- サーバー・クライアント間で使われているプロトコルバージョンと一致しないかも(MAYなのでHTTP/1.1に固定)

```
SERVER_PROTOCOL   = HTTP-Version | "INCLUDED" | extension-version
HTTP-Version      = "HTTP" "/" 1*digit "." 1*digit
extension-version = protocol [ "/" 1*digit "." 1*digit ]
protocol          = token
```

### SERVER_SOFTWARE
- サーバーソフトウェアの名前とバージョン

```
SERVER_SOFTWARE = 1*( product | comment )
product         = token [ "/" product-version ]
product-version = token
comment         = "(" *( ctext | comment ) ")"
ctext           = <any TEXT excluding "(" and ")">
```

## リクエストのbody
- POSTとかでファイルを扱うとき、特に定義されていない限り、標準入力かファイルディスクリプタから読む

```
Request-Data   = [ request-body ] [ extension-data ]
request-body   = <CONTENT_LENGTH>OCTET
extension-data = *OCTET
```

- CONTENT_LENGTHがNULLではない場合、このbodyがあるはず
- スクリプトはCONTENT_LENGTH以上、読み込んではいけない
- サポートしていないtransfer-codingsで送信されたら、CONTENT_LENGTHを再計算しなければならない
  - 再計算の結果、サイズがデカすぎるなど物理的に不可能だったら、クライアントからのリクエストを拒否するべき
- bodyからcontent-codingsを削除することもできる（MAY）？
  - これが一番楽？

## リクエストメソッド
　
### GET
- 特になし

### POST
- 処理前にCONTENT_LENGTHの値をチェック（MUST）

### HEAD
- リクエストにbodyを含めてはいけない
- もし含まれていたらサーバー側で無視する

## スクリプトコマンドライン？
> Some systems support a method for supplying an array of strings to the CGI script. 

- 実装しなくて良いんじゃない

# CGIレスポンス

## 処理
- スクリプトは常に空でないレスポンスを返す(MUST)
- 返す方法はシステムによる。定義されていなかったら、標準出力に出力
- リクエストの処理中とかレスポンスを返すときREQUEST_METHODのチェック(MUST)？
- タイムアウト処理(MAYだけどした方が良いんじゃない)

## Response Types
- レスポンスはヘッダーとボディがあり、空行で分かれている
- ヘッダーは一つ以上のフィールドを持っている
- ボディはNULLかも

```
generic-response = 1*header-field NL [ response-body ]
```

- ドキュメントレスポンス、ローカルリダイレクトレスポンス、クライアントリダイレクトレスポンスのどれかひとつを返す(MUST)

```
CGI-Response = document-response | local-redir-response | client-redir-response | client-redirdoc-response
```

### Document Response
> The CGI script can return a document to the user in a document response, with an optional error code indicating the success status of the response.

- canということは返さなくて良いの
  - よくわからないけど、基本これだと思う

```
document-response = Content-Type [ Status ] *other-field NL response-body
```

## Response Header Fields
> The response header fields are either CGI or extension header fields to be interpreted by the server, or protocol-specific header fields to be included in the response returned to the client. 

- ただクライアントにそのまま返せるやつを返すわけではないんか

```
header-field    = CGI-field | other-field
CGI-field       = Content-Type | Location | Status
other-field     = protocol-field | extension-field
protocol-field  = generic-field
extension-field = generic-field
generic-field   = field-name ":" [ field-value ] NL
field-name      = token
field-value     = *( field-content | LWSP )
field-content   = *( token | separator | quoted-string )
```

- field-nameは大文字小文字を区別しない
- NULL値は実質送られていないに等しい
- 各フィールドは一行で
- 空白以下の場所で許可
  - `:`とfield-valueの間(逆はだめ)
  - field-value内の複数tokenの間

### Content-Type
- Internet Media Type（拡張子的な）

```
Content-Type = "Content-Type:" media-type NL
```

- bodyが返された時は、Content-Typeがセットされるべき(MUST)
- テキストに関しては、charsetはHTTPなら　ISO-8859-1、そうでなければUS-ASCIIとして解釈されるっぽい？
- なのでスクリプトはcharsetも指定するべき(SHOULD)

### Location
- なんだこれ

```
Location        = local-Location | client-Location
client-Location = "Location:" fragment-URI NL
local-Location  = "Location:" local-pathquery NL
fragment-URI    = absoluteURI [ "#" fragment ]
fragment        = *uric
local-pathquery = abs-path [ "?" query-string ]
abs-path        = "/" path-segments
path-segments   = segment *( "/" segment )
segment         = *pchar
pchar           = unreserved | escaped | extra
extra           = ":" | "@" | "&" | "=" | "+" | "$" | ","
```

### Status
- 3桁の数字とそれに基づいたフレーズ

```
Status         = "Status:" status-code SP reason-phrase NL
status-code    = "200" | "302" | "400" | "501" | extension-code
extension-code = 3digit
reason-phrase  = *TEXT
```

- ex
  - `200 OK`
    - 成功した場合。基本これ
  - `302 Found`
    - > Status code 302 'Found' is used with a Location header field and response message-body.
  - `400 Bad Request`
    - リクエストのフォーマットがおかしい。たとえばCONTENT_TYPEがないとか
  - `501 Not Implemented`
    - 知らないREQUEST_METHODだ...

### Protocol-Specific Header Fields
- サーバーはCGIからのヘッダーをHTTPヘッダーに変換する必要がある
  - ex
    - 改行とか
      - US-ASCII LFはHTTPだと、US-ASCII CR+LF
- > The script MUST NOT return any header fields that relate to client-side communication issues and could affect the server's ability to send the response to the client. 

### Response Message-Body
- サーバーはスクリプトからのbodyを全て読まないといけない

```
response-body = *OCTET4
```

### System Specifications

## UNIX
- Cならgetenv関数で環境変数からメタ変数の値をとる
- コマンドラインはargcとargvから取れるけど...
- メタ変数とヘッダーフィールド、CHARはUS-ASCII charsetが使われる
- TEXTはISO-8859-1
- PATH_TRANSLATEDは一文字8bit（多分）
- 改行はLF。サーバーはCR LFも改行として受け入れるべき（なので受け入れなくて良い）

# 実装について

## サーバー側
- `.`、`..`の挙動に注意
- パスの長さとか

## Recommendations for Scripts
- >If the script does not intend processing the PATH_INFO data, then it should reject the request with 404 Not Found if PATH_INFO is not NULL.
- どういう状況だこれ

# メモ
- RFCで定められている中でもMUSTで対応すべきもの、RECOMMENDEDなものなどいくつかレベルがある。MUSTとREQUIREDなものだけで良いんじゃないかと思う
- 全体的にCGIプログラムはスクリプト前提な気がする
- Basic認証しているならMUST、みたいな項目は省略
- POST+CGIのとき、ファイルを読み込むのはサーバーなのか　CGIなのか
- DELETEはHTTP/1.1から追加されたっぽい。CGI/1.0？ではMAYだから対応するかどうか
