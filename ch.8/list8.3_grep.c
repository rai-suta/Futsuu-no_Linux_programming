#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

static void do_grep(regex_t *pat, FILE *src);

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fputs("no pattern\n", stderr);
    exit(1);
  }

  // 正規表現をチェック
  regex_t pat;
  int err = regcomp(&pat, argv[1]
                    , REG_EXTENDED  // POSIX拡張正規表現を使う
                      | REG_NOSUB   // マッチの場所を報告しない、成功・失敗のみを返す
                      | REG_NEWLINE // 1行ごとに検索する
                    );
  if (err != 0) { // 正規表現 エラー処理
    char buf[1024];
    regerror(err, &pat, buf, sizeof buf);
    puts(buf);
    exit(1);
  }

  // grep処理
  if (argc == 2) {
    //  標準入力から
    do_grep(&pat, stdin);
  }
  else {
    // ファイルから
    for (int i = 2; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (!f) { // ファイルオープン エラー処理
        perror(argv[i]);
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

  while (fgets(buf, sizeof buf, src)) {
    if (regexec(pat, buf, 0, NULL, 0) == 0) {
      fputs(buf, stdout);
    }
  }
}
