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

ヘッダー名 | リクエスト | レスポンス
 -- | -- | --
[Accept](#accept) | ◯(任意) | ✖︎
[Date](#Date) | ◯(必須) | ◯(必須)
[Content-Length](#content-length) | ◯(必須) | ◯(必須)

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

## 参考

[HTTP入門](<https://www.tohoho-web.com/ex/http.htm>)
