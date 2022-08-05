#!/bin/bash

SRC_DIR=$PWD
TARGET_DIR=$PWD/submit

# コピー前に提出用ディレクトリのファイルを削除する
rm -rf "$TARGET_DIR"/*
cp -r "$SRC_DIR"/srcs "$TARGET_DIR"
cp -r "$SRC_DIR"/.gitignore "$TARGET_DIR"
cp -r "$SRC_DIR"/for_review/sample_data "$TARGET_DIR"
cp -r "$SRC_DIR"/for_review/default.conf "$TARGET_DIR"
sed -n "/# -------------------------- Rules For Test -------------------------------/q;p" "$SRC_DIR"/Makefile > "$TARGET_DIR"/Makefile
