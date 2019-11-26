// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/CompoundStatement.h"
#include "Logging.h"
#include "exception/ScriptException.h"
#include "script/Interpreter.h"
#include <fmt/format.h>

namespace fastype {

CompoundStatement::CompoundStatement(std::vector<Ast *> children)
    : children_(children) {}

int CompoundStatement::size() const { return children_.size(); }

Ast *CompoundStatement::get(int pos) const {
  F_CHECK(pos >= 0, "pos {} >= 0", pos);
  F_CHECK(pos < children_.size(), "pos {} < children_#size {}", pos,
          children_.size());
  if (pos < 0) {
    F_THROW(ScriptException, "children index failure! pos {} < 0", pos);
  }
  if (pos >= children_.size()) {
    F_THROW(ScriptException,
            "children index failure! pos {} >= children_#size {}", pos,
            children_.size());
  }
  return children_[pos];
}

std::string CompoundStatement::toString() const {
  return fmt::format("[ @CompoundStatement children_#size:{} ]",
                     children_.size());
}

Ast::AstType CompoundStatement::type() const {
  return Ast::AstType::COMPOUND_STATEMENT;
}

} // namespace fastype
