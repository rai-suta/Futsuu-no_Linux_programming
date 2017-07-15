#!/bin/sh

# 7.3 gdbを使ったデバッグ

# デバッグ対象のプログラムをコンパイル
gcc -Wall -g -o head3.o ch.7/head3_bug.c

# segmentation faultで異常終了
./head3.o -n 5 1>/dev/null

# gdb を実行
gdb ./head3.o
