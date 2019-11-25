// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/Empty.h"
#include <fmt/format.h>

namespace fastype {

Empty::Empty() {}

std::string Empty::toString() const { return fmt::format("[ @Empty ]"); }

Ast::AstType Empty::type() const { return Ast::AstType::EMPTY; }

} // namespace fastype
