#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

static void do_ls(char *path);

int
main(int argc, char *argv[])
{
  // 引数チェック
  if (argc < 2) {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }

  // lsを実行
  for (int i = 1; i < argc; i++) {
    do_ls(argv[i]);
  }
  exit(0);
}

static void
do_ls(char *path)
{
  // ディレクトリストリームを開く
  DIR *d = opendir(path);
  if (!d) {
    perror(path);
    exit(1);
  }

  // ディレクトリ内の全ての名前を表示する
  struct dirent *ent;
  while (ent = readdir(d)) {
    printf("%s\n", ent->d_name);
  }

  // ディレクトリストリームを閉じる
  closedir(d);
}
