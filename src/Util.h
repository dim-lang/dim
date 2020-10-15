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

#define cast(x, y) ((x *)(y))

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
