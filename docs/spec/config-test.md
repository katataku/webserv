一旦ドキュメントとして書いておくが、最終的にはテストケースとしてそれぞれについて用意することを想定している。

# テストケース

## 全般

- エラーの種類を以下に列挙する。

### コンテキストが違う場所にディレクティブがある

Example:

```
server {
	limit_except GET POST DELETE;
}
```

Error:
"limit_except" directive is not allowed here

### ディレクティブに必要な値の数が異なる

Example:

```
server {
	server_name;
}
```

Error:
invalid number of arguments in "server_name" directive

### ディレクティブに不適切な値が設定されている

Example:

```
server {
	server_name .;
}
```

Error:
"server_name" directive invalid value "."

### 同コンテキストにディレクティブが重複して存在する

- nginxの場合、以下の例だと構文的にokだがwebservはエラーとして扱う。

Example:

```
server {
	server_name hoge1.com;
	server_name hoge2.com;
}
```

Error:
"server_name" directive is duplicate

### 期待する文字と異なるものが存在する

Example:

```
server
```

Error:
unexpected end of file, expecting "{"

### 複数の値をとるディレクティブに重複した値が存在する

Example:

```
server {
	limit_except GET GET;
}
```

Error:
"limit_except" directive value is duplicate

## 具体的なケース

- 考慮すべきエラーケースを記載
- ディレクティブで共通するエラーケースは省略
  - 値が存在しない
  - 不適切な値が存在する
  - `{`、`}`、`;`がない


### server

Example:

```
server
```

Error:
unexpected end of file, expecting "{"

Example:

```
server hoge {
	listen 80;
}
```

Error:
invalid number of arguments in "server" directive

### location

Example:

```
server {
	location {
		alias /var/html/www;
	}
}
```

Error:
invalid number of arguments in "location" directive

```
server {
	location / /hoge {
		alias /var/html/www;
	}
}
```

Error:
invalid location modifier "/"
