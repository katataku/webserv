# 開発環境構築

- cpplintインストール
- clang-tidyのインストール
- [mdformat]のインストール

# vscode拡張機能

- cpplint
- C/C++ Include Guard
- Clang-Format

# コンテナの利用方法

## webserv

- build: make build
- up: make up
- login: make login
- down: make down

## nginx

- build: make build CONTAINER=nginx
- up: make up CONTAINER=nginx
- login: make login CONTAINER=nginx
- down: make down CONTAINER=nginx

[mdformat]: https://github.com/executablebooks/mdformat
