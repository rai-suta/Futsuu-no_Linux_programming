#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define _GNU_SOURCE
#include <getopt.h>

static bool repTab, repLf;

static void do_cat(FILE *f);

int
main(int argc, char *argv[])
{
  // オプション引数処理
  int opt;
  while ( (opt = getopt(argc, argv, "tn")) != -1 ) {
    switch (opt) {
      case 't': {
        repTab = true;
      } break;
      case 'n': {
        repLf = true;
      } break;
      case 'h': {
        fprintf(stdout, "Usage: %s [-t] [-n] [FILE ...]\n", argv[0]);
        exit(0);
      } break;
    }
  }

  // パス引数処理
  if (optind == argc) {
    // 標準入力を処理する
    do_cat(stdin);
  }
  else {
    // ファイルパスを開く
    for (int i = optind; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);  // ファイルオープン エラー処理
        exit(1);
      }
      do_cat(f);
      fclose(f);
    }
  }
  exit(0);
}

// TAB、改行を可視化するcat
static void
do_cat(FILE *f)
{
  int c;
  while ((c = fgetc(f)) != EOF) {
    int put_err = (repTab && c == '\t')
                    ? fputs("\\t", stdout)
                    : (repLf && c == '\n')
                      ? fputs("$\n", stdout)
                      : putchar(c);
    if (put_err < 0) {
      exit(1);    // ストリーム出力 エラー処理
    }
  }
}
