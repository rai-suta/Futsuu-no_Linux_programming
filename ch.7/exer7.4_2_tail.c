#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_tail(FILE *f, int nrow);
static int get_nrow(FILE *f);

#define DEFAULT_N_LINES 5

int
main(int argc, char *argv[])
{
  // オプション引数処理
  int opt, opt_n = DEFAULT_N_LINES;
  while ( (opt = getopt(argc, argv, "hn:")) != -1 ) {
    switch (opt) {
      case 'n': {
        opt_n = atoi(optarg);
      } break;
      case 'h': {
        fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
        exit(0);
      } break;
    }
  }

  if (argc == optind) { // 引数 エラー処理
    fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
    exit(1);
  }

  // ファイルパスを開く
  for (int i = optind; i < argc; i++) {
    FILE *f = fopen(argv[i], "r");
    if (!f) { // ファイルオープン エラー処理
      perror(argv[i]);
      exit(1);
    }
    do_tail(f, opt_n);
    fclose(f);
  }
  exit(0);
}

// ファイル最後の数行を出力する
static void
do_tail(FILE *f, int nrow)
{
  if ( nrow <= 0 ) {
    return;
  }

  // ファイルの行数
  int file_nrow = get_nrow(f);

  // ファイル先頭へシーク
  int seek_err = fseek(f, 0, SEEK_SET);
  if (seek_err) {
    exit(1);    // ファイルシーク エラー処理
  }

  // 目的の行まで読み飛ばす
  int c = fgetc(f);
  while ( file_nrow > nrow && c != EOF ) {
    if ( c == '\n' ) { file_nrow--; }
    c = fgetc(f);
  }

  // 出力する
  while ( c != EOF ) {
    int put_err = putchar(c);
    if (put_err < 0) {
      exit(1);    // ストリーム出力 エラー処理
    }
    c = fgetc(f);
  }
}

// ファイルの行数を取得する
static int
get_nrow(FILE *f)
{
  // 標準入力から1行ずつ読み出す
  int b, c, nrow = 0;
  while ( (c = getc(f)) != EOF ) {
    if ( c == '\n' ) { nrow++; }
    b = c;
  }
  if ( b != '\n' ) { nrow++; }  // 改行なしの終端

  return nrow;
}
