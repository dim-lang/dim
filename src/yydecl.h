// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

#define YY_NO_UNISTD_H
#include "parser.tab.hh"
#define YY_DECL yy::parser::symbol_type yylex(yyscan_t yyscanner)
extern YY_DECL;
#include "location.hh"
#include "tokenizer.yy.hh"
