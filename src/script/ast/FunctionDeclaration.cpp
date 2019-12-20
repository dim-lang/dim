// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/FunctionDeclaration.h"

namespace fastype {

FunctionDeclaration::FunctionDeclaration(
    std::shared_ptr<Ast> functionId,
    const std::vector<std::shared_ptr<Ast>> &varList,
    std::shared_ptr<Ast> compoundStatement)
    : functionId_(functionId), varList_(varList),
      compoundStatement_(compoundStatement) {}

std::shared_ptr<Ast> FunctionDeclaration::functionId() const {
  return functionId_;
}

const std::vector<std::shared_ptr<Ast>> &FunctionDeclaration::varList() const {
  return varList_;
}

std::shared_ptr<Ast> FunctionDeclaration::compoundStatement() const {
  return compoundStatement_;
}

std::string FunctionDeclaration::toString() const {
  std::stringstream ss;
  ss << fmt::format("[ @FunctionDeclaration id:{}, varSize:{}, ",
                    functionId_->toString(), varList_.size());
  for (int i = 0; i < (int)varList_.size(); i++) {
    ss << fmt::format("{}:{}, ", i, varList_[i]->toString());
  }
  std::string _1 = compoundStatement_ ? compoundStatement_->toString() : "null";
  ss << _1 << " ]";
  return ss.str();
}

int FunctionDeclaration::type() const { return F_TYPE_FUNCTION_DECLARATION; }

} // namespace fastype
