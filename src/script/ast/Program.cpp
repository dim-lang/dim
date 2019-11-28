// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/Program.h"
#include "script/ast/Util.h"
#include <fmt/format.h>

namespace fastype {

Program::Program(Sptr<Ast> node) : statementList_(node) {}

Sptr<Ast> Program::statementList() const { return statementList_; }

std::string Program::toString() const {
  return fmt::format("[ @Program statementList_:{} ]",
                     statementList_->toString());
}

Ast::AstType Program::type() const { return Ast::AstType::PROGRAM; }

} // namespace fastype