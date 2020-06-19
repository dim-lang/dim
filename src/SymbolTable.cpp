// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Log.h"
#include "SymbolTable.h"

SymbolTable::SymbolTable()
    : global(new GlobalScope()), current(nullptr), depth_(0) {
  push(global);
}

SymbolTable::~SymbolTable() {
  pop();
  EX_ASSERT(depth_ == 0, "depth_ {} != 0", depth_);
  if (global) {
    delete global;
    global = nullptr;
  }
  current = nullptr;
}

void SymbolTable::push(Scope *sc) {
  EX_ASSERT(depth_ >= 0, "depth_ {} >= 0", depth_);
  current = sc;
  ++depth_;
}

void SymbolTable::pop() {
  EX_ASSERT(depth_ >= 0, "depth_ {} >= 0", depth_);
  if (depth_ > 0) {
    current = current->enclosingScope();
    --depth_;
  }
  EX_ASSERT(depth_ >= 0, "depth_ {} >= 0", depth_);
}

int SymbolTable::depth() const { return depth_; }
