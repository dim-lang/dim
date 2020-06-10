// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Log.h"
#include "Parser.h"

Scanner::Scanner()
    : fileName_(""), translateUnit_(nullptr), yy_scaninfo_(nullptr),
      bufferStack_(nullptr) {
  int r = yylex_init_extra(this, &yy_scaninfo_);
  X_ASSERT(r == 0, "yylex_init_extra fail: {}", r);
  translateUnit_ = new AstTranslateUnit();
  bufferStack_ = new BufferStack(yy_scaninfo_);
}

Scanner::~Scanner() {
  if (yy_scaninfo_) {
    yylex_destroy(yy_scaninfo_);
    yy_scaninfo_ = nullptr;
  }
  if (translateUnit_) {
    delete translateUnit_;
    translateUnit_ = nullptr;
  }
  if (bufferStack_) {
    delete bufferStack_;
    bufferStack_ = nullptr;
  }
}

int Scanner::pushBuffer(const std::string &fileName) {
  X_ASSERT(bufferStack_, "bufferStack_ is null");
  return bufferStack_->push(fileName);
}

int Scanner::popBuffer() {
  X_ASSERT(bufferStack_, "bufferStack_ is null");
  return bufferStack_->pop();
}

const std::string &Scanner::currentBuffer() const {
  X_ASSERT(bufferStack_, "bufferStack_ is null");
  X_ASSERT(!bufferStack_->empty(), "bufferStack_ is empty: {}!",
          bufferStack_->size());
  return bufferStack_->top()->fileName;
}

Buffer *Scanner::top() const {
  X_ASSERT(bufferStack_, "bufferStack_ is null");
  X_ASSERT(!bufferStack_->empty(), "bufferStack_ is empty: {}!",
          bufferStack_->size());
  return bufferStack_->top();
}

int Scanner::size() const {
  X_ASSERT(bufferStack_, "bufferStack_ is null");
  return bufferStack_->size();
}

bool Scanner::empty() const {
  X_ASSERT(bufferStack_, "bufferStack_ is null");
  return bufferStack_->empty();
}

const AstTranslateUnit *Scanner::translateUnit() const {
  return translateUnit_;
}

AstTranslateUnit *&Scanner::translateUnit() { return translateUnit_; }

const yyscan_t Scanner::yy_scaninfo() const { return yy_scaninfo_; }

yyscan_t &Scanner::yy_scaninfo() { return yy_scaninfo_; }

const std::string &Scanner::fileName() const { return fileName_; }

std::tuple<int, YYSTYPE, YYLTYPE> Scanner::tokenize() {
  YYSTYPE yy_value;
  YYLTYPE yy_location;
  int r = yylex(&yy_value, &yy_location, yy_scaninfo_);
  return std::make_tuple(r, yy_value, yy_location);
}

int Scanner::parse() { return yyparse(yy_scaninfo_); }
