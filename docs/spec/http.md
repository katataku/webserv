# HTTP

## 設定ファイルの構成

## 項目

### リクエスト行

Usage:

``` http
Syntax: メソッド パス名 HTTP/バージョン
```

- 利用可能メソッドは後述。
- パス名は/aaa/bbb/ccc.htmlのような、スラッシュで始まるパス名や、http:// などで始まるURLを指定。
- バージョンは1.1で固定。

Example:

``` http
GET / HTTP/1.1
```

## メソッド

### 設定可能なメソッド一覧

- GET
- HEAD
- POST
- DELETE
- PUT？

## ヘッダー

### 設定可能なヘッダー一覧

記載範囲外のヘッダーがリクエストに含まれていた場合は何も処理をしない。（エラーとせずに処理を継続する。）

ヘッダー名 | リクエスト | レスポンス
 -- | -- | --
[Accept](#accept) | ◯(任意) | ✖︎
[Date](#Date) | ◯(必須) | ◯(必須)
[Content-Encoding](#content-encoding) | TBD | TBD
[Content-Length](#content-length) | TBD | TBD
[Content-type](#content-type) | TBD | TBD
[Server](#Server)| TBD | TBD
[Host](#host)| TBD | TBD

- Expect

### Accept

ブラウザが受信可能なデータ形式（MIMEタイプ）をサーバに伝えます。アスタリスク（*）は「すべて」を意味します。下記は、ブラウザがGIFやJPEG、その他どんな形式のデータでも受信可能であることを示します。（→ Content-Type）

Example:

```http
Accept: image/gif, image/jpeg, */*
```

### Content-Length

コンテンツ（＝メッセージボディ）の長さをバイト単位で示します。ヘッダーとメッセージボディの間の改行のバイト数は除きます。

Example:

```http
Content-Length: 4891
```

### Content-Encoding

コンテンツのエンコード方式を示します。下記は、コンテンツが gzip 形式で圧縮されていることを示します。（→ Accept-Encoding）

Example:

```http
Content-Encoding: gzip
```

### Host

HTTP/1.1で唯一の必須ヘッダーです。ブラウザからサーバに対して、サーバ名を送信します。サーバが名前ベースの仮想ホストをサポートしている場合、この名前を手がかりにどのサーバとして振舞うか決定されます。たとえば、<http://aaa.sample.dom/> と <http://bbb.sample.dom/> は実は同じサーバ（IPアドレス：61.206.47.206）ですが、Hostヘッダーでホスト名を指定することにより、仮想的に2つのサーバとして振舞うことが可能になります。

Example:

```http
Host: aaa.sample.dom
```

## 参考

[HTTP入門](<https://www.tohoho-web.com/ex/http.htm>)
