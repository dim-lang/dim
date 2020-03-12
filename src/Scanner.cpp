// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Scanner.h"
#include "Log.h"
#include "Parser.h"

Scanner::Scanner(const std::string &fileName)
    : fileName_(fileName), program_(nullptr), gss_(nullptr), css_(nullptr),
      gts_(nullptr), cts_(nullptr) yy_scaninfo_(nullptr),
      bufferStack_(nullptr) {
  int r = yylex_init_extra(this, &yy_scaninfo_);
  CASSERT(r == 0, "yylex_init_extra fail: {}", r);
  program_ = new AstProgram();
  Symbol::push(gss_, css_, new GlobalSymtab());
  Type::push(gts_, cts_, new GlobalTytab());
  bufferStack_ = new BufferStack(yy_scaninfo_);
  int p = push(fileName);
  CASSERT(p == 1, "bufferStack_#push fail: {}", p);
}

Scanner::~Scanner() {
  if (yy_scaninfo_) {
    yylex_destroy(yy_scaninfo_);
    yy_scaninfo_ = nullptr;
  }
  if (program_) {
    delete program_;
    program_ = nullptr;
  }
  if (gss_) {
    delete gss_;
    gss_ = nullptr;
  }
  css_ = nullptr;
  if (gts_) {
    delete gts_;
    gts_ = nullptr;
  }
  cts_ = nullptr;
  if (bufferStack_) {
    delete bufferStack_;
    bufferStack_ = nullptr;
  }
}

int Scanner::push(const std::string &fileName) {
  CASSERT(bufferStack_, "bufferStack_ is null");
  return bufferStack_->push(fileName);
}

int Scanner::pop() {
  CASSERT(bufferStack_, "bufferStack_ is null");
  return bufferStack_->pop();
}

const std::string &Scanner::currentBuffer() const {
  CASSERT(bufferStack_, "bufferStack_ is null");
  CASSERT(!bufferStack_->empty(), "bufferStack_ is empty: {}!",
          bufferStack_->size());
  return bufferStack_->top()->fileName;
}

Buffer *Scanner::top() const {
  CASSERT(bufferStack_, "bufferStack_ is null");
  CASSERT(!bufferStack_->empty(), "bufferStack_ is empty: {}!",
          bufferStack_->size());
  return bufferStack_->top();
}

int Scanner::size() const {
  CASSERT(bufferStack_, "bufferStack_ is null");
  return bufferStack_->size();
}

bool Scanner::empty() const {
  CASSERT(bufferStack_, "bufferStack_ is null");
  return bufferStack_->empty();
}

const AstProgram *Scanner::program() const { return program_; }

AstProgram *&Scanner::program() { return program_; }

const Symtab *Scanner::gss() const { return gss_; }

Symtab *&Scanner::gss() { return gss_; }

const Symtab *Scanner::css() const { return css_; }

Symtab *&Scanner::css() { return css_; }

const Tytab *Scanner::gts() const { return gts_; }

Tytab *&Scanner::gts() { return gts_; }

const Tytab *Scanner::cts() const { return cts_; }

Tytab *&Scanner::cts() { return cts_; }

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
