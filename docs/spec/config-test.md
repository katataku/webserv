一旦ドキュメントとして書いておくが、最終的にはテストケースとしてそれぞれについて用意することを想定している。

# テストケース

## 全般

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

<!-- ### 同コンテキストにディレクティブが重複して存在する

Example:

```
server {
	server_name hoge1.com;
	server_name hoge2.com;
}
```

Error:
duplicate "server_name" in server context -->

## 具体的なケース

### server_name

<!-- 

BNF

<official hostname> ::= <hname>
<hname> ::= <name>*["."<name>]
<name>  ::= <let-or-digit>[*[<let-or-digit-or-hyphen>]<let-or-digit>]

letter or digitだがRFC的には以下の文字が推奨されている

Recommended

"A-Z", "a-z", "0-9", dash and underscore

See https://suu-g.hateblo.jp/entry/2019/09/19/232913

 -->

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

### error_page

### client_max_body_size

Example:

```
server {
	client_max_body_size a;
}
```

Error:
"client_max_body_size" directive invalid value

### alias

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

<!-- 
これはエラー？

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
unknown method "get", may be "GET"? -->

### autoindex

<!-- 
nginxはケースインセンシティブ?

Example:

```
server {
	autoindex OFF;
}
```

Error:
autoindex cat take "on" or "off" -->

### index

### return

Example:

```
server {
	return fuga://hoge.www.com;
}
```

Error:
invalid return code "fuga://hoge.www.com"

### server

Example:

```
server
```

Error:
unexpected end of file, expecting ";" or "}"

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

<!-- Example:

このケースもinvalid number of arguments in "location" directiveでエラーにしたい

```
server {
	location / /hoge {
		alias /var/html/www;
	}
}
```

Error:
invalid location modifier "/" -->
