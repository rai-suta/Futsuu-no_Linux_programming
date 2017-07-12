#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void do_cat(const char *path);
static void die(const char *s);

int
main(int argc, char *argv[])
{
	// パラメータを確認
	if (argc < 2) {
		fprintf(stderr, "%s: file name not given\n", argv[0]);
		exit(1);
	}

	// 引数の数だけcatを実行
	for (int i = 1; i < argc; i++) {
		do_cat(argv[i]);
	}
}

#define BUFFER_SIZE 2048

// ファイルパスを読みだして、標準出力へ連結(concatenate)する
static void
do_cat(const char *path)
{
	// ファイルディスクリプタを開く
	int fd = open(path, O_RDONLY);	// O_RDONLY: 読み込み専用
	if (fd < 0) { die(path); }

	// ストリーム列から読み出しと書き出し
	for(;;) {
		unsigned char buf[BUFFER_SIZE];

		ssize_t n = read(fd, buf, sizeof buf);
		if (n < 0) {
			die(path); 	// 読み出しエラー処理
		}
		if (n == 0) {
			break; 			// 読み出し終了
		}

		if (write(STDOUT_FILENO, buf, n) < 0) { // STDOUT_FILENO: 標準出力
			die(path); // 書き出しエラー処理
		}
	}

	// ファイルディスクリプタを閉じる
	if (close(fd) < 0) { die(path); }
}

// エラー出力して終了
static void
die(const char *s) {
	perror(s);	// "%s: <エラーメッセージ>" をエラー出力
	exit(1);
}
