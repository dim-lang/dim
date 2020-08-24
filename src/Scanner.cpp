// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Exception.h"
#include "Log.h"

extern int yyparse(yyscan_t yyscanner);

Scanner::Scanner()
    : translateUnit_(nullptr), yyscanner_(nullptr), fileName_(""),
      bufferStack_(nullptr) {
  int r = yylex_init_extra(this, &yyscanner_);
  EX_ASSERT(r == 0, "yylex_init_extra fail: {}", r);
  translateUnit_ = new AstTranslateUnit();
  bufferStack_ = new BufferStack(this);
  EX_ASSERT(yyscanner_, "yyscanner_ must not null");
  EX_ASSERT(translateUnit_, "translateUnit_ must not null");
  EX_ASSERT(bufferStack_, "bufferStack_ must not null");
}

Scanner::~Scanner() {
  EX_ASSERT(yyscanner_, "yyscanner_ must not null");
  yylex_destroy(yyscanner_);
  yyscanner_ = nullptr;
  EX_ASSERT(translateUnit_, "translateUnit_ must not null");
  delete translateUnit_;
  translateUnit_ = nullptr;
  EX_ASSERT(bufferStack_, "bufferStack_ must not null");
  delete bufferStack_;
  bufferStack_ = nullptr;
}

int Scanner::pushBuffer(const std::string &fileName) {
  EX_ASSERT(bufferStack_, "bufferStack must not null");
  return bufferStack_->push(fileName);
}

int Scanner::popBuffer() {
  EX_ASSERT(bufferStack_, "bufferStack must not null");
  return bufferStack_->pop();
}

const std::string &Scanner::currentBuffer() const {
  EX_ASSERT(bufferStack_, "bufferStack must not null");
  EX_ASSERT(!bufferStack_->empty(), "bufferStack_ is empty: {}!",
            bufferStack_->size());
  return bufferStack_->top()->fileName;
}

Buffer *Scanner::top() const {
  EX_ASSERT(bufferStack_, "bufferStack must not null");
  EX_ASSERT(!bufferStack_->empty(), "bufferStack_ is empty: {}!",
            bufferStack_->size());
  return bufferStack_->top();
}

int Scanner::size() const {
  EX_ASSERT(bufferStack_, "bufferStack must not null");
  return bufferStack_->size();
}

bool Scanner::empty() const {
  EX_ASSERT(bufferStack_, "bufferStack must not null");
  return bufferStack_->empty();
}

const std::string &Scanner::fileName() const { return fileName_; }

const AstTranslateUnit *Scanner::translateUnit() const {
  return translateUnit_;
}

AstTranslateUnit *&Scanner::translateUnit() { return translateUnit_; }

const yyscan_t *Scanner : yyscanner() const { return yyscanner_; }

yyscan_t *&Scanner::yyscanner() { return yyscanner_; }

std::tuple<int, YYSTYPE, YYLTYPE> Scanner::tokenize() {
  YYSTYPE yy_lval;
  YYLTYPE yy_lloc;
  int r = yylex(&yy_lval, &yy_lloc, yyscanner_);
  return std::make_tuple(r, yy_lval, yy_lloc);
}

int Scanner::parse() { return yyparse(yyscanner_); }
