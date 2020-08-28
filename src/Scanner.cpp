// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Log.h"
#include "tokenizer.yy.hh"

extern YY_DECL;

Scanner::Scanner(const std::string &a_fileName)
    : fileName(a_fileName), yyBufferState(nullptr), fp(nullptr),
      yyscanner(nullptr), parser(nullptr), compileUnit(nullptr) {
  // init scanner
  int r = yylex_init_extra(this, &yyscanner);
  LOG_ASSERT(r == 0, "yylex_init_extra fail: {}", r);
  LOG_ASSERT(yyscanner, "yyscanner must not null");
  parser = std::shared_ptr<yy::parser>(new yy::parser(yyscanner));

  // init buffer
  fp = std::fopen(fileName.c_str(), "r");
  LOG_ASSERT(fp, "file {} cannot open!", fileName);
  yyBufferState = yy_create_buffer(fp, YY_BUF_SIZE, yyscanner);
  LOG_ASSERT(yyBufferState, "yyBufferState {} on file {} create fail!",
             (void *)yyBufferState, fileName);
  yy_switch_to_buffer(yyBufferState, yyscanner);
  yyset_lineno(1, yyscanner);
}

Scanner::~Scanner() {
  if (yyBufferState) {
    yy_delete_buffer(yyBufferState, yyscanner);
    yyBufferState = nullptr;
  }
  if (fp) {
    std::fclose(fp);
    fp = nullptr;
  }
  if (yyscanner) {
    yylex_destroy(yyscanner);
    yyscanner = nullptr;
  }
}

yy::parser::symbol_type Scanner::tokenize() { return yylex(yyscanner); }

int Scanner::parse() { return parser->parse(); }
