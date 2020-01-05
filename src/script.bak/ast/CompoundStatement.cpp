// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/CompoundStatement.h"
#include "Logging.h"
#include "exception/ScriptException.h"
#include "script/Interpreter.h"
#include "script/Util.h"

namespace fastype {

CompoundStatement::CompoundStatement(std::shared_ptr<Ast> node)
    : statementList_(node) {}

std::shared_ptr<Ast> CompoundStatement::statementList() const {
  return statementList_;
}

std::string CompoundStatement::toString() const {
  std::string _1 = statementList_ ? statementList_->toString() : "null";
  return fmt::format("[ @CompoundStatement statementList_:{} ]", _1);
}

int CompoundStatement::type() const { return F_TYPE_COMPOUND_STATEMENT; }

} // namespace fastype
