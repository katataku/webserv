# HTTP

## 設定ファイルの構成

### リクエスト

クライアントは、 HTTPリクエストをサーバへ送信する。 それは、順に次のものからなる、リクエストメッセージの形をとる：

1. [request-line](#request-line)

1. 1個以上の[ヘッダー](#%E3%83%98%E3%83%83%E3%83%80%E3%83%BC)

1. ヘッダー節の終端を指示する空行。(\\r\\n).

1. 最後に、ペイロード本体を包含しているメッセージ本体（もし在れば）

### レスポンス

サーバは、クライアントからのリクエストに対し、［1個以上のHTTPレスポンスメッセージ］を送信して、レスポンドする。 そのそれぞれは、順に、次のものからなる：

1. [status-line](#status-line)

1. 1個以上の [ヘッダー](#%E3%83%98%E3%83%83%E3%83%80%E3%83%BC)

1. ヘッダー節の終端を指示する空行。(\\r\\n).

1. 最後に、ペイロードボディを包含しているメッセージボディ（もし在れば）

## request-line

Usage:

```http
Syntax: メソッド リクエストターゲット プロトコルバージョン
```

- [利用可能なメソッド一覧](#%E8%A8%AD%E5%AE%9A%E5%8F%AF%E8%83%BD%E3%81%AA%E3%83%A1%E3%82%BD%E3%83%83%E3%83%89%E4%B8%80%E8%A6%A7)に記載のメソッド。
- リクエストターゲットは[origin-form](#origin-form)形式。
- プロトコルバージョンは`HTTP/1.1`で固定。

Example:

```http
GET / HTTP/1.1
```

### origin-form

リクエストターゲットは[origin-form](https://triple-underscore.github.io/RFC7230-ja.html#section-5.3.1)形式のみを許容する。

Usage:

```http
Syntax: absolute-path [ "?" query ]
```

- absolute-pathには`/aaa/bbb/ccc.html`のような、スラッシュで始まるパス名を指定する。

  - absolute-pathが空の場合、今回のサーバはリクエスト不正として400エラーレスポンスを返すものとする。
    - RFCでは、クライアントは「pathが空の場合`"/"`を送信しなければならない（MUST）」と規定されている。
  - absolute-pathはドットセグメント( "." と ".." )を許容する
    - `"/aaa/./bbb"`は`"/aaa/bbb"`とする。
    - `"/aaa/ccc/../bbb"`は`"/aaa/bbb"`とする。
    - `"/../aaa/bbb"`は400エラーとする。
    - ドットセグメントの削除処理詳細は[こちら](https://triple-underscore.github.io/rfc-others/RFC3986-ja.html#section-5.2.4)に規定されている。

- queryは、「?」+「変数名」+「=」+「変数の値」というのが、基本構造になっている。パラメーターが複数ある場合は「&」でつないでいく。

Example:

`http://www.example.org/where?q=now`というURLへのアクセスの例

```http
GET /where?q=now HTTP/1.1
Host: www.example.org
```

## status-line

Usage:

```http
Syntax: プロトコルバージョン ステータスコード 自由フレーズ 
```

- プロトコルバージョンは`HTTP/1.1`で固定。
- [ステータスコード](#%E3%82%B9%E3%83%86%E3%83%BC%E3%82%BF%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89)に記載のステータスコード。
- [ステータスコード](#%E3%82%B9%E3%83%86%E3%83%BC%E3%82%BF%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89)に記載の自由フレーズ。

Example:

```http
HTTP/1.1 200 OK
```

## ステータスコード

以下のステータスコードのレスポンスに対応する。

| ステータスコード | 自由フレーズ                     | 説明                                                                                                                                                                                                               |
| -------- | -------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 200      | OK                         | GET/DELETEの処理が正常に完了した場合に使用する。                                                                                                                                                                                    |
| 201      | Created                    | POSTの処理が正常に完了した場合に使用する。                                                                                                                                                                                          |
| 302      | Moved Temporarily          | configにリダイレクト設定がされていた場合に使用する。[Location](#location)  ヘッダーを必含めること。                                                                                                                                                 |
| 400      | Bad Request                | リクエスト内容のパースに失敗した場合、あるいは不正な内容が含まれていた場合全般に使用する。                                                                                                                                                                    |
| 403      | Forbidden                  | リクエスト内容は正常だが、リソースに対する権限がない場合に使用する。                                                                                                                                                                               |
| 404      | Not Found                  | リクエスト内容は正常だが、リソースが見つからない場合に使用する。                                                                                                                                                                                 |
| 405      | Method Not Allowed         | [利用可能なメソッド一覧](#%E8%A8%AD%E5%AE%9A%E5%8F%AF%E8%83%BD%E3%81%AA%E3%83%A1%E3%82%BD%E3%83%83%E3%83%89%E4%B8%80%E8%A6%A7)に記載されているメソッドがターゲットリソースでサポートされていない場合に使用する。レスポンスには[Allow](#allow)ヘッダーを含めること。                    |
| 413      | Payload Too Large          | 要求されたpayloadのサイズがconfigで設定したサイズを超える場合に使用する。                                                                                                                                                                      |
| 414      | URI Too Long               | URI文字長が1024文字以上の場合に使用する。 1024という値は決め。                                                                                                                                                                            |
| 500      | Internal Server Error      | CGIプログラムが異常終了した際に使用する。また、サーバ処理にてシステムコールのエラーが発生した際に使用する。                                                                                                                                                          |
| 501      | Not Implemented            | [Transfer-Encoding](#Transfer-Encoding)において、未対応の値が含まれている場合に使用する。 [利用可能なメソッド一覧](#%E8%A8%AD%E5%AE%9A%E5%8F%AF%E8%83%BD%E3%81%AA%E3%83%A1%E3%82%BD%E3%83%83%E3%83%89%E4%B8%80%E8%A6%A7)に記載されていないメソッドが含まれている場合に使用する。 |
| 505      | HTTP Version Not Supported | リクエストに含まれる、Request-lineのHTTPバージョンが1.1以外の場合に使用する。                                                                                                                                                                 |

## メソッド

### 設定可能なメソッド一覧

以下のメソッドを設定可能とする。

- GET
- HEAD
- POST
- DELETE

### POSTメソッドについて

- リソースが存在する場合：新たにファイルを作成する。
- リソースが存在しない場合：既存ファイルを削除して、新たにファイルを作成する。

## ヘッダー

### 設定可能なヘッダー一覧

記載範囲外のヘッダーがリクエストに含まれていた場合は何も処理をしない。（エラーとせずに処理を継続する。）

ヘッダ名は大文字小文字を区別しない。

<!-- ◯(必須) | ◯(任意) | ✖︎ -->

| ヘッダー名                                   | リクエスト | レスポンス |
| --------------------------------------- | ----- | ----- |
| [Host](#host)                           | ◯(必須) | ✖︎    |
| [Connection](#connection)               | ✖︎    | ◯(必須) |
| [Content-Length](#content-length)       | ◯(任意) | ◯(必須) |
| [Content-Type](#content-type)           | ◯(任意) | ✖︎    |
| [Transfer-Encoding](#Transfer-Encoding) | ◯(任意) | ✖︎    |
| [Allow](#allow)                         | ✖︎    | ◯(任意) |
| [Location](#location)                   | ✖︎    | ◯(任意) |

### Host

HTTP/1.1で唯一の必須ヘッダーです。ブラウザからサーバに対して、サーバ名を送信します。サーバが名前ベースの仮想ホストをサポートしている場合、この名前を手がかりにどのサーバとして振舞うか決定されます。たとえば、<http://aaa.sample.dom/> と <http://bbb.sample.dom/> は実は同じサーバ（IPアドレス：61.206.47.206）ですが、Hostヘッダーでホスト名を指定することにより、仮想的に2つのサーバとして振舞うことが可能になります。

Example:

```http
Host: aaa.sample.dom
```

サーバは、次のいずれかに該当するどのリクエストメッセージに対しても、ステータスコード400 (Bad Request) でレスポンドする。

- HTTP/1.1メッセージであって、Hostヘッダーを欠くもの。
- 複数個のHostヘッダーを包含するもの。
- 包含するHostヘッダーのヘッダー値が妥当でないもの。

[RFC](https://triple-underscore.github.io/RFC7230-ja.html#section-5.4)

### Connection

すべてのレスポンスに`Connection: close`を含めてレスポンドする。

Example:

```http
Connection: close
```

今回のサーバは持続的な接続をサポートしない方針とする。したがって、RFCの定めにより、close値を含めたレスポンスを返すものとする。

[RFC](https://triple-underscore.github.io/RFC7230-ja.html#p.Connection)

### Content-Length

コンテンツ（＝メッセージボディ）の長さをバイト単位で示します。ヘッダーとメッセージボディの間の改行のバイト数は除きます。

Example:

```http
Content-Length: 4891
```

[RFC](https://triple-underscore.github.io/RFC7230-ja.html#section-3.3.2)

### Content-Type

コンテンツ（＝メッセージボディ）の種類を示します。
HTTPリクエストにContent-Typeが設定されていて、かつCGIを起動する場合、CGIリクエストのCONTENT_TYPEに同じ値を設定する。
HTTPレスポンスには、CGIレスポンスにContent-Typeが設定されていてもそれを破棄し、設定しない。

Example:

```http
Content-Type: text/html
```

[RFC](https://triple-underscore.github.io/RFC7231-ja.html#section-3.1.1.5)

### Transfer-Encoding

転送に使用されるエンコード形式を示します。

以下の転送エンコード形式に対応する。

- chunked

未対応のエンコード形式のデータを受信したときには、501 (Not Implemented) でレスポンドする。

送信者は「Transfer-Encodingヘッダーを包含するどのメッセージにも、 Content-Lengthヘッダーを送信してはならない。」と規定されている。そのため、Transfer-EncodingとContent-Lengthを同時に受信した際には、400(Bad Request)でレスポンドする。

Example:

```http
Transfer-Encoding: chunked
```

[RFC](https://triple-underscore.github.io/RFC7230-ja.html#section-3.3.1)

### Allow

オリジンサーバは、 405 (Method Not Allowed) レスポンス内には、Allowヘッダーを生成しなければならない。

Example:

```http
Allow: GET, POST, DELETE
```

[RFC7231](https://triple-underscore.github.io/RFC7231-ja.html#header.allow)

### Location

エンティティの場所が移動した場合など、ブラウザが要求したURLとは別のURLへジャンプさせたい場合に使用します。302リダイレクトをレスポンドする際には、必ず付与するものとする。

Example:

```http
Location: http://www.yyy.zzz/aaa.html
```

## 参考

[HTTP入門](https://www.tohoho-web.com/ex/http.htm)
