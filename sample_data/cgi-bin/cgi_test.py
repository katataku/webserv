#!/usr/bin/env python3

import os
import sys


def print_log(msg: str) -> None:
    print(f'[CGI log] {msg}', file=sys.stderr)


def write_to_file(path: str, data: str) -> None:
    with open(path, 'w') as f:
        f.write(data)


def print_status_line(code: str, phrase: str) -> None:
    print(f'Status: {code} {phrase}')


REQUEST_METHOD = os.getenv('REQUEST_METHOD')
CONTENT_LENGTH = os.getenv('CONTENT_LENGTH')
PATH_INFO = os.getenv('PATH_INFO')

# サーバーからbodyが送信される場合
if CONTENT_LENGTH:
    print_log("Read body")
    body = sys.stdin.read()
    write_to_file('/var/www/html/hoge.file', body)
    print_status_line("201", "Created")
    print('')
    exit(0)

if REQUEST_METHOD == "DELETE":
    print_log("Delete file")
    os.remove('/var/www/html/hoge.file')
    print_status_line("200", "OK")
    print('')
    exit(0)

print('Content-Type: text/plain')
print('')
print('hello from cgi')
