#!/usr/bin/env python3

import os
import sys


def write_to_file(path: str, data: str) -> None:
    with open(path, 'w') as f:
        f.write(data)


CONTENT_LENGTH = os.getenv('CONTENT_LENGTH')
PATH_INFO = os.getenv('PATH_INFO')

print(f'content length [{CONTENT_LENGTH}]', file=sys.stderr)

# サーバーからbodyが送信される場合
if CONTENT_LENGTH:
    print(f'[CGI log] Read body', file=sys.stderr)

    body = input()
    write_to_file('./hoge.file', body)


print('Content-Type: text/plain')
print('')
print('hello from cgi')
