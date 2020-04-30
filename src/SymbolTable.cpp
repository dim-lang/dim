// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "SymbolTable.h"
#include "Log.h"

SymbolTable::SymbolTable()
    : gss_(new GlobalSymtab()), css_(nullptr), symstk_(0),
      gts_(new GlobalTytab()), cts_(nullptr), tystk_(0) {
  pushSymbol(gss_);
  pushType(gts_);
}

SymbolTable::~SymbolTable() {
  popSymbol();
  popType();
  LOG_ASSERT(symstk_ == 0, "symstk_ {} != 0", symstk_);
  LOG_ASSERT(tystk_ == 0, "tystk_ {} != 0", tystk_);
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
}

Symtab *&SymbolTable::gss() { return gss_; }

const Symtab *SymbolTable::gss() const { return gss_; }

Symtab *&SymbolTable::css() { return css_; }

const Symtab *SymbolTable::css() const { return css_; }

int SymbolTable::symstk() const { return symstk_; }

Tytab *&SymbolTable::gts() { return gts_; }

const Tytab *SymbolTable::gts() const { return gts_; }

Tytab *&SymbolTable::cts() { return cts_; }

const Tytab *SymbolTable::cts() const { return cts_; }

int SymbolTable::tystk() const { return tystk_; }

void SymbolTable::pushSymbol(Symtab *st) {
  LOG_ASSERT(symstk_ >= 0, "symstk_ {} >= 0", symstk_);
  css_ = st;
  ++symstk_;
}

void SymbolTable::popSymbol() {
  LOG_ASSERT(symstk_ >= 0, "symstk_ {} >= 0", symstk_);
  if (symstk_ > 0) {
    css_ = css_->enclosingScope();
    --symstk_;
  }
  LOG_ASSERT(symstk_ >= 0, "symstk_ {} >= 0", symstk_);
}

void SymbolTable::pushType(Tytab *tt) {
  LOG_ASSERT(tystk_ >= 0, "tystk_ {} >= 0", tystk_);
  cts_ = tt;
  ++tystk_;
}

void SymbolTable::popType() {
  LOG_ASSERT(tystk_ >= 0, "tystk_ {} >= 0", tystk_);
  if (tystk_ > 0) {
    cts_ = cts_->enclosingScope();
    --tystk_;
  }
  LOG_ASSERT(tystk_ >= 0, "tystk_ {} >= 0", tystk_);
}

void SymbolTable::resetSymbolStack() {
  css_ = nullptr;
  symstk_ = 0;
}

void SymbolTable::resetTypeStack() {
  cts_ = nullptr;
  tystk_ = 0;
}
