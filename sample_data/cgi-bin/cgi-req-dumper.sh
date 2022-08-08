#!/bin/bash

echo "<!DOCTYPE html>
<html>
  <head>
    <meta charset=\"utf-8\">
    <title>CGIRequest Dump</title>
  </head>
  <body>
  <h1>CGI Request Dump shell</h1>"

# 環境変数の出力
echo "    <p>AUTH_TYPE         : $AUTH_TYPE</p>"
echo "    <p>CONTENT_LENGTH    : $CONTENT_LENGTH</p>"
echo "    <p>CONTENT_TYPE      : $CONTENT_TYPE</p>"
echo "    <p>GATEWAY_INTERFACE : $GATEWAY_INTERFACE</p>"
echo "    <p>PATH_INFO         : $PATH_INFO</p>"
echo "    <p>PATH_TRANSLATED   : $PATH_TRANSLATED</p>"
echo "    <p>QUERY_STRING      : $QUERY_STRING</p>"
echo "    <p>REMOTE_HOST       : $REMOTE_HOST</p>"
echo "    <p>REMOTE_IDENT      : $REMOTE_IDENT</p>"
echo "    <p>REMOTE_USER       : $REMOTE_USER</p>"
echo "    <p>SCRIPT_NAME       : $SCRIPT_NAME</p>"
echo "    <p>SERVER_NAME       : $SERVER_NAME</p>"
echo "    <p>SERVER_PORT       : $SERVER_PORT</p>"
echo "    <p>SERVER_PROTOCOL   : $SERVER_PROTOCOL</p>"
echo "    <p>SERVER_SOFTWARE   : $SERVER_SOFTWARE</p>"
echo "    <p>SERVER_NAME       : $SERVER_NAME</p>"
echo "    <p>REQUEST_METHOD    : $REQUEST_METHOD</p>"

echo "  </body>
</html>"
