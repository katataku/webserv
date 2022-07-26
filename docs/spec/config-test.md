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
