// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Ast.h"
#include "Log.h"
#include "tokenizer.yy.hh"
#include <unordered_map>

Scanner::Scanner(const std::string &fileName)
    : fileName_(fileName), yyBufferState_(nullptr), fp_(nullptr),
      yyscanner_(nullptr), compileUnit_(nullptr), parenthesesStack_() {
  // init scanner
  int r = yylex_init_extra(this, &yyscanner_);
  LOG_ASSERT(r == 0, "yylex_init_extra fail: {}", r);
  LOG_ASSERT(yyscanner_, "yyscanner_ must not null");

  // init buffer
  fp_ = std::fopen(fileName_.c_str(), "r");
  LOG_ASSERT(fp_, "file {} cannot open!", fileName_);
  yyBufferState_ = yy_create_buffer(fp_, YY_BUF_SIZE, yyscanner_);
  LOG_ASSERT(yyBufferState_, "yyBufferState_ {} on file {} create fail!",
             (void *)yyBufferState_, fileName_);
  yy_switch_to_buffer(yyBufferState_, yyscanner_);
  yyset_lineno(1, yyscanner_);
}

Scanner::~Scanner() {
  if (yyBufferState_) {
    yy_delete_buffer(yyBufferState_, yyscanner_);
    yyBufferState_ = nullptr;
  }
  if (fp_) {
    std::fclose(fp_);
    fp_ = nullptr;
  }
  if (yyscanner_) {
    yylex_destroy(yyscanner_);
    yyscanner_ = nullptr;
  }
  if (compileUnit_) {
    delete compileUnit_;
    compileUnit_ = nullptr;
  }
}

const std::string &Scanner::fileName() const { return fileName_; }

const Ast *Scanner::compileUnit() const { return compileUnit_; }

Ast *&Scanner::compileUnit() { return compileUnit_; }

Token Scanner::tokenize() {
  YYSTYPE yylval;
  YYLTYPE yylloc;
  int value = yylex(&yylval, &yylloc, yyscanner_);
  return Token(value, yylval, yylloc);
}

int Scanner::parse() { return yyparse(yyscanner_); }

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
