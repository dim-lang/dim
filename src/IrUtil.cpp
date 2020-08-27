// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrUtil.h"
#include "Ir.h"
#include "Log.h"
#include "NameGenerator.h"
#include "boost/preprocessor/stringize.hpp"
#include "llvm/Support/raw_ostream.h"
#include <system_error>

#define PREFIX std::string("shp.ir.")

std::string IrUtil::namegen(const std::string &name) {
  static NameGenerator nameGenerator(PREFIX, ".", ".");
  return nameGenerator.generate(name);
}

static std::string toLLVMNameImpl(const std::string &name,
                                  const std::string prefix) {
  LOG_ASSERT(name.length() > 0, "name {} length {} <= 0", name, name.length());
  std::string tmp(name);
  for (int i = 0; i < (int)tmp.length(); i++) {
    if (tmp[i] == '_') {
      tmp[i] = '.';
    }
  }
  return prefix + tmp;
}

static std::string fromLLVMNameImpl(const std::string &name,
                                    const std::string prefix) {
  LOG_ASSERT(name.length() > prefix.length(),
             "name {} length {} <= prefix {} length {}", name, name.length(),
             prefix, prefix.length());
  std::string tmp(
      name.substr(prefix.length(), name.length() - prefix.length()));
  for (int i = 0; i < (int)tmp.length(); i++) {
    if (tmp[i] == '.') {
      tmp[i] = '_';
    }
  }
  return tmp;
}

std::string IrUtil::toLLVMName(const std::string &name) {
  return toLLVMNameImpl(name, PREFIX);
}

std::string IrUtil::fromLLVMName(const std::string &name) {
  return fromLLVMNameImpl(name, PREFIX);
}
