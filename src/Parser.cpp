// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Parser.h"
#include <cstdarg>
#include <cstdio>

void yyerror(AstProgram **program, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}
