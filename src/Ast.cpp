// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Ast.h"
#include "config/Header.h"

AstIdentifierConstant::AstIdentifierConstant(const std::string &value)
    : value_(value) {}

int AstIdentifierConstant::type() const { return FA_IDENTIFIER_CONSTANT; }

std::string AstIdentifierConstant::toString() const {
  return fmt::format("[ @AstIdentifierConstant value_:{} ]", value_);
}

const std::string &AstIdentifierConstant::value() const { return value_; }

AstIntegerConstant::AstIntegerConstant(const int64_t &value) : value_(value) {}

int AstIntegerConstant::type() const { return FA_INTEGER_CONSTANT; }

std::string AstIntegerConstant::toString() const {
  return fmt::format("[ @AstIntegerConstant value_:{} ]", value_);
}

const int64_t &AstIntegerConstant::value() const { return value_; }

AstDoubleConstant::AstDoubleConstant(const double &value) : value_(value) {}

int AstDoubleConstant::type() const { return FA_DOUBLE_CONSTANT; }

std::string AstDoubleConstant::toString() const {
  return fmt::format("[ @AstDoubleConstant value_:{} ]", value_);
}

const double &AstDoubleConstant::value() const { return value_; }

AstStringConstant::AstStringConstant(const std::string &value)
    : value_(value) {}

int AstStringConstant::type() const { return FA_STRING_CONSTANT; }

std::string AstStringConstant::toString() const {
  return fmt::format("[ @AstStringConstant value_:{} ]", value_);
}

const std::string &AstStringConstant::value() const { return value_; }

AstFunctionCallExpression::AstFunctionCallExpression(
    const std::string &identifier, const AstExpressionList &argumentList)
    : identifier_(identifier), argumentList_(argumentList) {}

int AstFunctionCallExpression::type() const {
  return FA_FUNCTION_CALL_EXPRESSION;
}

std::string AstFunctionCallExpression::toString() const {
  return fmt::format("[ @AstFunctionCallExpression identifier_:{} ]",
                     identifier_);
}

const std::string &AstFunctionCallExpression::identifier() const {
  return identifier_;
}

const AstExpressionList &AstFunctionCallExpression::argumentList() const {
  return argumentList_;
}

AstUnaryOperationExpression::AstUnaryOperationExpression(
    int token, std::shared_ptr<AstExpression> expression)
    : token_(token), expression_(expression) {}

int AstUnaryOperationExpression::type() const {
  return FA_UNARY_OPERATION_EXPRESSION;
}

std::string AstUnaryOperationExpression::toString() const {
  return fmt::format(
      "[ @AstUnaryOperationExpression token_:{}, expression_:{} ]", token_,
      expression_->toString());
}
int AstUnaryOperationExpression::token() const { return token_; }

std::shared_ptr<AstExpression> AstUnaryOperationExpression::expression() const {
  return expression_;
}

AstBinaryOperationExpression::AstBinaryOperationExpression(
    std::shared_ptr<AstExpression> left, int token,
    std::shared_ptr<AstExpression> right)
    : left_(left), token_(token), right_(right) {}
int AstBinaryOperationExpression::type() const {
  return FA_BINARY_OPERATION_EXPRESSION;
}

std::string AstBinaryOperationExpression::toString() const {
  return fmt::format(
      "[ @AstBinaryOperationExpression left_:{}, token_:{}, right_:{} ]",
      left_->toString(), token_, right_->toString());
}

std::shared_ptr<AstExpression> AstBinaryOperationExpression::left() const {
  return left_;
}
int AstBinaryOperationExpression::token() const { return token_; }

std::shared_ptr<AstExpression> AstBinaryOperationExpression::right() const {
  return right_;
}

AstExpressionStatement::AstExpressionStatement(
    std::shared_ptr<AstExpression> expression)
    : expression_(expression) {}

int AstExpressionStatement::type() const { return FA_EXPRESSION_STATEMENT; }

std::string AstExpressionStatement::toString() const {
  return fmt::format("[ @AstExpressionStatement expression_:{} ]",
                     expression_->toString());
}
std::shared_ptr<AstExpression> AstExpressionStatement::expression() const {
  return expression_;
}

AstCompoundStatement::AstCompoundStatement(
    const AstStatementList &statementList)
    : statementList_(statementList) {}

int AstCompoundStatement::type() const { return FA_COMPOUND_STATEMENT; }

std::string AstCompoundStatement::toString() const {
  return fmt::format("[ @AstCompoundStatement statementList_#size:{} ]",
                     statementList_.size());
}

const AstStatementList &AstCompoundStatement::statementList() const {
  return statementList_;
}

AstIfStatement::AstIfStatement(std::shared_ptr<AstExpression> expression,
                               std::shared_ptr<AstStatement> ifStatement,
                               std::shared_ptr<AstStatement> elseStatement)
    : expression_(expression), ifStatement_(ifStatement),
      elseStatement_(elseStatement) {}

int AstIfStatement::type() const { return FA_IF_STATEMENT; }

std::string AstIfStatement::toString() const {
  return fmt::format(
      "[ @AstIfStatement expression_:{}, ifStatement_:{}, elseStatement_:{} ]",
      expression_->toString(), ifStatement_->toString(),
      elseStatement_->toString());
}

std::shared_ptr<AstExpression> AstIfStatement::expression() const {
  return expression_;
}

std::shared_ptr<AstStatement> AstIfStatement::ifStatement() const {
  return ifStatement_;
}

std::shared_ptr<AstStatement> AstIfStatement::elseStatement() const {
  return elseStatement_;
}

AstWhileStatement::AstWhileStatement(std::shared_ptr<AstExpression> expression,
                                     std::shared_ptr<AstStatement> statement)
    : expression_(expression), statement_(statement) {}

int AstWhileStatement::type() const { return FA_WHILE_STATEMENT; }

std::string AstWhileStatement::toString() const {
  return fmt::format("[ @AstWhileStatement expression_:{}, statement_:{} ]",
                     expression_->toString(), statement_->toString());
}

std::shared_ptr<AstExpression> AstWhileStatement::expression() const {
  return expression_;
}
std::shared_ptr<AstStatement> AstWhileStatement::statement() const {
  return statement_;
}

AstForStatement::AstForStatement(
    std::shared_ptr<AstExpression> initExpression,
    std::shared_ptr<AstExpression> conditionExpression,
    std::shared_ptr<AstExpression> postExpression,
    std::shared_ptr<AstStatement> statement)
    : initExpression_(initExpression),
      conditionExpression_(conditionExpression),
      postExpression_(postExpression), statement_(statement) {}

int AstForStatement::type() const { return FA_FOR_STATEMENT; }

std::string AstForStatement::toString() const {
  return fmt::format(
      "[ @AstForStatement initExpression_:{}, conditionExpression_:{}, "
      "postExpression_:{}, statement_:{} ]",
      initExpression_->toString(), conditionExpression_->toString(),
      postExpression_->toString(), statement_->toString());
}

std::shared_ptr<AstExpression> AstForStatement::initExpression() const {
  return initExpression_;
}

std::shared_ptr<AstExpression> AstForStatement::conditionExpression() const {
  return conditionExpression_;
}

std::shared_ptr<AstExpression> AstForStatement::postExpression() const {
  return postExpression_;
}

std::shared_ptr<AstStatement> AstForStatement::statement() const {
  return statement_;
}

AstContinueStatement::AstContinueStatement() {}

int AstContinueStatement::type() const { return FA_CONTINUE_STATEMENT; }

std::string AstContinueStatement::toString() const {
  return fmt::format("[ @AstContinueStatement ]");
}

AstBreakStatement::AstBreakStatement() {}

int AstBreakStatement::type() const { return FA_BREAK_STATEMENT; }

std::string AstBreakStatement::toString() const {
  return fmt::format("[ @AstBreakStatement ]");
}

AstReturnStatement::AstReturnStatement(
    std::shared_ptr<AstExpression> expression)
    : expression_(expression) {}

int AstReturnStatement::type() const { return FA_RETURN_STATEMENT; }

std::string AstReturnStatement::toString() const {
  return fmt::format("[ @AstReturnStatement expression_:{} ]",
                     expression_->toString());
}

AstVariableDeclaration::AstVariableDeclaration(
    const AstStringList &identifierList,
    const AstExpressionList &expressionList)
    : identifierList_(identifierList), expressionList_(expressionList) {}

int AstVariableDeclaration::type() const { return FA_VARIABLE_DECLARATION; }

std::string AstVariableDeclaration::toString() const {
  return fmt::format("[ @AstVariableDeclaration identifierList_#size:{}, "
                     "expressionList_#size:{} ]",
                     identifierList_.size(), expressionList_.size());
}

const AstStringList &AstVariableDeclaration::identifierList() const {
  return identifierList_;
}

const AstExpressionList &AstVariableDeclaration::expressionList() const {
  return expressionList_;
}

AstFunctionDeclaration::AstFunctionDeclaration(
    const std::string &identifier, const AstStringList &argumentList,
    std::shared_ptr<AstCompoundStatement> compoundStatement)
    : identifier_(identifier), argumentList_(argumentList),
      compoundStatement_(compoundStatement) {}

int AstFunctionDeclaration::type() const { return FA_FUNCTION_DECLARATION; }

std::string AstFunctionDeclaration::toString() const {
  return fmt::format("[ @AstFunctionDeclaration identifier_:{}, "
                     "argumentList_#size:{}, compoundStatement_:{} ]",
                     identifier_, argumentList_.size(),
                     compoundStatement_->toString());
}

const std::string &AstFunctionDeclaration::identifier() const {
  return identifier_;
}

const AstStringList &AstFunctionDeclaration::argumentList() const {
  return argumentList_;
}

std::shared_ptr<AstCompoundStatement>
AstFunctionDeclaration::compoundStatement() const {
  return compoundStatement_;
}
