#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int do_cat(const int fd);
static void die(const char *s);

int
main(int argc, char *argv[])
{
	// パスが指定されなければ、標準入力を処理する
	if (argc < 2) {
		do_cat(STDIN_FILENO);
	}

	// パスの数だけcatを実行
	for (int i = 1; i < argc; i++) {
		// ファイルディスクリプタを開く
		int fd = open(argv[i], O_RDONLY);	// O_RDONLY: 読み込み専用
		if (fd < 0) { die(argv[i]); }
		if (do_cat(fd) < 0) { die(argv[i]); };
		// ファイルディスクリプタを閉じる
		if (close(fd) < 0) { die(argv[i]); }
	}
}

#define BUFFER_SIZE 2048

// ファイルディスクリプタを読みだして、標準出力へ連結(concatenate)する
static int
do_cat(const int fd)
{
	// ストリーム列から読み出しと書き出し
	for(;;) {
		unsigned char buf[BUFFER_SIZE];

		ssize_t n = read(fd, buf, sizeof buf);
		if (n < 0) {
			return(-1); 	// 読み出しエラー処理
		}
		if (n == 0) {
			break; 			// 読み出し終了
		}

		if (write(STDOUT_FILENO, buf, n) < 0) { // STDOUT_FILENO: 標準出力
			return(-1); // 書き出しエラー処理
		}
	}
	return(0);
}

// エラー出力して終了
static void
die(const char *s) {
	perror(s);	// "%s: <エラーメッセージ>" をエラー出力
	exit(1);
}
