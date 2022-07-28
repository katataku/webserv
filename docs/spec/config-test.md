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
server name "." is invalid

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
duplicate "server_name" in server context

## 具体的なケース

- 考慮すべきエラーケースを記載
- ディレクティブで共通するエラーケースは省略。例えば値がない、;がないなど。

### server_name

- server_name(ホスト名)は[このサイト](https://suu-g.hateblo.jp/entry/2019/09/19/232913)を見るに、RFCで定義されている。
- `_`の扱いは曖昧だが、webservはこの文字をserver_nameに設定しても良いとする。
- ホスト名のBNF

```
<official hostname> ::= <hname>
<hname> ::= <name>*["."<name>]
<name>  ::= <let-or-digit>[*[<let-or-digit-or-hyphen>]<let-or-digit>]
```

Example:

```
server {
	server_name .;
}
```

Error:
server name "." is invalid

### listen

Example:

```
server {
	listen 8080;
	listen 8080;
}
```

"listen" directive is allowed only one.

Example:

```
server {
	listen 65536;
}

server {
	listen 0;
}
```

invalid port in "65536" of the "listen" directive

<!-- ### error_page -->

### client_max_body_size

Example:

```
server {
	client_max_body_size a;
}
```

Error:
"client_max_body_size" directive invalid value

<!-- ### alias -->

### limit_except

Example:

```
server {
	limit_except HOGE;
}
```

Error:
invalid method "HOGE"

Example:

```
server {
	limit_except;
}
```

Error:
invalid number of arguments in "limit_except" directive

```
server {
	limit_except GET GET;
}
```

Error:
duplicate method "GET"

```
server {
	limit_except get;
}
```

Error:
limit_except "get" is invalid

### autoindex

Example:

```
server {
	autoindex OFF;
}
```

Error:
autoindex can take "on" or "off"

<!-- ### index -->

### return

Example:

```
server {
	return fuga://hoge.www.com;
}
```

Error:
"fuga://hoge.www.com" is invalid

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
