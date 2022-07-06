一旦ドキュメントとして書いておくが、最終的にはテストケースとしてそれぞれについて用意することを想定している。

# テストケース
## 全般
### コンテキストが違う場所にディレクティブがある
Example: 
```
server {
    limit_except GET POST DELETE {}
}
```

Error:
"limit_except" directive is not allowed here

### limit_except
Example: 
```
server {
    limit_except HOGE {}
}
```

Error:
invalid method "HOGE"

Example: 
```
server {
    limit_except {}
}
```
Error:
invalid number of arguments in "limit_except" directive
