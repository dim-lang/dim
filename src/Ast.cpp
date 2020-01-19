// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Ast.h"
#include "config/Header.h"

AstIdentifierConstant::AstIdentifierConstant(const char *value)
    : value_(value) {}

AstIdentifierConstant::~AstIdentifierConstant() {}

int AstIdentifierConstant::type() const { return FA_IDENTIFIER_CONSTANT; }

std::string AstIdentifierConstant::toString() const {
  return fmt::format("[ @AstIdentifierConstant value_:{} ]", value_);
}

const std::string &AstIdentifierConstant::value() const { return value_; }

AstIntegerConstant::AstIntegerConstant(const int64_t &value) : value_(value) {}

AstIntegerConstant::~AstIntegerConstant() {}

int AstIntegerConstant::type() const { return FA_INTEGER_CONSTANT; }

std::string AstIntegerConstant::toString() const {
  return fmt::format("[ @AstIntegerConstant value_:{} ]", value_);
}

const int64_t &AstIntegerConstant::value() const { return value_; }

AstDoubleConstant::AstDoubleConstant(const double &value) : value_(value) {}

AstDoubleConstant::~AstDoubleConstant() {}

int AstDoubleConstant::type() const { return FA_DOUBLE_CONSTANT; }

std::string AstDoubleConstant::toString() const {
  return fmt::format("[ @AstDoubleConstant value_:{} ]", value_);
}

const double &AstDoubleConstant::value() const { return value_; }

AstStringConstant::AstStringConstant(const char *value) : value_(value) {}

AstStringConstant::~AstStringConstant() {}

int AstStringConstant::type() const { return FA_STRING_CONSTANT; }

std::string AstStringConstant::toString() const {
  return fmt::format("[ @AstStringConstant value_:{} ]", value_);
}

const std::string &AstStringConstant::value() const { return value_; }

AstFunctionCallExpression::AstFunctionCallExpression(
    const char *identifier, AstExpressionList *argumentList)
    : identifier_(identifier), argumentList_(argumentList) {}

AstFunctionCallExpression::~AstFunctionCallExpression() {
  if (argumentList_) {
    for (int i = 0; i < argumentList_->size(); i++) {
      delete (*argumentList_)[i];
      (*argumentList_)[i] = nullptr;
    }
    delete argumentList_;
    argumentList_ = nullptr;
  }
}

int AstFunctionCallExpression::type() const {
  return FA_FUNCTION_CALL_EXPRESSION;
}

std::string AstFunctionCallExpression::toString() const {
  return fmt::format(
      "[ @AstFunctionCallExpression identifier_:{}, argumentList_#size:{} ]",
      identifier_, argumentList_ ? (int)argumentList_->size() : 0);
}

const std::string &AstFunctionCallExpression::identifier() const {
  return identifier_;
}

AstExpressionList *AstFunctionCallExpression::argumentList() const {
  return argumentList_;
}

AstUnaryExpression::AstUnaryExpression(int token, AstExpression *expression)
    : token_(token), expression_(expression) {}

AstUnaryExpression::~AstUnaryExpression() {
  delete expression_;
  expression_ = nullptr;
}

int AstUnaryExpression::type() const { return FA_UNARY_OPERATION_EXPRESSION; }

std::string AstUnaryExpression::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstUnaryExpression token_:{}, expression_:{} ]",
                     token_, exprStr);
}

int AstUnaryExpression::token() const { return token_; }

AstExpression *AstUnaryExpression::expression() const { return expression_; }

AstBinaryExpression::AstBinaryExpression(AstExpression *left, int token,
                                         AstExpression *right)
    : left_(left), token_(token), right_(right) {}

AstBinaryExpression::~AstBinaryExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

int AstBinaryExpression::type() const { return FA_BINARY_OPERATION_EXPRESSION; }

std::string AstBinaryExpression::toString() const {
  std::string leftStr = left_ ? left_->toString() : "null";
  std::string rightStr = right_ ? right_->toString() : "null";
  return fmt::format("[ @AstBinaryExpression left_:{}, token_:{}, right_:{} ]",
                     leftStr, token_, rightStr);
}

AstExpression *AstBinaryExpression::left() const { return left_; }

int AstBinaryExpression::token() const { return token_; }

AstExpression *AstBinaryExpression::right() const { return right_; }

AstConditionalExpression::AstConditionalExpression(
    AstExpression *conditionExpression, AstExpression *ifExpression,
    AstExpression *elseExpression)
    : conditionExpression_(conditionExpression), ifExpression_(ifExpression),
      elseExpression_(elseExpression) {}

AstConditionalExpression::~AstConditionalExpression() {
  delete conditionExpression_;
  conditionExpression_ = nullptr;
  delete ifExpression_;
  ifExpression_ = nullptr;
  delete elseExpression_;
  elseExpression_ = nullptr;
}

int AstConditionalExpression::type() const { return FA_CONDITIONAL_EXPRESSION; }

std::string AstConditionalExpression::toString() const {
  std::string condStr =
      conditionExpression_ ? conditionExpression_->toString() : "null";
  std::string ifStr = ifExpression_ ? ifExpression_->toString() : "null";
  std::string elseStr = elseExpression_ ? elseExpression_->toString() : "null";
  return fmt::format("[ @AstConditionalExpression conditionExpression_:{}, "
                     "ifExpression_:{}, elseExpression_:{} ]",
                     condStr, ifStr, elseStr);
}

AstExpression *AstConditionalExpression::conditionExpression() const {
  return conditionExpression_;
}

AstExpression *AstConditionalExpression::ifExpression() const {
  return ifExpression_;
}

AstExpression *AstConditionalExpression::elseExpression() const {
  return elseExpression_;
}

AstAssignmentExpression::AstAssignmentExpression(AstExpression *left, int token,
                                                 AstExpression *right)
    : left_(left), token_(token), right_(right) {}

AstAssignmentExpression::~AstAssignmentExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

int AstAssignmentExpression::type() const {
  return FA_BINARY_OPERATION_EXPRESSION;
}

std::string AstAssignmentExpression::toString() const {
  std::string leftStr = left_ ? left_->toString() : "null";
  std::string rightStr = right_ ? right_->toString() : "null";
  return fmt::format(
      "[ @AstAssignmentExpression left_:{}, token_:{}, right_:{} ]", leftStr,
      token_, rightStr);
}

AstExpression *AstAssignmentExpression::left() const { return left_; }

int AstAssignmentExpression::token() const { return token_; }

AstExpression *AstAssignmentExpression::right() const { return right_; }

AstExpressionStatement::AstExpressionStatement(AstExpression *expression)
    : expression_(expression) {}

AstExpressionStatement::~AstExpressionStatement() {
  delete expression_;
  expression_ = nullptr;
}

int AstExpressionStatement::type() const { return FA_EXPRESSION_STATEMENT; }

std::string AstExpressionStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstExpressionStatement expression_:{} ]", exprStr);
}

AstExpression *AstExpressionStatement::expression() const {
  return expression_;
}

AstCompoundStatement::AstCompoundStatement(
    const AstStatementList &statementList)
    : statementList_(statementList) {}

AstCompoundStatement::~AstCompoundStatement() {
  for (int i = 0; i < statementList_.size(); i++) {
    delete statementList_[i];
    statementList_[i] = nullptr;
  }
}

int AstCompoundStatement::type() const { return FA_COMPOUND_STATEMENT; }

std::string AstCompoundStatement::toString() const {
  return fmt::format("[ @AstCompoundStatement statementList_#size:{} ]",
                     statementList_.size());
}

const AstStatementList &AstCompoundStatement::statementList() const {
  return statementList_;
}

AstIfStatement::AstIfStatement(AstExpression *expression,
                               AstStatement *ifStatement,
                               AstStatement *elseStatement)
    : expression_(expression), ifStatement_(ifStatement),
      elseStatement_(elseStatement) {}

AstIfStatement::~AstIfStatement() {
  delete expression_;
  expression_ = nullptr;
  delete ifStatement_;
  ifStatement_ = nullptr;
  delete elseStatement_;
  elseStatement_ = nullptr;
}

int AstIfStatement::type() const { return FA_IF_STATEMENT; }

std::string AstIfStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  std::string ifStr = ifStatement_ ? ifStatement_->toString() : "null";
  std::string elseStr = elseStatement_ ? elseStatement_->toString() : "null";
  return fmt::format(
      "[ @AstIfStatement expression_:{}, ifStatement_:{}, elseStatement_:{} ]",
      exprStr, ifStr, elseStr);
}

AstExpression *AstIfStatement::expression() const { return expression_; }

AstStatement *AstIfStatement::ifStatement() const { return ifStatement_; }

AstStatement *AstIfStatement::elseStatement() const { return elseStatement_; }

AstWhileStatement::AstWhileStatement(AstExpression *expression,
                                     AstStatement *statement)
    : expression_(expression), statement_(statement) {}

AstWhileStatement::~AstWhileStatement() {
  delete expression_;
  expression_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

int AstWhileStatement::type() const { return FA_WHILE_STATEMENT; }

std::string AstWhileStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format("[ @AstWhileStatement expression_:{}, statement_:{} ]",
                     exprStr, stmtStr);
}

AstExpression *AstWhileStatement::expression() const { return expression_; }

AstStatement *AstWhileStatement::statement() const { return statement_; }

AstForStatement::AstForStatement(AstExpression *initExpression,
                                 AstExpression *conditionExpression,
                                 AstExpression *postExpression,
                                 AstStatement *statement)
    : initExpression_(initExpression),
      conditionExpression_(conditionExpression),
      postExpression_(postExpression), statement_(statement) {}

AstForStatement::~AstForStatement() {
  delete initExpression_;
  initExpression_ = nullptr;
  delete conditionExpression_;
  conditionExpression_ = nullptr;
  delete postExpression_;
  postExpression_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

int AstForStatement::type() const { return FA_FOR_STATEMENT; }

std::string AstForStatement::toString() const {
  std::string initStr = initExpression_ ? initExpression_->toString() : "null";
  std::string condStr =
      conditionExpression_ ? conditionExpression_->toString() : "null";
  std::string postStr = postExpression_ ? postExpression_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format(
      "[ @AstForStatement initExpression_:{}, conditionExpression_:{}, "
      "postExpression_:{}, statement_:{} ]",
      initStr, condStr, postStr, stmtStr);
}

AstExpression *AstForStatement::initExpression() const {
  return initExpression_;
}

AstExpression *AstForStatement::conditionExpression() const {
  return conditionExpression_;
}

AstExpression *AstForStatement::postExpression() const {
  return postExpression_;
}

AstStatement *AstForStatement::statement() const { return statement_; }

AstContinueStatement::AstContinueStatement() {}

AstContinueStatement::~AstContinueStatement() {}

int AstContinueStatement::type() const { return FA_CONTINUE_STATEMENT; }

std::string AstContinueStatement::toString() const {
  return fmt::format("[ @AstContinueStatement ]");
}

AstBreakStatement::AstBreakStatement() {}

AstBreakStatement::~AstBreakStatement() {}

int AstBreakStatement::type() const { return FA_BREAK_STATEMENT; }

std::string AstBreakStatement::toString() const {
  return fmt::format("[ @AstBreakStatement ]");
}

AstReturnStatement::AstReturnStatement(AstExpression *expression)
    : expression_(expression) {}

AstReturnStatement::~AstReturnStatement() {
  delete expression_;
  expression_ = nullptr;
}

int AstReturnStatement::type() const { return FA_RETURN_STATEMENT; }

std::string AstReturnStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstReturnStatement expression_:{} ]", exprStr);
}

AstVariableDeclaration::AstVariableDeclaration(const char *identifier,
                                               AstExpression *expression)
    : identifier_(identifier), expression_(expression) {}

AstVariableDeclaration::~AstVariableDeclaration() {
  delete expression_;
  expression_ = nullptr;
}

int AstVariableDeclaration::type() const { return FA_VARIABLE_DECLARATION; }

std::string AstVariableDeclaration::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstVariableDeclaration identifier_:{}, "
                     "expression_:{} ]",
                     identifier_, exprStr);
}

const std::string &AstVariableDeclaration::identifier() const {
  return identifier_;
}

AstExpression *AstVariableDeclaration::expression() const {
  return expression_;
}

AstFunctionDeclaration::AstFunctionDeclaration(
    const std::string &identifier, const AstStringList &argumentList,
    AstCompoundStatement *compoundStatement)
    : identifier_(identifier), argumentList_(argumentList),
      compoundStatement_(compoundStatement) {}

AstFunctionDeclaration::~AstFunctionDeclaration() {
  delete compoundStatement_;
  compoundStatement_ = nullptr;
}

int AstFunctionDeclaration::type() const { return FA_FUNCTION_DECLARATION; }

std::string AstFunctionDeclaration::toString() const {
  std::string compStr =
      compoundStatement_ ? compoundStatement_->toString() : "null";
  return fmt::format("[ @AstFunctionDeclaration identifier_:{}, "
                     "argumentList_#size:{}, compoundStatement_:{} ]",
                     identifier_, argumentList_.size(), compStr);
}

const std::string &AstFunctionDeclaration::identifier() const {
  return identifier_;
}

const AstStringList &AstFunctionDeclaration::argumentList() const {
  return argumentList_;
}

AstCompoundStatement *AstFunctionDeclaration::compoundStatement() const {
  return compoundStatement_;
}
