#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_head(FILE *f, long nlines);

#define DEFAULT_N_LINES 10
#define dprintf(...)   printf(__VA_ARGS__)

// ロングオプション設定
static struct option longopts[] = {
  {"lines", required_argument, NULL, 'n'},
  {"help" , no_argument      , NULL, 'h'},
  {0}
};

int
main(int argc, char *argv[])
{
  dprintf( "--------------------------------\n" );
  dprintf( "argc = %d\n", argc );
  for (int i = 0; i < argc; i++) {
    dprintf( "argv[%d] = %s\n", i, argv[i] );
  }
  dprintf( "--------------------------------\n" );
  long nlines = DEFAULT_N_LINES;

  // オプション引数処理
  // getopt() に関連したグローバル変数
  //    optarg: 現在処理中のオプションのパラメータ
  //    optind: 現在処理中のオプションのargvでのインデックス
  //    optopt: 現在処理中のオプション文字
  //    opterr: 真ならばエラー時にgetopt()がメッセージを表示する
  int opt;
  while ( (opt = getopt_long(argc, argv, "n", longopts, NULL)) != -1 ) {
    switch (opt) {
      case 'n': { // 行数を指定する
        nlines = atoi(optarg);
      } break;
      case 'h': { // ヘルプを表示
        fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
        exit(0);
      } break;
      case '?': { // 不正なオプション
        fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
        exit(1);
      } break;
    }
  }
  // getopt() を処理すると、引数のパスが一番最後に移動される
  dprintf( "--------------------------------\n" );
  dprintf( "optind = %d\n", optind );
  for (int i = 0; i < argc; i++) {
    dprintf( "argv[%d] = %s\n", i, argv[i] );
  }
  dprintf( "--------------------------------\n" );

  // パス引数処理
  if (optind == argc) {   // 引数にパスが無い
    // 標準入力を処理する
    do_head(stdin, nlines);
  }
  else {                  // 引数にパスが有る
    for (int i = optind; i < argc; i++) {
      // ファイルストリームを開く
      FILE *f;
      if ( !(f = fopen(argv[i], "r")) ) {
        perror(argv[i]);    // エラー処理
        exit(1);
      }
      // ファイルストリームを処理する
      do_head(f, nlines);
      fclose(f);
    }
  }
  exit(0);
}

// 先頭から指定数の行を出力する
static void
do_head(FILE *f, long nlines)
{
  if ( nlines <= 0 ) { return; }

  int c = getc(f);
  while ( nlines > 0 && c != EOF ) {
    if (putchar(c) < 0) {
      exit(1);
    }
    if (c == '\n') {
      nlines--;
    }
    c = getc(f);
  }
}
