// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Exception.h"
#include "Log.h"

extern int yyparse(yyscan_t yyscanner);

Scanner::Scanner()
    : compileUnit(nullptr), yyscanner(nullptr), bufferStack(this) {
  int r = yylex_init_extra(this, &yyscanner);
  EX_ASSERT(r == 0, "yylex_init_extra fail: {}", r);
  EX_ASSERT(yyscanner, "yyscanner must not null");
}

Scanner::~Scanner() {
  if (yyscanner) {
    yylex_destroy(yyscanner);
    yyscanner = nullptr;
  }
  if (compileUnit) {
    EX_ASSERT(compileUnit, "compileUnit must not null");
    delete compileUnit;
    compileUnit = nullptr;
  }
}

int Scanner::tokenize() { return yylex(yyscanner); }

int Scanner::parse() { return yyparse(yyscanner); }
