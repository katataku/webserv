#!/bin/bash

CONFIG_FILE=./log/summary-valgrind.log

# ログファイルにリークが含まれていた場合はexit 1.
if [ `grep definitely ./log/summary-valgrind.log  | grep -v "0 byte"` ] ; then
    exit 1
fi
