// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <unordered_map>
#include <vector>

class Ast;
class A_Exprs;
class A_Params;
class A_BlockStats;
class A_TopStats;
class Symbol;
class TypeSymbol;
class Scope;
class Visitor;

// cast pointer {

#define CAST_DEF(x)                                                            \
  template <typename T> T *cast(x *e) { return (T *)(e); }

CAST_DEF(Ast)
CAST_DEF(A_Exprs)
CAST_DEF(A_Params)
CAST_DEF(A_BlockStats)
CAST_DEF(A_TopStats)
CAST_DEF(Symbol)
CAST_DEF(TypeSymbol)
CAST_DEF(Scope)
CAST_DEF(Visitor)

#undef CAST_DEF

// cast pointer }

template <typename T> void del(T *&x) {
  delete x;
  x = nullptr;
}

template <typename T> void del(std::vector<T *> &x) {
  for (auto i = x.begin(); i != x.end(); i++) {
    delete *i;
    *i = nullptr;
  }
  x.clear();
}

template <typename T, typename U> void del(std::unordered_map<T *, U *> &x) {
  for (auto i = x.begin(); i != x.end(); i++) {
    delete i->first;
    delete i->second;
    i->first = nullptr;
    i->second = nullptr;
  }
  x.clear();
}

template <typename T, typename U> void del(std::unordered_map<T, U *> &x) {
  for (auto i = x.begin(); i != x.end(); i++) {
    delete i->second;
    i->second = nullptr;
  }
  x.clear();
}

template <typename T, typename U> void del(std::unordered_map<T *, U> &x) {
  for (auto i = x.begin(); i != x.end(); i++) {
    delete i->first;
    i->first = nullptr;
  }
  x.clear();
}
