#!/bin/bash

SRC_DIR=$PWD
TARGET_DIR=$PWD/submit

cp -r "$SRC_DIR"/srcs "$TARGET_DIR"
cp -r "$SRC_DIR"/docs "$TARGET_DIR"
cp -r "$SRC_DIR"/test_data "$TARGET_DIR"
cp -r "$SRC_DIR"/sample_data "$TARGET_DIR"
cp -r "$SRC_DIR"/.gitignore "$TARGET_DIR"
sed -n "/# -------------------------- Rules For Test -------------------------------/q;p" "$SRC_DIR"/Makefile > "$TARGET_DIR"/Makefile
