// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#define YY_DECL yy::parser::symbol_type yylex(yyscan_t yyscanner)
struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
typedef void *yyscan_t;
void *yyget_extra(yyscan_t yyscanner);

#include "Ast.h"
#include "location.hh"
#include "parser.tab.hh"
#include <cstdio>
#include <memory>
#include <string>
#include <utility>

class Scanner {
public:
  Scanner(const std::string &a_fileName);
  virtual ~Scanner();

  // attributes
  std::string fileName;
  YY_BUFFER_STATE yyBufferState;
  yy::location location;
  FILE *fp;
  yyscan_t yyscanner;
  std::shared_ptr<yy::parser> parser;

  std::shared_ptr<Ast> compileUnit;

  // wrapper for flex/bison
  virtual yy::parser::symbol_type tokenize();
  virtual int parse();
};
