// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/EmptyStatement.h"

namespace fastype {

EmptyStatement::EmptyStatement() {}

std::string EmptyStatement::toString() const {
  return fmt::format("[ @EmptyStatement ]");
}

int EmptyStatement::type() const { return F_TYPE_EMPTY_STATEMENT; }

} // namespace fastype
