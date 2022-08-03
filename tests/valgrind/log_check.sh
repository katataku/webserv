#!/bin/bash

CONFIG_FILE=./log/summary-valgrind.log

grep definitely ./log/summary-valgrind.log  | grep -v "0 byte"
# ログファイルにリークが含まれていた場合はexit 1.
if [ $? -eq 0 ] ; then
    exit 1
fi
