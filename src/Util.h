// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "LinkedHashMap.h"
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

#define del(x)                                                                 \
  do {                                                                         \
    delete x;                                                                  \
    x = nullptr;                                                               \
  } while (0)

#define del_array(x)                                                           \
  do {                                                                         \
    for (auto i = (x).begin(); i != (x).end(); i++) {                          \
      delete *i;                                                               \
      *i = nullptr;                                                            \
    }                                                                          \
    (x).clear();                                                               \
  } while (0)

#define del_map_first(x)                                                       \
  do {                                                                         \
    for (auto i = (x).begin(); i != (x).end(); i++) {                          \
      delete i->first;                                                         \
      i->first = nullptr;                                                      \
    }                                                                          \
    (x).clear();                                                               \
  } while (0)

#define del_map_second(x)                                                      \
  do {                                                                         \
    for (auto i = (x).begin(); i != (x).end(); i++) {                          \
      delete i->second;                                                        \
      i->second = nullptr;                                                     \
    }                                                                          \
    (x).clear();                                                               \
  } while (0)

#define del_map(x)                                                             \
  do {                                                                         \
    for (auto i = (x).begin(); i != (x).end(); i++) {                          \
      delete i->first;                                                         \
      delete i->second;                                                        \
      i->first = nullptr;                                                      \
      i->second = nullptr;                                                     \
    }                                                                          \
    (x).clear();                                                               \
  } while (0)
