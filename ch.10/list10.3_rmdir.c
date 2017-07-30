#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
  // 引数チェック
  if (argc < 2) {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }

  // ディレクトリを削除する
  for (int i = 1; i < argc; i++) {
    if (rmdir(argv[i]) < 0) {
      perror(argv[i]);
      exit(1);
    }
  }
  exit(0);
}
