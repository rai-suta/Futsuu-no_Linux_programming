#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
  // 標準入力から1行ずつ読み出す
  int b, c, nrow = 0;
  while ( (c = getc(stdin)) != EOF ) {
    if ( c == '\n' ) { nrow++; }
    b = c;
  }
  if ( b != '\n' ) { nrow++; }  // 改行なしの終端

  // 行数を表示
  printf("%d\n", nrow);

  exit(0);
}
