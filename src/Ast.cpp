// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Token.h"
#include <algorithm>
#include <sstream>
#include <utility>

std::string AstExpressionList::stringify() const { return "AstExpressionList"; }

int AstExpressionList::type() const { return A_EXPRESSION_LIST; }

std::string AstStatementList::stringify() const { return "AstStatementList"; }

int AstStatementList::type() const { return A_STATEMENT_LIST; }

std::string AstDeclarationList::stringify() const {
  return "AstDeclarationList";
}

int AstDeclarationList::type() const { return A_DECLARATION_LIST; }

AstIdentifierConstant::AstIdentifierConstant(const char *value)
    : value_(value) {}

AstIdentifierConstant::~AstIdentifierConstant() {}

int AstIdentifierConstant::type() const { return A_IDENTIFIER_CONSTANT; }

std::string AstIdentifierConstant::toString() const {
  return fmt::format("[ @AstIdentifierConstant value_:{} ]", value_);
}

const std::string &AstIdentifierConstant::value() const { return value_; }

AstI8Constant::AstI8Constant(const int8_t &value) : value_(value) {}

AstI8Constant::~AstI8Constant() {}

int AstI8Constant::type() const { return A_I8_CONSTANT; }

std::string AstI8Constant::toString() const {
  return fmt::format("[ @AstI8Constant value_:{} ]", (int)value_);
}

const int8_t &AstI8Constant::value() const { return value_; }

AstU8Constant::AstU8Constant(const uint8_t &value) : value_(value) {}

AstU8Constant::~AstU8Constant() {}

int AstU8Constant::type() const { return A_U8_CONSTANT; }

std::string AstU8Constant::toString() const {
  return fmt::format("[ @AstU8Constant value_:{} ]", (int)value_);
}

const uint8_t &AstU8Constant::value() const { return value_; }

AstI16Constant::AstI16Constant(const int16_t &value) : value_(value) {}

AstI16Constant::~AstI16Constant() {}

int AstI16Constant::type() const { return A_I16_CONSTANT; }

std::string AstI16Constant::toString() const {
  return fmt::format("[ @AstI16Constant value_:{} ]", value_);
}

const int16_t &AstI16Constant::value() const { return value_; }

AstU16Constant::AstU16Constant(const uint16_t &value) : value_(value) {}

AstU16Constant::~AstU16Constant() {}

int AstU16Constant::type() const { return A_U16_CONSTANT; }

std::string AstU16Constant::toString() const {
  return fmt::format("[ @AstU16Constant value_:{} ]", value_);
}

const uint16_t &AstU16Constant::value() const { return value_; }

AstI32Constant::AstI32Constant(const int32_t &value) : value_(value) {}

AstI32Constant::~AstI32Constant() {}

int AstI32Constant::type() const { return A_I32_CONSTANT; }

std::string AstI32Constant::toString() const {
  return fmt::format("[ @AstI32Constant value_:{} ]", value_);
}

const int32_t &AstI32Constant::value() const { return value_; }

AstU32Constant::AstU32Constant(const uint32_t &value) : value_(value) {}

AstU32Constant::~AstU32Constant() {}

int AstU32Constant::type() const { return A_U32_CONSTANT; }

std::string AstU32Constant::toString() const {
  return fmt::format("[ @AstU32Constant value_:{} ]", value_);
}

const uint32_t &AstU32Constant::value() const { return value_; }

AstI64Constant::AstI64Constant(const int64_t &value) : value_(value) {}

AstI64Constant::~AstI64Constant() {}

int AstI64Constant::type() const { return A_I64_CONSTANT; }

std::string AstI64Constant::toString() const {
  return fmt::format("[ @AstI64Constant value_:{} ]", value_);
}

const int64_t &AstI64Constant::value() const { return value_; }

AstU64Constant::AstU64Constant(const uint64_t &value) : value_(value) {}

AstU64Constant::~AstU64Constant() {}

int AstU64Constant::type() const { return A_U64_CONSTANT; }

std::string AstU64Constant::toString() const {
  return fmt::format("[ @AstU64Constant value_:{} ]", value_);
}

const uint64_t &AstU64Constant::value() const { return value_; }

AstF32Constant::AstF32Constant(const float &value) : value_(value) {}

AstF32Constant::~AstF32Constant() {}

int AstF32Constant::type() const { return A_F32_CONSTANT; }

std::string AstF32Constant::toString() const {
  return fmt::format("[ @AstF32Constant value_:{} ]", value_);
}

const float &AstF32Constant::value() const { return value_; }

AstF64Constant::AstF64Constant(const double &value) : value_(value) {}

AstF64Constant::~AstF64Constant() {}

int AstF64Constant::type() const { return A_F64_CONSTANT; }

std::string AstF64Constant::toString() const {
  return fmt::format("[ @AstF64Constant value_:{} ]", value_);
}

const double &AstF64Constant::value() const { return value_; }

AstStringConstant::AstStringConstant(const char *value) : value_(value) {}

AstStringConstant::~AstStringConstant() {}

int AstStringConstant::type() const { return A_STRING_CONSTANT; }

std::string AstStringConstant::toString() const {
  return fmt::format("[ @AstStringConstant value_:{} ]", value_);
}

const std::string &AstStringConstant::value() const { return value_; }

void AstStringConstant::append(const char *value) {
  value_.append(std::string(value));
}

AstBooleanConstant::AstBooleanConstant(const bool &value) : value_(value) {}

AstBooleanConstant::~AstBooleanConstant() {}

int AstBooleanConstant::type() const { return A_BOOLEAN_CONSTANT; }

std::string AstBooleanConstant::toString() const {
  return fmt::format("[ @AstBooleanConstant value_:{} ]", value_);
}

const bool &AstBooleanConstant::value() const { return value_; }

AstCallExpression::AstCallExpression(const char *identifier,
                                     AstExpressionList *argumentList)
    : identifier_(identifier), argumentList_(argumentList) {}

AstCallExpression::~AstCallExpression() {
  delete argumentList_;
  argumentList_ = nullptr;
}

int AstCallExpression::type() const { return A_CALL_EXPRESSION; }

std::string AstCallExpression::toString() const {
  return fmt::format("[ @AstCallExpression identifier_:{}, argumentList_:{} ]",
                     identifier_,
                     argumentList_ ? argumentList_->toString() : "null");
}

const std::string &AstCallExpression::identifier() const { return identifier_; }

AstExpressionList *AstCallExpression::argumentList() const {
  return argumentList_;
}

AstUnaryExpression::AstUnaryExpression(int token, AstExpression *expression)
    : token_(token), expression_(expression) {}

AstUnaryExpression::~AstUnaryExpression() {
  delete expression_;
  expression_ = nullptr;
}

int AstUnaryExpression::type() const { return A_UNARY_EXPRESSION; }

std::string AstUnaryExpression::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstUnaryExpression token_:{}, expression_:{} ]",
                     TokenName(token_), exprStr);
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

int AstBinaryExpression::type() const { return A_BINARY_EXPRESSION; }

std::string AstBinaryExpression::toString() const {
  std::string leftStr = left_ ? left_->toString() : "null";
  std::string rightStr = right_ ? right_->toString() : "null";
  return fmt::format("[ @AstBinaryExpression left_:{}, token_:{}, right_:{} ]",
                     leftStr, TokenName(token_), rightStr);
}

AstExpression *AstBinaryExpression::left() const { return left_; }

int AstBinaryExpression::token() const { return token_; }

AstExpression *AstBinaryExpression::right() const { return right_; }

AstConditionalExpression::AstConditionalExpression(AstExpression *condition,
                                                   AstExpression *left,
                                                   AstExpression *right)
    : condition_(condition), left_(left), right_(right) {}

AstConditionalExpression::~AstConditionalExpression() {
  delete condition_;
  condition_ = nullptr;
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

int AstConditionalExpression::type() const { return A_CONDITIONAL_EXPRESSION; }

std::string AstConditionalExpression::toString() const {
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string lStr = left_ ? left_->toString() : "null";
  std::string rStr = right_ ? right_->toString() : "null";
  return fmt::format(
      "[ @AstConditionalExpression condition_:{}, left_:{}, right_:{} ]",
      condStr, lStr, rStr);
}

AstExpression *AstConditionalExpression::condition() const {
  return condition_;
}

AstExpression *AstConditionalExpression::left() const { return left_; }

AstExpression *AstConditionalExpression::right() const { return right_; }

AstAssignmentExpression::AstAssignmentExpression(AstExpression *left, int token,
                                                 AstExpression *right)
    : left_(left), token_(token), right_(right) {}

AstAssignmentExpression::~AstAssignmentExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

int AstAssignmentExpression::type() const { return A_ASSIGNMENT_EXPRESSION; }

std::string AstAssignmentExpression::toString() const {
  std::string lstr = left_ ? left_->toString() : "null";
  std::string rstr = right_ ? right_->toString() : "null";
  return fmt::format(
      "[ @AstAssignmentExpression left_:{}, token_:{}, right_:{} ]", lstr,
      TokenName(token_), rstr);
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

int AstExpressionStatement::type() const { return A_EXPRESSION_STATEMENT; }

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
  delete statementList_;
  statementList_ = nullptr;
}

int AstCompoundStatement::type() const { return A_COMPOUND_STATEMENT; }

std::string AstCompoundStatement::toString() const {
  return fmt::format("[ @AstCompoundStatement statementList_:{} ]",
                     statementList_ ? statementList_->toString() : "null");
}

AstStatementList *AstCompoundStatement::statementList() const {
  return statementList_;
}

AstIfStatement::AstIfStatement(AstExpression *condition, AstStatement *left,
                               AstStatement *right)
    : condition_(condition), left_(left), right_(right) {}

AstIfStatement::~AstIfStatement() {
  delete condition_;
  condition_ = nullptr;
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

int AstIfStatement::type() const { return A_IF_STATEMENT; }

std::string AstIfStatement::toString() const {
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string lStr = left_ ? left_->toString() : "null";
  std::string rStr = right_ ? right_->toString() : "null";
  return fmt::format("[ @AstIfStatement condition_:{}, left_:{}, right_:{} ]",
                     condStr, lStr, rStr);
}

AstExpression *AstIfStatement::condition() const { return condition_; }

AstStatement *AstIfStatement::left() const { return left_; }

AstStatement *AstIfStatement::right() const { return right_; }

AstWhileStatement::AstWhileStatement(AstExpression *condition,
                                     AstStatement *statement)
    : condition_(condition), statement_(statement) {}

AstWhileStatement::~AstWhileStatement() {
  delete condition_;
  condition_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

int AstWhileStatement::type() const { return A_WHILE_STATEMENT; }

std::string AstWhileStatement::toString() const {
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format("[ @AstWhileStatement condition_:{}, statement_:{} ]",
                     condStr, stmtStr);
}

AstExpression *AstWhileStatement::condition() const { return condition_; }

AstStatement *AstWhileStatement::statement() const { return statement_; }

AstForStatement::AstForStatement(AstStatement *initialize,
                                 AstStatement *condition, AstExpression *post,
                                 AstStatement *statement)
    : initialize_(initialize), condition_(condition), post_(post),
      statement_(statement) {}

AstForStatement::~AstForStatement() {
  delete initialize_;
  initialize_ = nullptr;
  delete condition_;
  condition_ = nullptr;
  delete post_;
  post_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

int AstForStatement::type() const { return A_FOR_STATEMENT; }

std::string AstForStatement::toString() const {
  std::string initStr = initialize_ ? initialize_->toString() : "null";
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string postStr = post_ ? post_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format("[ @AstForStatement initialize_:{}, condition_:{}, "
                     "post_:{}, statement_:{} ]",
                     initStr, condStr, postStr, stmtStr);
}

AstStatement *AstForStatement::initialize() const { return initialize_; }

AstStatement *AstForStatement::condition() const { return condition_; }

AstExpression *AstForStatement::post() const { return post_; }

AstStatement *AstForStatement::statement() const { return statement_; }

AstContinueStatement::AstContinueStatement() {}

AstContinueStatement::~AstContinueStatement() {}

int AstContinueStatement::type() const { return A_CONTINUE_STATEMENT; }

std::string AstContinueStatement::toString() const {
  return fmt::format("[ @AstContinueStatement ]");
}

AstBreakStatement::AstBreakStatement() {}

AstBreakStatement::~AstBreakStatement() {}

int AstBreakStatement::type() const { return A_BREAK_STATEMENT; }

std::string AstBreakStatement::toString() const {
  return fmt::format("[ @AstBreakStatement ]");
}

AstReturnStatement::AstReturnStatement(AstExpression *expression)
    : expression_(expression) {}

AstReturnStatement::~AstReturnStatement() {
  delete expression_;
  expression_ = nullptr;
}

int AstReturnStatement::type() const { return A_RETURN_STATEMENT; }

std::string AstReturnStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstReturnStatement expression_:{} ]", exprStr);
}

AstExpression *AstReturnStatement::expression() const { return expression_; }

AstVariableDeclaration::AstVariableDeclaration(
    AstDeclarationList *declarationList)
    : declarationList_(declarationList) {}

AstVariableDeclaration::~AstVariableDeclaration() {
  delete declarationList_;
  declarationList_ = nullptr;
}

int AstVariableDeclaration::type() const { return A_VARIABLE_DECLARATION; }

std::string AstVariableDeclaration::toString() const {
  return fmt::format("[ @AstVariableDeclaration declarationList_:{} ]",
                     declarationList_ ? declarationList_->toString() : "null");
}

AstDeclarationList *AstVariableDeclaration::declarationList() const {
  return declarationList_;
}

AstVariableAssignmentDeclaration::AstVariableAssignmentDeclaration(
    const char *identifier, AstExpression *expression)
    : identifier_(identifier), expression_(expression) {}
AstVariableAssignmentDeclaration::~AstVariableAssignmentDeclaration() {
  delete expression_;
  expression_ = nullptr;
}

int AstVariableAssignmentDeclaration::type() const {
  return A_VARIABLE_ASSIGNMENT_DECLARATION;
}

std::string AstVariableAssignmentDeclaration::toString() const {
  std::string expr = expression_ ? expression_->toString() : "null";
  return fmt::format(
      "[ @AstVariableAssignmentDeclaration identifier_:{}, expression_:{} ]",
      identifier_, expr);
}

const std::string &AstVariableAssignmentDeclaration::identifier() const {
  return identifier_;
}

AstExpression *AstVariableAssignmentDeclaration::expression() const {
  return expression_;
}

AstFunctionDeclaration::AstFunctionDeclaration(const char *identifier,
                                               AstDeclarationList *argumentList,
                                               AstExpression *result,
                                               AstStatement *statement)
    : identifier_(identifier), argumentList_(argumentList), result_(result),
      statement_(statement) {}

AstFunctionDeclaration::~AstFunctionDeclaration() {
  delete argumentList_;
  argumentList_ = nullptr;
  delete result_;
  result_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

int AstFunctionDeclaration::type() const { return A_FUNCTION_DECLARATION; }

std::string AstFunctionDeclaration::toString() const {
  std::string arguStr = argumentList_ ? argumentList_->toString() : "null";
  std::string rStr = result_ ? result_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format("[ @AstFunctionDeclaration identifier_:{}, "
                     "argumentList_:{}, result_:{}, statement_:{} ]",
                     identifier_, arguStr, rStr, stmtStr);
}

const std::string &AstFunctionDeclaration::identifier() const {
  return identifier_;
}

AstDeclarationList *AstFunctionDeclaration::argumentList() const {
  return argumentList_;
}

AstExpression *AstFunctionDeclaration::result() const { return result_; }

AstStatement *AstFunctionDeclaration::statement() const { return statement_; }

AstFunctionArgumentDeclaration::AstFunctionArgumentDeclaration(
    const char *value)
    : value_(value) {}

AstFunctionArgumentDeclaration::~AstFunctionArgumentDeclaration() {}

int AstFunctionArgumentDeclaration::type() const {
  return A_FUNCTION_ARGUMENT_DECLARATION;
}

std::string AstFunctionArgumentDeclaration::toString() const {
  return fmt::format("[ @AstFunctionArgumentDeclaration value_:{} ]", value_);
}

const std::string &AstFunctionArgumentDeclaration::value() const {
  return value_;
}
