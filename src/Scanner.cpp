// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Log.h"

extern int yyparse(yyscan_t yyscanner);

Scanner::Scanner()
    : fileName_(""), translateUnit_(nullptr), yy_scanner_(nullptr),
      bufferStack_(nullptr) {
  int r = yylex_init_extra(this, &yy_scanner_);
  EX_ASSERT(r == 0, "yylex_init_extra fail: {}", r);
  translateUnit_ = new AstTranslateUnit();
  bufferStack_ = new BufferStack(yy_scanner_);
}

Scanner::~Scanner() {
  if (yy_scanner_) {
    yylex_destroy(yy_scanner_);
    yy_scanner_ = nullptr;
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

std::tuple<int, YYSTYPE, YYLTYPE> Scanner::tokenize() {
  YYSTYPE yy_lval;
  YYLTYPE yy_lloc;
  int r = yylex(&yy_lval, &yy_lloc, yy_scanner_);
  return std::make_tuple(r, yy_lval, yy_lloc);
}

int Scanner::parse() { return yyparse(yy_scanner_); }
