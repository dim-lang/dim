// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Parser.h"
#include "Ast.h"
#include "Parser.tab.hpp"
#include <cstdarg>
#include <cstdio>

void yyerror(AstProgram **program, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  if (yylloc.first_line) {
    fprintf(stderr, "%d.%d-%d.%d: error: ", yylloc.first_line,
            yylloc.first_column, yylloc.last_line, yylloc.last_column);
  }
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}
