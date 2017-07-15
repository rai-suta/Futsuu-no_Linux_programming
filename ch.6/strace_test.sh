#!/bin/sh

# 6.10 stdioの動作を確かめる

# 調査対象のプログラムをコンパイル
cc ch.1/list1.1_hello.c -o hello.o
cc ch.5/list5.1_cat.c -o cat.o
cc ch.6/list6.1_cat2.c -o cat2.o

# 4KBサイズのデータを作る
echo -n >./data.tmp
for cnt in `seq 1 1024`
do
  echo -n aaaa >>./data.tmp
done

# hello のシステムコールを表示する
strace ./hello.o 2>./strace_hello.log 1>/dev/null

# システムコール版catのシステムコールを表示する
strace -e trace=open,read,write,close ./cat.o ./data.tmp 2>./strace_cat.log 1>/dev/null
  # -e trace にてチェックするシステムコールを4つに限定

# stdio版catのシステムコールを表示する
strace -e trace=open,read,write,close ./cat2.o ./data.tmp 2>./strace_cat2.log 1>/dev/null
