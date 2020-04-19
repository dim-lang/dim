// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "llvm_IRBuilder_Util.h"
#include <unordered_map>

const char *getOpTmp(Op op) {
  static std::unordered_map<Op, const char *> opTmpMap = {
      {Op::Add, "addtmp"},
      {Op::Sub, "subtmp"},
      {Op::Mul, "multmp"},
      {Op::Div, "divtmp"},
  };
  return opTmpMap[op];
}

const char *getOpName(Op op) {
  static std::unordered_map<Op, const char *> opNameMap = {
      {Op::Add, "+"},
      {Op::Sub, "-"},
      {Op::Mul, "*"},
      {Op::Div, "/"},
  };
  return opNameMap[op];
}
