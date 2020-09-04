// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Log.h"
#include "TokenName.h"
#include "tokenizer.yy.hh"
#include <unordered_map>

extern YY_DECL;

Scanner::Scanner(const std::string &a_fileName)
    : fileName(a_fileName), yyBufferState(nullptr), fp(nullptr),
      yyscanner(nullptr), parser(nullptr), compileUnit(nullptr),
      parenthesesStack_(), parentheses_region_(true) {
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

static bool isParenthesesRegion(int tok) {
  return tok == yy::parser::token::T_LPAREN ||
         tok == yy::parser::token::T_LBRACKET ||
         tok == yy::parser::token::T_LBRACE ||
         tok == yy::parser::token::T_RPAREN ||
         tok == yy::parser::token::T_RBRACKET ||
         tok == yy::parser::token::T_RBRACE;
}

static bool isDoWhileRegion(int tok) {
  return tok == yy::parser::token::T_DO || tok == yy::parser::token::T_WHILE;
}

static bool isTryCatchRegion(int tok) {
  return tok == yy::parser::token::T_TRY || tok == yy::parser::token::T_CATCH ||
         tok == yy::parser::token::T_FINALLY;
}

int Scanner::topRegionToken() { return bracketStack_.top(); }

int Scanner::eatRegionToken(int tok) {
  if (tok == yy::parser::token::T_LPAREN ||
      tok == yy::parser::token::T_LBRACKET ||
      tok == yy::parser::token::T_LBRACE) {
    bracketStack_.push(tok);
    return bracketStack_.top();
  } else if (tok == yy::parser::token::T_RPAREN ||
             tok == yy::parser::token::T_RBRACKET ||
             tok == yy::parser::token::T_RBRACE) {
    const static std::unordered_map<int, int> bracketMapping = {
        {yy::parser::token::T_LPAREN, yy::parser::token::T_RPAREN},
        {yy::parser::token::T_LBRACKET, yy::parser::token::T_RBRACKET},
        {yy::parser::token::T_LBRACE, yy::parser::token::T_RBRACE}};
    LOG_ASSERT(!bracketStack_.empty(), "bracketStack_ must not empty:{}",
               bracketStack_.size());
    int save = bracketStack_.top();
    LOG_ASSERT(bracketMapping[save] == tok,
               "bracketStack_.top {} must match token {}", bracketStack_.top(),
               tok);
    bracketStack_.pop();
    return save;
  } else {
    LOG_ASSERT(false, "invalid token: {}", tokenName(tok));
  }
}

int Scanner::newline_enabled() const { return nl_bracket_region_; }
