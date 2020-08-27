// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Log.h"

Scanner::Scanner()
    : compileUnit(nullptr), yyscanner(nullptr), bufferStack(this),
      parser(nullptr) {
  int r = yylex_init_extra(this, &yyscanner);
  LOG_ASSERT(r == 0, "yylex_init_extra fail: {}", r);
  LOG_ASSERT(yyscanner, "yyscanner must not null");
  parser = std::shared_ptr<yy::parser>(new yy::parser(yyscanner));
}

Scanner::~Scanner() {
  if (yyscanner) {
    yylex_destroy(yyscanner);
    yyscanner = nullptr;
  }
}

yy::parser::symbol_type Scanner::tokenize() { return yylex(yyscanner); }

int Scanner::parse() { return parser->parse(); }
