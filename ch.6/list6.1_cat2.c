#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
  for (int i = 1; i < argc; i++) {
    // 読み込み専用でファイルを開く
    FILE *f = fopen(argv[i], "r");
    if (!f) {
      perror(argv[i]);  // ファイルオープン エラー処理
      exit(1);
    }

    // EOFになるまで入力ストリームを出力
    int c;
    while ((c = fgetc(f)) != EOF) {
      if (putchar(c) < 0) {
        exit(1);    // ストリーム出力 エラー処理
      }
    }

    // ファイルストリームを閉じる
    fclose(f);
  }
  exit(0);
}
