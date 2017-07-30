#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int
main(int argc, char *argv[])
{
  // 引数チェック
  if (argc < 2) {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }

  // パーミッションマスクを設定
  //               rwx rwx rwx
  // mode  = 777 : 111 111 111
  // umask = 022 : 000 010 010
  // res   = 755 : 111 101 101
  fprintf(stderr, "pre umask = %03o\n", umask(022));

  // ディレクトリを作成
  for (int i = 1; i < argc; i++) {
    if (mkdir(argv[i], 0777) < 0) {
      perror(argv[i]);
      exit(1);
    }
  }
  exit(0);
}
