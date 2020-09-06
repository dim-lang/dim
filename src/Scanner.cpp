// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Log.h"
#include "TokenName.h"
#include <unordered_map>

Scanner::Scanner(const std::string &a_fileName)
    : fileName(a_fileName), yyBufferState(nullptr), fp(nullptr),
      yyscanner(nullptr), parser(nullptr), compileUnit(nullptr),
      parenthesesStack_() {
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

static bool isOpenParentheses(int tok) {
  return tok == yy::parser::token::T_LPAREN ||
         tok == yy::parser::token::T_LBRACKET ||
         tok == yy::parser::token::T_LBRACE;
}

static bool isCloseParentheses(int tok) {
  return tok == yy::parser::token::T_RPAREN ||
         tok == yy::parser::token::T_RBRACKET ||
         tok == yy::parser::token::T_RBRACE;
}

int Scanner::topParentheses() const { return parenthesesStack_.top(); }

int Scanner::eatParentheses(int tok) {
  if (isOpenParentheses(tok)) {
    parenthesesStack_.push(tok);
    return topParentheses();
  } else {
    LOG_ASSERT(isCloseParentheses(tok), "token {} must be close parentheses",
               tokenName(tok));
    LOG_ASSERT(!parenthesesStack_.empty(),
               "parenthesesStack_ must not empty:{}", parenthesesStack_.size());
    const static std::unordered_map<int, int> parenthesesMapping = {
        {yy::parser::token::T_RPAREN, yy::parser::token::T_LPAREN},
        {yy::parser::token::T_RBRACKET, yy::parser::token::T_LBRACKET},
        {yy::parser::token::T_RBRACE, yy::parser::token::T_LBRACE}};
    int save = topParentheses();
    LOG_ASSERT(parenthesesMapping.find(tok)->second == save,
               "topParentheses {} must match token {}", topParentheses(), tok);
    parenthesesStack_.pop();
    return save;
  }
}

int Scanner::newlineEnabled() const {
  return parenthesesStack_.empty() ||
         (topParentheses() == yy::parser::token::T_LBRACE ||
          topParentheses() == yy::parser::token::T_RBRACE);
}

bool Scanner::parenthesesEmpty() const { return parenthesesStack_.empty(); }

int Scanner::parenthesesSize() const { return parenthesesStack_.size(); }
