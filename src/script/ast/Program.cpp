// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/Program.h"
#include "script/Util.h"

namespace fastype {

Program::Program(std::shared_ptr<Ast> node) : statementList_(node) {}

Program::~Program() {
  F_INFO("Destructor this:{}, {}", (void *)this, toString());
}

std::shared_ptr<Ast> Program::statementList() const { return statementList_; }

std::string Program::toString() const {
  std::string _1 = statementList_ ? statementList_->toString() : "null";
  return fmt::format("[ @Program statementList_:{} ]", _1);
}

Ast::AstType Program::type() const { return Ast::AstType::PROGRAM; }

} // namespace fastype
