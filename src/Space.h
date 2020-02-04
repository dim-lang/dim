// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <string>
#include <unordered_map>
#include <utility>

#define S_UNKNOWN 0
#define S_I8 1
#define S_UI8 2
#define S_I16 3
#define S_UI16 4
#define S_I32 5
#define S_UI32 6
#define S_I64 7
#define S_UI64 8
#define S_F32 9
#define S_F64 10
#define S_STRING 11
#define S_BOOLEAN 12

class Ast;

class Space {
public:
  virtual ~Space() = 0;
  virtual void *visit(Ast *node) = 0;
  virtual void store(const std::string &name, int type, void *value);
  virtual std::pair<int, void *> query(const std::string &name);

protected:
  std::unordered_map<std::string, std::pair<int, void *>> memoryTable_;
};

class GlobalSpace : public Space {
public:
};
