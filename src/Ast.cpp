// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "config/Header.h"
#include <sstream>

AstExpressionList::AstExpressionList() {}

int AstExpressionList::type() const { return A_EXPRESSION_LIST; }

std::string AstExpressionList::toString() const {
  std::stringstream ss;
  ss << "[ @AstExpressionList size:" << expressions_.size();
  if (expressions_.empty()) {
    ss << " ]";
    return ss.str();
  }
  ss << ", ";
  for (int i = 0; i < (int)expressions_.size(); i++) {
    std::shared_ptr<AstExpression> expr = expressions_[i];
    ss << fmt::format("{}:{}", i, expr ? expr->toString() : "null");
    if (i < (int)expressions_.size() - 1) {
      ss << ", ";
    }
  }
  ss << " ]";
  return ss.str();
}

int AstExpressionList::size() const { return expressions_.size(); }

std::shared_ptr<AstExpression> AstExpressionList::get(int pos) const {
  return expressions_[pos];
}

void AstExpressionList::add(AstExpression *expression) {
  expressions_.push_back(std::shared_ptr<AstExpression>(expression));
}

AstStatementList::AstStatementList() {}

int AstStatementList::type() const { return A_STATEMENT_LIST; }

std::string AstStatementList::toString() const {
  std::stringstream ss;
  ss << "[ @AstStatementList size:" << statements_.size();
  if (statements_.empty()) {
    ss << " ]";
    return ss.str();
  }
  ss << ", ";
  for (int i = 0; i < (int)statements_.size(); i++) {
    std::shared_ptr<AstStatement> stmt = statements_[i];
    ss << fmt::format("{}:{}", i, stmt ? stmt->toString() : "null");
    if (i < (int)statements_.size() - 1) {
      ss << ", ";
    }
  }
  ss << " ]";
  return ss.str();
}

int AstStatementList::size() const { return statements_.size(); }

std::shared_ptr<AstStatement> AstStatementList::get(int pos) const {
  return statements_[pos];
}

void AstStatementList::add(AstStatement *statement) {
  statements_.push_back(std::shared_ptr<AstStatement>(statement));
}

AstIdentifierConstant::AstIdentifierConstant(const char *value)
    : value_(value) {}

int AstIdentifierConstant::type() const { return A_IDENTIFIER_CONSTANT; }

std::string AstIdentifierConstant::toString() const {
  return fmt::format("[ @AstIdentifierConstant value_:{} ]", value_);
}

const std::string &AstIdentifierConstant::value() const { return value_; }

AstI8Constant::AstI8Constant(const int8_t &value) : value_(value) {}

int AstI8Constant::type() const { return A_I8_CONSTANT; }

std::string AstI8Constant::toString() const {
  return fmt::format("[ @AstI8Constant value_:{} ]", (int)value_);
}

void AstI8Constant::reset(const int8_t &value) { value_ = value; }

const int8_t &AstI8Constant::value() const { return value_; }

AstUI8Constant::AstUI8Constant(const uint8_t &value) : value_(value) {}

int AstUI8Constant::type() const { return A_UI8_CONSTANT; }

std::string AstUI8Constant::toString() const {
  return fmt::format("[ @AstUI8Constant value_:{} ]", (int)value_);
}

void AstUI8Constant::reset(const uint8_t &value) { value_ = value; }

const uint8_t &AstUI8Constant::value() const { return value_; }

AstI16Constant::AstI16Constant(const int16_t &value) : value_(value) {}

int AstI16Constant::type() const { return A_I16_CONSTANT; }

std::string AstI16Constant::toString() const {
  return fmt::format("[ @AstI16Constant value_:{} ]", value_);
}

void AstI16Constant::reset(const int16_t &value) { value_ = value; }

const int16_t &AstI16Constant::value() const { return value_; }

AstUI16Constant::AstUI16Constant(const uint16_t &value) : value_(value) {}

int AstUI16Constant::type() const { return A_UI16_CONSTANT; }

std::string AstUI16Constant::toString() const {
  return fmt::format("[ @AstUI16Constant value_:{} ]", value_);
}

void AstUI16Constant::reset(const uint16_t &value) { value_ = value; }

const uint16_t &AstUI16Constant::value() const { return value_; }

AstI32Constant::AstI32Constant(const int32_t &value) : value_(value) {}

int AstI32Constant::type() const { return A_I32_CONSTANT; }

std::string AstI32Constant::toString() const {
  return fmt::format("[ @AstI32Constant value_:{} ]", value_);
}

void AstI32Constant::reset(const int32_t &value) { value_ = value; }

const int32_t &AstI32Constant::value() const { return value_; }

AstUI32Constant::AstUI32Constant(const uint32_t &value) : value_(value) {}

int AstUI32Constant::type() const { return A_UI32_CONSTANT; }

std::string AstUI32Constant::toString() const {
  return fmt::format("[ @AstUI32Constant value_:{} ]", value_);
}

void AstUI32Constant::reset(const uint32_t &value) { value_ = value; }

const uint32_t &AstUI32Constant::value() const { return value_; }

AstI64Constant::AstI64Constant(const int64_t &value) : value_(value) {}

int AstI64Constant::type() const { return A_I64_CONSTANT; }

std::string AstI64Constant::toString() const {
  return fmt::format("[ @AstI64Constant value_:{} ]", value_);
}

void AstI64Constant::reset(const int64_t &value) { value_ = value; }

const int64_t &AstI64Constant::value() const { return value_; }

AstUI64Constant::AstUI64Constant(const uint64_t &value) : value_(value) {}

int AstUI64Constant::type() const { return A_UI64_CONSTANT; }

std::string AstUI64Constant::toString() const {
  return fmt::format("[ @AstUI64Constant value_:{} ]", value_);
}

void AstUI64Constant::reset(const uint64_t &value) { value_ = value; }

const uint64_t &AstUI64Constant::value() const { return value_; }

AstF32Constant::AstF32Constant(const float &value) : value_(value) {}

int AstF32Constant::type() const { return A_F32_CONSTANT; }

std::string AstF32Constant::toString() const {
  return fmt::format("[ @AstF32Constant value_:{} ]", value_);
}

void AstF32Constant::reset(const float &value) { value_ = value; }

const float &AstF32Constant::value() const { return value_; }

AstF64Constant::AstF64Constant(const double &value) : value_(value) {}

int AstF64Constant::type() const { return A_F64_CONSTANT; }

std::string AstF64Constant::toString() const {
  return fmt::format("[ @AstF64Constant value_:{} ]", value_);
}

void AstF64Constant::reset(const double &value) { value_ = value; }

const double &AstF64Constant::value() const { return value_; }

AstStringConstant::AstStringConstant(const char *value) : value_(value) {}

int AstStringConstant::type() const { return A_STRING_CONSTANT; }

std::string AstStringConstant::toString() const {
  return fmt::format("[ @AstStringConstant value_:{} ]", value_);
}

void AstStringConstant::reset(const std::string &value) { value_ = value; }

const std::string &AstStringConstant::value() const { return value_; }

AstBooleanConstant::AstBooleanConstant(const bool &value) : value_(value) {}

int AstBooleanConstant::type() const { return A_BOOLEAN_CONSTANT; }

std::string AstBooleanConstant::toString() const {
  return fmt::format("[ @AstBooleanConstant value_:{} ]", value_);
}

void AstBooleanConstant::reset(const bool &value) { value_ = value; }

const bool &AstBooleanConstant::value() const { return value_; }

AstFunctionCallExpression::AstFunctionCallExpression(
    const char *identifier, AstExpressionList *argumentList)
    : identifier_(identifier),
      argumentList_(std::shared_ptr<AstExpressionList>(argumentList)) {}

int AstFunctionCallExpression::type() const {
  return A_FUNCTION_CALL_EXPRESSION;
}

std::string AstFunctionCallExpression::toString() const {
  return fmt::format(
      "[ @AstFunctionCallExpression identifier_:{}, argumentList_:{} ]",
      identifier_, argumentList_ ? argumentList_->toString() : "null");
}

const std::string &AstFunctionCallExpression::identifier() const {
  return identifier_;
}

std::shared_ptr<AstExpressionList>
AstFunctionCallExpression::argumentList() const {
  return argumentList_;
}

AstUnaryExpression::AstUnaryExpression(int token, AstExpression *expression)
    : token_(token), expression_(std::shared_ptr<AstExpression>(expression)) {}

int AstUnaryExpression::type() const { return A_UNARY_OPERATION_EXPRESSION; }

std::string AstUnaryExpression::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstUnaryExpression token_:{}, expression_:{} ]",
                     token_, exprStr);
}

int AstUnaryExpression::token() const { return token_; }

std::shared_ptr<AstExpression> AstUnaryExpression::expression() const {
  return expression_;
}

AstBinaryExpression::AstBinaryExpression(AstExpression *left, int token,
                                         AstExpression *right)
    : left_(std::shared_ptr<AstExpression>(left)), token_(token),
      right_(std::shared_ptr<AstExpression>(right)) {}

int AstBinaryExpression::type() const { return A_BINARY_OPERATION_EXPRESSION; }

std::string AstBinaryExpression::toString() const {
  std::string leftStr = left_ ? left_->toString() : "null";
  std::string rightStr = right_ ? right_->toString() : "null";
  return fmt::format("[ @AstBinaryExpression left_:{}, token_:{}, right_:{} ]",
                     leftStr, token_, rightStr);
}

std::shared_ptr<AstExpression> AstBinaryExpression::left() const {
  return left_;
}

int AstBinaryExpression::token() const { return token_; }

std::shared_ptr<AstExpression> AstBinaryExpression::right() const {
  return right_;
}

AstConditionalExpression::AstConditionalExpression(
    AstExpression *condExpression, AstExpression *ifExpression,
    AstExpression *elseExpression)
    : condExpression_(std::shared_ptr<AstExpression>(condExpression)),
      ifExpression_(std::shared_ptr<AstExpression>(ifExpression)),
      elseExpression_(std::shared_ptr<AstExpression>(elseExpression)) {}

int AstConditionalExpression::type() const { return A_CONDITIONAL_EXPRESSION; }

std::string AstConditionalExpression::toString() const {
  std::string condStr = condExpression_ ? condExpression_->toString() : "null";
  std::string ifStr = ifExpression_ ? ifExpression_->toString() : "null";
  std::string elseStr = elseExpression_ ? elseExpression_->toString() : "null";
  return fmt::format("[ @AstConditionalExpression condExpression_:{}, "
                     "ifExpression_:{}, elseExpression_:{} ]",
                     condStr, ifStr, elseStr);
}

std::shared_ptr<AstExpression>
AstConditionalExpression::condExpression() const {
  return condExpression_;
}

std::shared_ptr<AstExpression> AstConditionalExpression::ifExpression() const {
  return ifExpression_;
}

std::shared_ptr<AstExpression>
AstConditionalExpression::elseExpression() const {
  return elseExpression_;
}

AstAssignmentExpression::AstAssignmentExpression(AstExpressionList *left,
                                                 int token,
                                                 AstExpressionList *right)
    : left_(std::shared_ptr<AstExpressionList>(left)), token_(token),
      right_(std::shared_ptr<AstExpressionList>(right)) {}

int AstAssignmentExpression::type() const {
  return A_BINARY_OPERATION_EXPRESSION;
}

std::string AstAssignmentExpression::toString() const {
  return fmt::format(
      "[ @AstAssignmentExpression left_:{}, token_:{}, right_:{} ]",
      left_->toString(), token_, right_->toString());
}

std::shared_ptr<AstExpressionList> AstAssignmentExpression::left() const {
  return left_;
}

int AstAssignmentExpression::token() const { return token_; }

std::shared_ptr<AstExpressionList> AstAssignmentExpression::right() const {
  return right_;
}

AstExpressionStatement::AstExpressionStatement(AstExpression *expression)
    : expression_(std::shared_ptr<AstExpression>(expression)) {}

int AstExpressionStatement::type() const { return A_EXPRESSION_STATEMENT; }

std::string AstExpressionStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstExpressionStatement expression_:{} ]", exprStr);
}

std::shared_ptr<AstExpression> AstExpressionStatement::expression() const {
  return expression_;
}

AstCompoundStatement::AstCompoundStatement(AstStatementList *statementList)
    : statementList_(std::shared_ptr<AstStatementList>(statementList)) {}

int AstCompoundStatement::type() const { return A_COMPOUND_STATEMENT; }

std::string AstCompoundStatement::toString() const {
  return fmt::format("[ @AstCompoundStatement statementList_:{} ]",
                     statementList_ ? statementList_->toString() : "null");
}

std::shared_ptr<AstStatementList> AstCompoundStatement::statementList() const {
  return statementList_;
}

AstIfStatement::AstIfStatement(AstExpression *condExpression,
                               AstStatement *ifStatement,
                               AstStatement *elseStatement)
    : condExpression_(std::shared_ptr<AstExpression>(condExpression)),
      ifStatement_(std::shared_ptr<AstStatement>(ifStatement)),
      elseStatement_(std::shared_ptr<AstStatement>(elseStatement)) {}

int AstIfStatement::type() const { return A_IF_STATEMENT; }

std::string AstIfStatement::toString() const {
  std::string condStr = condExpression_ ? condExpression_->toString() : "null";
  std::string ifStr = ifStatement_ ? ifStatement_->toString() : "null";
  std::string elseStr = elseStatement_ ? elseStatement_->toString() : "null";
  return fmt::format("[ @AstIfStatement condExpression_:{}, ifStatement_:{}, "
                     "elseStatement_:{} ]",
                     condStr, ifStr, elseStr);
}

std::shared_ptr<AstExpression> AstIfStatement::condExpression() const {
  return condExpression_;
}

std::shared_ptr<AstStatement> AstIfStatement::ifStatement() const {
  return ifStatement_;
}

std::shared_ptr<AstStatement> AstIfStatement::elseStatement() const {
  return elseStatement_;
}

AstWhileStatement::AstWhileStatement(AstExpression *condExpression,
                                     AstStatement *statement)
    : condExpression_(std::shared_ptr<AstExpression>(condExpression)),
      statement_(std::shared_ptr<AstStatement>(statement)) {}

int AstWhileStatement::type() const { return A_WHILE_STATEMENT; }

std::string AstWhileStatement::toString() const {
  std::string condStr = condExpression_ ? condExpression_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format("[ @AstWhileStatement condExpression_:{}, statement_:{} ]",
                     condStr, stmtStr);
}

std::shared_ptr<AstExpression> AstWhileStatement::condExpression() const {
  return condExpression_;
}

std::shared_ptr<AstStatement> AstWhileStatement::statement() const {
  return statement_;
}

AstForStatement::AstForStatement(AstStatement *initStatement,
                                 AstStatement *condStatement,
                                 AstExpression *postExpression,
                                 AstStatement *statement)
    : initStatement_(std::shared_ptr<AstStatement>(initStatement)),
      condStatement_(std::shared_ptr<AstStatement>(condStatement)),
      postExpression_(std::shared_ptr<AstExpression>(postExpression)),
      statement_(std::shared_ptr<AstStatement>(statement)) {}

int AstForStatement::type() const { return A_FOR_STATEMENT; }

std::string AstForStatement::toString() const {
  std::string initStr = initStatement_ ? initStatement_->toString() : "null";
  std::string condStr = condStatement_ ? condStatement_->toString() : "null";
  std::string postStr = postExpression_ ? postExpression_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format("[ @AstForStatement initStatement_:{}, condStatement_:{}, "
                     "postExpression_:{}, statement_:{} ]",
                     initStr, condStr, postStr, stmtStr);
}

std::shared_ptr<AstStatement> AstForStatement::initStatement() const {
  return initStatement_;
}

std::shared_ptr<AstStatement> AstForStatement::condStatement() const {
  return condStatement_;
}

std::shared_ptr<AstExpression> AstForStatement::postExpression() const {
  return postExpression_;
}

std::shared_ptr<AstStatement> AstForStatement::statement() const {
  return statement_;
}

AstContinueStatement::AstContinueStatement() {}

int AstContinueStatement::type() const { return A_CONTINUE_STATEMENT; }

std::string AstContinueStatement::toString() const {
  return fmt::format("[ @AstContinueStatement ]");
}

AstBreakStatement::AstBreakStatement() {}

int AstBreakStatement::type() const { return A_BREAK_STATEMENT; }

std::string AstBreakStatement::toString() const {
  return fmt::format("[ @AstBreakStatement ]");
}

AstReturnStatement::AstReturnStatement(AstExpression *expression)
    : expression_(std::shared_ptr<AstExpression>(expression)) {}

int AstReturnStatement::type() const { return A_RETURN_STATEMENT; }

std::string AstReturnStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstReturnStatement expression_:{} ]", exprStr);
}

std::shared_ptr<AstExpression> AstReturnStatement::expression() const {
  return expression_;
}

AstVariableDeclaration::AstVariableDeclaration(
    AstExpressionList *identifierList, AstExpressionList *expressionList)
    : identifierList_(std::shared_ptr<AstExpressionList>(identifierList)),
      expressionList_(std::shared_ptr<AstExpressionList>(expressionList)) {}

int AstVariableDeclaration::type() const { return A_VARIABLE_DECLARATION; }

std::string AstVariableDeclaration::toString() const {
  return fmt::format(
      "[ @AstVariableDeclaration identifierList_:{}, expressionList_:{} ]",
      identifierList_ ? identifierList_->toString() : "null",
      expressionList_ ? expressionList_->toString() : "null");
}

std::shared_ptr<AstExpressionList>
AstVariableDeclaration::identifierList() const {
  return identifierList_;
}

std::shared_ptr<AstExpressionList>
AstVariableDeclaration::expressionList() const {
  return expressionList_;
}

AstFunctionDeclaration::AstFunctionDeclaration(const char *identifier,
                                               AstExpressionList *argumentList,
                                               AstStatementList *statementList)
    : identifier_(identifier),
      argumentList_(std::shared_ptr<AstExpressionList>(argumentList)),
      statementList_(std::shared_ptr<AstStatementList>(statementList)) {}

AstFunctionDeclaration::AstFunctionDeclaration(
    const char *identifier, AstExpressionList *argumentList,
    std::shared_ptr<AstStatementList> statementList)
    : identifier_(identifier),
      argumentList_(std::shared_ptr<AstExpressionList>(argumentList)),
      statementList_(statementList) {}

int AstFunctionDeclaration::type() const { return A_FUNCTION_DECLARATION; }

std::string AstFunctionDeclaration::toString() const {
  return fmt::format("[ @AstFunctionDeclaration identifier_:{}, "
                     "argumentList_:{}, statementList_:{} ]",
                     identifier_,
                     argumentList_ ? argumentList_->toString() : "null",
                     statementList_ ? statementList_->toString() : "null");
}

const std::string &AstFunctionDeclaration::identifier() const {
  return identifier_;
}

std::shared_ptr<AstExpressionList>
AstFunctionDeclaration::argumentList() const {
  return argumentList_;
}

std::shared_ptr<AstStatementList>
AstFunctionDeclaration::statementList() const {
  return statementList_;
}
