#!/usr/bin/env python3

import os
import sys

AUTH_TYPE = os.getenv('AUTH_TYPE')
CONTENT_LENGTH = os.getenv('CONTENT_LENGTH')
CONTENT_TYPE = os.getenv('CONTENT_TYPE')
GATEWAY_INTERFACE = os.getenv('GATEWAY_INTERFACE')
PATH_INFO = os.getenv('PATH_INFO')
PATH_TRANSLATED = os.getenv('PATH_TRANSLATED')
QUERY_STRING = os.getenv('QUERY_STRING')
REMOTE_HOST = os.getenv('REMOTE_HOST')
REMOTE_IDENT = os.getenv('REMOTE_IDENT')
REMOTE_USER = os.getenv('REMOTE_USER')
SCRIPT_NAME = os.getenv('SCRIPT_NAME')
SERVER_NAME = os.getenv('SERVER_NAME')
SERVER_PORT = os.getenv('SERVER_PORT')
SERVER_PROTOCOL = os.getenv('SERVER_PROTOCOL')
SERVER_SOFTWARE = os.getenv('SERVER_SOFTWARE')
SERVER_NAME = os.getenv('SERVER_NAME')
REQUEST_METHOD = os.getenv('REQUEST_METHOD')


def ptag(unformat: str) -> str:
    return '<p>' + unformat + '</p>'


print('Content-Type: text/html')

print('')

print('<!DOCTYPE html>')
print('<html>')
print('  <head>')
print('    <meta charset="utf-8">')
print('    <title>CGIRequest Dump</title>')
print('  </head>')
print('  <body>')

print('      <h1>CGI Request Dump</h1>')
print('    ' + ptag(f'AUTH_TYPE         : {AUTH_TYPE}'))
print('    ' + ptag(f'CONTENT_LENGTH    : {CONTENT_LENGTH}'))
print('    ' + ptag(f'CONTENT_TYPE      : {CONTENT_TYPE}'))
print('    ' + ptag(f'GATEWAY_INTERFACE : {GATEWAY_INTERFACE}'))
print('    ' + ptag(f'PATH_INFO         : {PATH_INFO}'))
print('    ' + ptag(f'PATH_TRANSLATED   : {PATH_TRANSLATED}'))
print('    ' + ptag(f'QUERY_STRING      : {QUERY_STRING}'))
print('    ' + ptag(f'REMOTE_HOST       : {REMOTE_HOST}'))
print('    ' + ptag(f'REMOTE_IDENT      : {REMOTE_IDENT}'))
print('    ' + ptag(f'REMOTE_USER       : {REMOTE_USER}'))
print('    ' + ptag(f'SCRIPT_NAME       : {SCRIPT_NAME}'))
print('    ' + ptag(f'SERVER_NAME       : {SERVER_NAME}'))
print('    ' + ptag(f'SERVER_PORT       : {SERVER_PORT}'))
print('    ' + ptag(f'SERVER_PROTOCOL   : {SERVER_PROTOCOL}'))
print('    ' + ptag(f'SERVER_SOFTWARE   : {SERVER_SOFTWARE}'))
print('    ' + ptag(f'SERVER_NAME       : {SERVER_NAME}'))
print('    ' + ptag(f'REQUEST_METHOD    : {REQUEST_METHOD}'))

for idx, arg in enumerate(sys.argv):
    print('    ' + ptag(f'ARGV[{idx}] : {arg}'))

print('  </body>')
print('</html>')
