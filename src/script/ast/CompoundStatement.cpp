// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/CompoundStatement.h"
#include "Logging.h"
#include "exception/ParseException.h"
#include "script/Interpreter.h"
#include <fmt/format.h>

namespace fastype {

CompoundStatement::CompoundStatement(std::vector<Ast *> statementList)
    : statementList_(statementList) {}

int CompoundStatement::size() const { return statementList_.size(); }

Ast *CompoundStatement::get(int pos) const {
  F_CHECK(pos >= 0, "pos {} >= 0", pos);
  F_CHECK(pos < statementList_.size(), "pos {} < statementList_#size {}", pos,
          statementList_.size());
  if (pos < 0) {
    F_THROW(ParseException, "statementList index failure! pos {} < 0", pos);
  }
  if (pos >= statementList_.size()) {
    F_THROW(ParseException,
            "statementList index failure! pos {} >= statementList_#size {}",
            pos, statementList_.size());
  }
  return statementList_[pos];
}

std::string CompoundStatement::toString() const {
  return fmt::format("[ @CompoundStatement statementList_#size:{} ]",
                     statementList_.size());
}

Ast::AstType CompoundStatement::type() const {
  return Ast::AstType::COMPOUND_STATEMENT;
}

} // namespace fastype
