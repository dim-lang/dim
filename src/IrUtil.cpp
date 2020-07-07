// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "IrUtil.h"
#include "Exception.h"

std::string IrUtil::prefix() { return "shp.ir."; }

static std::string toLLVMNameImpl(const std::string &name,
                                  const std::string prefix) {
  EX_ASSERT(name.length() > 0, "name {} length {} <= 0", name, name.length());
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
  EX_ASSERT(name.length() > prefix.length(),
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
  return toLLVMNameImpl(name, IrUtil::prefix());
}

std::string IrUtil::fromLLVMName(const std::string &name) {
  return fromLLVMNameImpl(name, IrUtil::prefix());
}

