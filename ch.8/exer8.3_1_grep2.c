#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

#define _GNU_SOURCE
#include <getopt.h>

static bool ignoreCase, invertMatch;
static struct option longopts[] = {
  {"ignore-case",   no_argument, NULL, 'i'},
  {"invert-match",  no_argument, NULL, 'v'},
  {0}
};

static void do_grep(regex_t *pat, FILE *src);

int
main(int argc, char *argv[])
{
  // オプション引数処理
  int opt;
  while ( (opt = getopt_long(argc, argv, "ivh", longopts, NULL)) != -1 ) {
    switch (opt) {
      case 'i': { // --ignore-case: 文字ケースを区別しない
        ignoreCase = true;
      } break;
      case 'v': { // --invert-match: マッチしない行を選択する
        invertMatch = true;
      } break;
      case 'h': {
        fprintf(stdout, "Usage: %s [-i] [-v] [-h] PATTERN [FILE ...]\n", argv[0]);
        exit(0);
      } break;
    }
  }

  // 正規表現引数
  if (optind == argc) {
    fputs("no pattern\n", stderr);
    exit(1);
  }
  char *argv_pttrn = argv[optind++];

  // 正規表現をチェック
  regex_t pat;
  int err = regcomp(&pat, argv_pttrn
                    , REG_EXTENDED  // POSIX拡張正規表現を使う
                      | REG_NOSUB   // マッチの場所を報告しない、成功・失敗のみを返す
                      | REG_NEWLINE // 1行ごとに検索する
                      | (ignoreCase ? REG_ICASE : 0)  // 文字ケースを無視する
                    );
  if (err != 0) { // 正規表現 エラー処理
    char buf[1024];
    regerror(err, &pat, buf, sizeof buf);
    puts(buf);
    exit(1);
  }

  // grep処理
  if (optind == argc) {
    //  標準入力から
    do_grep(&pat, stdin);
  }
  else {
    // ファイルから
    for (optind; optind < argc; optind++) {
      FILE *f = fopen(argv[optind], "r");
      if (!f) { // ファイルオープン エラー処理
        perror(argv[optind]);
        exit(1);
      }
      do_grep(&pat, f);
      fclose(f);
    }
  }
  regfree(&pat);
  exit(0);
}

static void
do_grep(regex_t *pat, FILE *src)
{
  char buf[4096];
  const int matcCase = invertMatch ? REG_NOMATCH  // 正規表現にマッチしない
                                   : 0;           // 正規表現にマッチする

  while (fgets(buf, sizeof buf, src)) {
    if (regexec(pat, buf, 0, NULL, 0) == matcCase) {
      fputs(buf, stdout);
    }
  }
}
