// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Log.h"
#include "tokenizer.yy.hh"
#include <unordered_map>

Scanner::Scanner(const std::string &a_fileName)
    : fileName(a_fileName), yyBufferState(nullptr), fp(nullptr),
      yyscanner(nullptr), compileUnit(nullptr), parenthesesStack_() {
  // init scanner
  int r = yylex_init_extra(this, &yyscanner);
  LOG_ASSERT(r == 0, "yylex_init_extra fail: {}", r);
  LOG_ASSERT(yyscanner, "yyscanner must not null");

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

Token Scanner::tokenize() {
  YYSTYPE yylval;
  YYLTYPE yylloc;
  int value = yylex(&yylval, &yylloc, yyscanner);
  return Token(value, yylval, yylloc);
}

int Scanner::parse() { return yyparse(yyscanner); }

static bool isOpenParentheses(int tok) {
  return tok == yytokentype::T_LPAREN || tok == yytokentype::T_LBRACKET ||
         tok == yytokentype::T_LBRACE;
}

static bool isCloseParentheses(int tok) {
  return tok == yytokentype::T_RPAREN || tok == yytokentype::T_RBRACKET ||
         tok == yytokentype::T_RBRACE;
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
        {yytokentype::T_RPAREN, yytokentype::T_LPAREN},
        {yytokentype::T_RBRACKET, yytokentype::T_LBRACKET},
        {yytokentype::T_RBRACE, yytokentype::T_LBRACE}};
    int save = topParentheses();
    LOG_ASSERT(parenthesesMapping.find(tok)->second == save,
               "topParentheses {} must match token {}", topParentheses(), tok);
    parenthesesStack_.pop();
    return save;
  }
}

int Scanner::newlineEnabled() const {
  return parenthesesStack_.empty() ||
         (topParentheses() == yytokentype::T_LBRACE ||
          topParentheses() == yytokentype::T_RBRACE);
}

bool Scanner::parenthesesEmpty() const { return parenthesesStack_.empty(); }

int Scanner::parenthesesSize() const { return parenthesesStack_.size(); }
