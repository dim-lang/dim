#include "bison.tab.hpp"
#include <cstdio>

extern FILE *yyin;
extern int yylex(void);

int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("error! filename missing!\n");
    return 0;
  }

  FILE *fp = fopen(argv[1], "r");
  yyin = fp;
  int t;
  while ((t = yylex()) != 0) {
    printf("token: %d", t);
    if (t == FT_IDENTIFIER || t == FT_INTEGER || t == FT_DOUBLE ||
        t == FT_STRING) {
      printf("literal: %s\n", yylval.literal);
    } else {
      printf("\n");
    }
  }
  fclose(fp);

  return 0;
}
