// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/Program.h"
#include "script/Util.h"

namespace fastype {

Program::Program(std::shared_ptr<Ast> node) : statementList_(node) {}

std::shared_ptr<Ast> Program::statementList() const { return statementList_; }

std::string Program::toString() const {
  std::string _1 = statementList_ ? statementList_->toString() : "null";
  return fmt::format("[ @Program statementList_:{} ]", _1);
}

int Program::type() const { return F_TYPE_PROGRAM; }

} // namespace fastype
