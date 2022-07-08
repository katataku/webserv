# HTTP

## 設定ファイルの構成

### リクエスト

クライアントは、 HTTPリクエストをサーバへ送信する。 それは、順に次のものからなる、リクエストメッセージの形をとる：

1. [request-line](#request-line)

1. いくつかの[ヘッダー](#%E3%83%98%E3%83%83%E3%83%80%E3%83%BC)

1. ヘッダー節の終端を指示する空行

1. 最後に、ペイロード本体を包含しているメッセージ本体（もし在れば）

### レスポンス

サーバは、クライアントからのリクエストに対し、［1個以上のHTTPレスポンスメッセージ］を送信して、レスポンドする。 そのそれぞれは、順に、次のものからなる：

1. [status-line](#status-line)

1. 0個以上の [ヘッダー](#%E3%83%98%E3%83%83%E3%83%80%E3%83%BC)

1. ヘッダー節の終端を指示する空行

1. 最後に、ペイロードボディを包含しているメッセージボディ（もし在れば）

## request-line

Usage:

```http
Syntax: メソッド パス名 プロトコルバージョン
```

- [利用可能なメソッド一覧](#%E8%A8%AD%E5%AE%9A%E5%8F%AF%E8%83%BD%E3%81%AA%E3%83%A1%E3%82%BD%E3%83%83%E3%83%89%E4%B8%80%E8%A6%A7)に記載のメソッド。
- パス名は/aaa/bbb/ccc.htmlのような、スラッシュで始まるパス名や、http:// などで始まるURLを指定。
- プロトコルバージョンは1.1で固定。

Example:

```http
GET / HTTP/1.1
```

## status-line

Usage:

```http
Syntax: プロトコルバージョン ステータスコード 自由フレーズ 
```

- プロトコルバージョンは1.1で固定。
- [ステータスコード](#%E3%82%B9%E3%83%86%E3%83%BC%E3%82%BF%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89)に記載のステータスコード。
- [ステータスコード](#%E3%82%B9%E3%83%86%E3%83%BC%E3%82%BF%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89)に記載の自由フレーズ。

Example:

```http
HTTP/1.1 200 OK
```

## ステータスコード

以下のステータスコードのレスポンスに対応する。

| ステータスコード | 自由フレーズ                     | 説明                                                                                                                                                                     |
| -------- | -------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 200      | OK                         | GET/POST/DELETEの処理が正常に完了した場合に使用する。                                                                                                                                     |
| 301      | Moved Permanently          | configにリダイレクト設定がされていた場合に使用する。                                                                                                                                          |
| 400      | Bad Request                | リクエスト内容のパースに失敗した場合、あるいは不正な内容が含まれていた場合全般に使用する。                                                                                                                          |
| 404      | Not Found                  | リクエスト内容は正常だが、リソースが見つからない場合に使用する。                                                                                                                                       |
| 405      | Method Not Allowed         | [利用可能なメソッド一覧](#%E8%A8%AD%E5%AE%9A%E5%8F%AF%E8%83%BD%E3%81%AA%E3%83%A1%E3%82%BD%E3%83%83%E3%83%89%E4%B8%80%E8%A6%A7)に未記載のメソッドの場合に使用する。レスポンスには[Allow](#allow)ヘッダーを含めること。 |
| 408      | Request Timeout            | 処理時間が15秒を超えた場合に使用する。15秒という値は決め。                                                                                                                                        |
| 413      | Payload Too Large          | 要求されたpayloadのサイズが1GBを超える場合に使用する。1GBという値は決め。                                                                                                                            |
| 414      | URI Too Long               | URI文字長が1024文字以上の場合に使用する。 1024という値は決め。                                                                                                                                  |
| 500      | Internal Server Error      | CGIプログラムが異常終了した際に使用する。                                                                                                                                                 |
| 505      | HTTP Version Not Supported | このHTTPバージョンが1.1以外の場合に使用する。                                                                                                                                             |

## メソッド

### 設定可能なメソッド一覧

以下のメソッドを設定可能とする。

- GET
- POST
- DELETE

### HOSTメソッドについて

RFCでは必須と決められているが、今回は対応しない。

[RFC7231](https://triple-underscore.github.io/RFC7231-ja.html#section-4)

```text
すべての一般用サーバは、 GET および HEAD をサポートしなければならない。 他のメソッドのサポートは、すべて任意選択である。
```

## ヘッダー

### 設定可能なヘッダー一覧

記載範囲外のヘッダーがリクエストに含まれていた場合は何も処理をしない。（エラーとせずに処理を継続する。）

<!-- ◯(必須) | ◯(任意) | ✖︎ -->

| ヘッダー名                                   | リクエスト | レスポンス |
| --------------------------------------- | ----- | ----- |
| [Host](#host)                           | ◯(必須) | ✖︎    |
| [Connection](#connection)               | ✖︎    | ◯(必須) |
| [Transfer-Encoding](#Transfer-Encoding) | ◯(任意) | ◯(任意) |
| [Content-Length](#content-length)       | ◯(任意) | ◯(任意) |
| [Allow](#allow)                         | ✖︎    | ◯(任意) |

### Host

HTTP/1.1で唯一の必須ヘッダーです。ブラウザからサーバに対して、サーバ名を送信します。サーバが名前ベースの仮想ホストをサポートしている場合、この名前を手がかりにどのサーバとして振舞うか決定されます。たとえば、<http://aaa.sample.dom/> と <http://bbb.sample.dom/> は実は同じサーバ（IPアドレス：61.206.47.206）ですが、Hostヘッダーでホスト名を指定することにより、仮想的に2つのサーバとして振舞うことが可能になります。

Example:

```http
Host: aaa.sample.dom
```

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

## 参考

[HTTP入門](https://www.tohoho-web.com/ex/http.htm)
