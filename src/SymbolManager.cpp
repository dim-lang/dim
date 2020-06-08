// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "SymbolManager.h"
#include "Log.h"

SymbolManager::SymbolManager()
    : global(new GlobalScope()), current(nullptr), depth_(0) {
  push(global);
}

SymbolManager::~SymbolManager() {
  pop();
  LOG_ASSERT(depth_ == 0, "depth_ {} != 0", depth_);
  if (global) {
    delete global;
    global = nullptr;
  }
  current = nullptr;
}

void SymbolManager::push(Symtab *st) {
  LOG_ASSERT(depth_ >= 0, "depth_ {} >= 0", depth_);
  current = st;
  ++depth_;
}

void SymbolManager::pop() {
  LOG_ASSERT(depth_ >= 0, "depth_ {} >= 0", depth_);
  if (depth_ > 0) {
    current = current->enclosingScope();
    --depth_;
  }
  LOG_ASSERT(depth_ >= 0, "depth_ {} >= 0", depth_);
}

int SymbolManager::depth() const { return depth_; }

