// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Parser.h"
#include "Parser.tab.hpp"
#include <cstdarg>
#include <cstdio>

void yyerror(YYLTYPE *t, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  if (t && t->first_line) {
    fprintf(stderr, "%d.%d-%d.%d: error: ", t->first_line, t->first_column,
            t->last_line, t->last_column);
  }
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}
