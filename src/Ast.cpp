// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "config/Header.h"
#include <sstream>

std::string Ast::dump(const AstExpressionList *expressionList) {
  if (!expressionList) {
    return "expressionList: nullptr";
  }
  std::stringstream ss;
  ss << "expressionList, size:" << expressionList->size() << ", ";
  for (int i = 0; i < expressionList->size(); i++) {
    AstExpression *ae = (*expressionList)[i];
    std::string s = ae ? ae->toString() : "null";
    ss << fmt::format("{}:{}", i, s);
    if (i < expressionList->size() - 1) {
      ss << ", ";
    }
  }
  return ss.str();
}

std::string Ast::dump(const AstStatementList *statementList) {
  if (!statementList) {
    return "statementList: nullptr";
  }
  std::stringstream ss;
  ss << "statementList, size:" << statementList->size() << ", ";
  for (int i = 0; i < statementList->size(); i++) {
    AstStatement *ae = (*statementList)[i];
    std::string s = ae ? ae->toString() : "null";
    ss << fmt::format("{}:{}", i, s);
    if (i < statementList->size() - 1) {
      ss << ", ";
    }
  }
  return ss.str();
}

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
    AstExpression *condExpression, AstExpression *ifExpression,
    AstExpression *elseExpression)
    : condExpression_(condExpression), ifExpression_(ifExpression),
      elseExpression_(elseExpression) {}

AstConditionalExpression::~AstConditionalExpression() {
  delete condExpression_;
  conditionExpression_ = nullptr;
  delete ifExpression_;
  ifExpression_ = nullptr;
  delete elseExpression_;
  elseExpression_ = nullptr;
}

int AstConditionalExpression::type() const { return FA_CONDITIONAL_EXPRESSION; }

std::string AstConditionalExpression::toString() const {
  std::string condStr = condExpression_ ? condExpression_->toString() : "null";
  std::string ifStr = ifExpression_ ? ifExpression_->toString() : "null";
  std::string elseStr = elseExpression_ ? elseExpression_->toString() : "null";
  return fmt::format("[ @AstConditionalExpression condExpression_:{}, "
                     "ifExpression_:{}, elseExpression_:{} ]",
                     condStr, ifStr, elseStr);
}

AstExpression *AstConditionalExpression::condExpression() const {
  return condExpression_;
}

AstExpression *AstConditionalExpression::ifExpression() const {
  return ifExpression_;
}

AstExpression *AstConditionalExpression::elseExpression() const {
  return elseExpression_;
}

AstAssignmentExpression::AstAssignmentExpression(AstExpressionList *left,
                                                 int token,
                                                 AstExpressionList *right)
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
  return fmt::format(
      "[ @AstAssignmentExpression left_#size:{}, token_:{}, right_#size:{} ]",
      left_ ? left_->size() : 0, token_, right_ ? right_->size() : 0);
}

AstExpressionList *AstAssignmentExpression::left() const { return left_; }

int AstAssignmentExpression::token() const { return token_; }

AstExpressionList *AstAssignmentExpression::right() const { return right_; }

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

AstCompoundStatement::AstCompoundStatement(AstStatementList *statementList)
    : statementList_(statementList) {}

AstCompoundStatement::~AstCompoundStatement() {
  if (statementList_) {
    delete statementList_;
    statementList_ = nullptr;
  }
}

int AstCompoundStatement::type() const { return FA_COMPOUND_STATEMENT; }

std::string AstCompoundStatement::toString() const {
  return fmt::format("[ @AstCompoundStatement statementList_#size:{} ]",
                     statementList_ ? statementList_->size() : 0);
}

AstStatementList *AstCompoundStatement::statementList() const {
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

AstForStatement::AstForStatement(AstStatement *initStatement,
                                 AstStatement *condStatement,
                                 AstExpression *postExpression,
                                 AstStatement *statement)
    : initStatement_(initStatement), condStatement_(condStatement),
      postExpression_(postExpression), statement_(statement) {}

AstForStatement::~AstForStatement() {
  delete initStatement_;
  initStatement_ = nullptr;
  delete condStatement_;
  condStatement_ = nullptr;
  delete postExpression_;
  postExpression_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

int AstForStatement::type() const { return FA_FOR_STATEMENT; }

std::string AstForStatement::toString() const {
  std::string initStr = initStatement_ ? initStatement_->toString() : "null";
  std::string condStr = condStatement_ ? condStatement_->toString() : "null";
  std::string postStr = postExpression_ ? postExpression_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format("[ @AstForStatement initStatement_:{}, condStatement_:{}, "
                     "postExpression_:{}, statement_:{} ]",
                     initStr, condStr, postStr, stmtStr);
}

AstStatement *AstForStatement::initStatement() const { return initStatement_; }

AstStatement *AstForStatement::condStatement() const { return condStatement_; }

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

AstExpression *AstReturnStatement::expression() const { return expression_; }

AstVariableDeclaration::AstVariableDeclaration(
    AstExpressionList *identifierList, AstExpressionList *expressionList)
    : identifierList_(identifierList), expressionList_(expressionList) {}

AstVariableDeclaration::~AstVariableDeclaration() {
  delete identifierList_;
  identifierList_ = nullptr;
  delete expressionList_;
  expressionList_ = nullptr;
}

int AstVariableDeclaration::type() const { return FA_VARIABLE_DECLARATION; }

std::string AstVariableDeclaration::toString() const {
  return fmt::format("[ @AstVariableDeclaration identifierList_#size:{}, "
                     "expressionList_#size:{} ]",
                     identifierList_ ? identifierList_->size() : 0,
                     expressionList_ ? expressionList_->size() : 0);
}

AstExpressionList *AstVariableDeclaration::identifierList() const {
  return identifierList_;
}

AstExpressionList *AstVariableDeclaration::expressionList() const {
  return expressionList_;
}

AstFunctionDeclaration::AstFunctionDeclaration(const char *identifier,
                                               AstExpressionList *argumentList,
                                               AstStatementList *statementList)
    : identifier_(identifier), argumentList_(argumentList),
      statementList_(statementList) {}

AstFunctionDeclaration::~AstFunctionDeclaration() {
  delete argumentList_;
  argumentList_ = nullptr;
  delete statementList_;
  statementList_ = nullptr;
}

int AstFunctionDeclaration::type() const { return FA_FUNCTION_DECLARATION; }

std::string AstFunctionDeclaration::toString() const {
  return fmt::format("[ @AstFunctionDeclaration identifier_:{}, "
                     "argumentList_#size:{}, statementList_#size:{} ]",
                     identifier_, argumentList_ ? argumentList_->size() : 0,
                     statementList_ ? statementList_->size() : 0);
}

const std::string &AstFunctionDeclaration::identifier() const {
  return identifier_;
}

AstExpressionList *AstFunctionDeclaration::argumentList() const {
  return argumentList_;
}

AstStatementList *AstFunctionDeclaration::statementList() const {
  return statementList_;
}
