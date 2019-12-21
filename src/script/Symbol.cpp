// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Symbol.h"

namespace fastype {

const std::vector<int> &Symbol::symbolTypes() {
  static const std::vector<int> types = {};
  return types;
}

const std::shared_ptr<Symbol> Symbol::S_NIL(nullptr);

} // namespace fastype
