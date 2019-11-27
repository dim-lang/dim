// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/CompoundStatement.h"
#include "Logging.h"
#include "exception/ScriptException.h"
#include "script/Interpreter.h"
#include "script/ast/Util.h"
#include <fmt/format.h>

namespace fastype {

CompoundStatement::CompoundStatement(Ast *node) : statementList_(node) {}

Ast *CompoundStatement::statementList() const { return statementList_; }

std::string CompoundStatement::toString() const {
  return ast::AstVectortoString(children_, "CompoundStatement");
}

Ast::AstType CompoundStatement::type() const {
  return Ast::AstType::COMPOUND_STATEMENT;
}

} // namespace fastype
