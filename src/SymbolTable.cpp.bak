// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "SymbolTable.h"
#include "Log.h"

SymbolTable::SymbolTable()
    : global(new GlobalScope()), current(nullptr), depth_(0) {
  push(global);
}

SymbolTable::~SymbolTable() {
  pop();
  LOG_ASSERT(depth_ == 0, "depth_ {} != 0", depth_);
  if (global) {
    delete global;
    global = nullptr;
  }
  current = nullptr;
}

void SymbolTable::push(Scope *sc) {
  LOG_ASSERT(depth_ >= 0, "depth_ {} >= 0", depth_);
  current = sc;
  ++depth_;
}

void SymbolTable::pop() {
  LOG_ASSERT(depth_ >= 0, "depth_ {} >= 0", depth_);
  if (depth_ > 0) {
    current = current->enclosingScope();
    --depth_;
  }
  LOG_ASSERT(depth_ >= 0, "depth_ {} >= 0", depth_);
}

int SymbolTable::depth() const { return depth_; }
