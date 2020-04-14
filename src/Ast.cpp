// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Counter.h"
#include "Log.h"
#include "NameGenerator.h"
#include "TokenName.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>

Ast::Ast(const AstType &type)
    : type_(type), name_(namegen(type._to_string())) {}

std::string Ast::name() const { return name_; }

AstType Ast::type() const { return type_; }

AstExpression::AstExpression(const AstType &type) : Ast(type) {}

AstStatement::AstStatement(const AstType &type) : Ast(type) {}

AstDeclaration::AstDeclaration(const AstType &type) : AstStatement(type) {}

AstExpressionList::AstExpressionList()
    : detail::AstList<AstExpression>(AstType::EXPR_LIST) {}

AstExpressionList::~AstExpressionList() {}

AstStatementList::AstStatementList()
    : detail::AstList<AstStatement>(AstType::STMT_LIST) {}

AstStatementList::~AstStatementList() {}

AstDeclarationList::AstDeclarationList()
    : detail::AstList<AstDeclaration>(AstType::DECL_LIST) {}

AstDeclarationList::~AstDeclarationList() {}

AstTranslateUnit::AstTranslateUnit()
    : detail::AstList<AstDeclaration>(AstType::TUNIT) {}

AstTranslateUnit::~AstTranslateUnit() {}

AstIdentifierConstant::AstIdentifierConstant(const char *value)
    : AstExpression(AstType::ID_CONST), value_(value) {}

AstIdentifierConstant::~AstIdentifierConstant() {}

std::string AstIdentifierConstant::toString() const {
  return fmt::format("[ @AstIdentifierConstant value_:{} ]", value_);
}

const std::string &AstIdentifierConstant::value() const { return value_; }

AstI8Constant::AstI8Constant(const int8_t &value)
    : AstExpression(AstType::I8_CONST), value_(value) {}

AstI8Constant::~AstI8Constant() {}

std::string AstI8Constant::toString() const {
  return fmt::format("[ @AstI8Constant value_:{} ]", (int)value_);
}

const int8_t &AstI8Constant::value() const { return value_; }

AstU8Constant::AstU8Constant(const uint8_t &value)
    : AstExpression(AstType::U8_CONST), value_(value) {}

AstU8Constant::~AstU8Constant() {}

std::string AstU8Constant::toString() const {
  return fmt::format("[ @AstU8Constant value_:{} ]", (int)value_);
}

const uint8_t &AstU8Constant::value() const { return value_; }

AstI16Constant::AstI16Constant(const int16_t &value)
    : AstExpression(AstType::I16_CONST), value_(value) {}

AstI16Constant::~AstI16Constant() {}

std::string AstI16Constant::toString() const {
  return fmt::format("[ @AstI16Constant value_:{} ]", value_);
}

const int16_t &AstI16Constant::value() const { return value_; }

AstU16Constant::AstU16Constant(const uint16_t &value)
    : AstExpression(AstType::U16_CONST), value_(value) {}

AstU16Constant::~AstU16Constant() {}

std::string AstU16Constant::toString() const {
  return fmt::format("[ @AstU16Constant value_:{} ]", value_);
}

const uint16_t &AstU16Constant::value() const { return value_; }

AstI32Constant::AstI32Constant(const int32_t &value)
    : AstExpression(AstType::I32_CONST), value_(value) {}

AstI32Constant::~AstI32Constant() {}

std::string AstI32Constant::toString() const {
  return fmt::format("[ @AstI32Constant value_:{} ]", value_);
}

const int32_t &AstI32Constant::value() const { return value_; }

AstU32Constant::AstU32Constant(const uint32_t &value)
    : AstExpression(AstType::U32_CONST), value_(value) {}

AstU32Constant::~AstU32Constant() {}

std::string AstU32Constant::toString() const {
  return fmt::format("[ @AstU32Constant value_:{} ]", value_);
}

const uint32_t &AstU32Constant::value() const { return value_; }

AstI64Constant::AstI64Constant(const int64_t &value)
    : AstExpression(AstType::I64_CONST), value_(value) {}

AstI64Constant::~AstI64Constant() {}

std::string AstI64Constant::toString() const {
  return fmt::format("[ @AstI64Constant value_:{} ]", value_);
}

const int64_t &AstI64Constant::value() const { return value_; }

AstU64Constant::AstU64Constant(const uint64_t &value)
    : AstExpression(AstType::U64_CONST), value_(value) {}

AstU64Constant::~AstU64Constant() {}

std::string AstU64Constant::toString() const {
  return fmt::format("[ @AstU64Constant value_:{} ]", value_);
}

const uint64_t &AstU64Constant::value() const { return value_; }

AstF32Constant::AstF32Constant(const float &value)
    : AstExpression(AstType::F32_CONST), value_(value) {}

AstF32Constant::~AstF32Constant() {}

std::string AstF32Constant::toString() const {
  return fmt::format("[ @AstF32Constant value_:{} ]", value_);
}

const float &AstF32Constant::value() const { return value_; }

AstF64Constant::AstF64Constant(const double &value)
    : AstExpression(AstType::F64_CONST), value_(value) {}

AstF64Constant::~AstF64Constant() {}

std::string AstF64Constant::toString() const {
  return fmt::format("[ @AstF64Constant value_:{} ]", value_);
}

const double &AstF64Constant::value() const { return value_; }

AstStringConstant::AstStringConstant(const char *value)
    : AstExpression(AstType::STR_CONST), value_(value) {}

AstStringConstant::~AstStringConstant() {}

std::string AstStringConstant::toString() const {
  return fmt::format("[ @AstStringConstant value_:{} ]", value_);
}

const std::string &AstStringConstant::value() const { return value_; }

void AstStringConstant::add(const char *value) {
  value_ = std::string(value) + value_;
}

AstBooleanConstant::AstBooleanConstant(const bool &value)
    : AstExpression(AstType::BOOL_CONST), value_(value) {}

AstBooleanConstant::~AstBooleanConstant() {}

std::string AstBooleanConstant::toString() const {
  return fmt::format("[ @AstBooleanConstant value_:{} ]", value_);
}

const bool &AstBooleanConstant::value() const { return value_; }

AstCallExpression::AstCallExpression(const char *identifier,
                                     AstExpressionList *argumentList)
    : AstExpression(AstType::CALL_EXPR), identifier_(identifier),
      argumentList_(argumentList) {}

AstCallExpression::~AstCallExpression() {
  delete argumentList_;
  argumentList_ = nullptr;
}

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
    : AstExpression(AstType::UNA_EXPR), token_(token), expression_(expression) {
}

AstUnaryExpression::~AstUnaryExpression() {
  delete expression_;
  expression_ = nullptr;
}

std::string AstUnaryExpression::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstUnaryExpression token_:{}, expression_:{} ]",
                     tokenName(token_), exprStr);
}

int AstUnaryExpression::token() const { return token_; }

AstExpression *AstUnaryExpression::expression() const { return expression_; }

AstBinaryExpression::AstBinaryExpression(AstExpression *left, int token,
                                         AstExpression *right)
    : AstExpression(AstType::BIN_EXPR), left_(left), token_(token),
      right_(right) {}

AstBinaryExpression::~AstBinaryExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

std::string AstBinaryExpression::toString() const {
  std::string leftStr = left_ ? left_->toString() : "null";
  std::string rightStr = right_ ? right_->toString() : "null";
  return fmt::format("[ @AstBinaryExpression left_:{}, token_:{}, right_:{} ]",
                     leftStr, tokenName(token_), rightStr);
}

AstExpression *AstBinaryExpression::left() const { return left_; }

int AstBinaryExpression::token() const { return token_; }

AstExpression *AstBinaryExpression::right() const { return right_; }

AstConditionalExpression::AstConditionalExpression(AstExpression *condition,
                                                   AstExpression *hit,
                                                   AstExpression *miss)
    : AstExpression(AstType::COND_EXPR), condition_(condition), hit_(hit),
      miss_(miss) {}

AstConditionalExpression::~AstConditionalExpression() {
  delete condition_;
  condition_ = nullptr;
  delete hit_;
  hit_ = nullptr;
  delete miss_;
  miss_ = nullptr;
}

std::string AstConditionalExpression::toString() const {
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string hitStr = hit_ ? hit_->toString() : "null";
  std::string missStr = miss_ ? miss_->toString() : "null";
  return fmt::format(
      "[ @AstConditionalExpression condition_:{}, hit_:{}, miss_:{} ]", condStr,
      hitStr, missStr);
}

AstExpression *AstConditionalExpression::condition() const {
  return condition_;
}

AstExpression *AstConditionalExpression::hit() const { return hit_; }

AstExpression *AstConditionalExpression::miss() const { return miss_; }

AstAssignmentExpression::AstAssignmentExpression(AstExpression *variable,
                                                 int token,
                                                 AstExpression *value)
    : AstExpression(AstType::ASSIGN_EXPR), variable_(variable), token_(token),
      value_(value) {}

AstAssignmentExpression::~AstAssignmentExpression() {
  delete variable_;
  variable_ = nullptr;
  delete value_;
  value_ = nullptr;
}

std::string AstAssignmentExpression::toString() const {
  std::string varStr = variable_ ? variable_->toString() : "null";
  std::string valStr = value_ ? value_->toString() : "null";
  return fmt::format(
      "[ @AstAssignmentExpression variable_:{}, token_:{}, value_:{} ]", varStr,
      tokenName(token_), valStr);
}

AstExpression *AstAssignmentExpression::variable() const { return variable_; }

int AstAssignmentExpression::token() const { return token_; }

AstExpression *AstAssignmentExpression::value() const { return value_; }

AstSequelExpression::AstSequelExpression(AstExpressionList *expressionList)
    : AstExpression(AstType::SEQ_EXPR), expressionList_(expressionList) {}

AstSequelExpression::~AstSequelExpression() {
  delete expressionList_;
  expressionList_ = nullptr;
}

std::string AstSequelExpression::toString() const {
  return fmt::format("[ @AstSequelExpression expressionList_:{} ]",
                     expressionList_ ? expressionList_->toString() : "null");
}

AstExpressionList *AstSequelExpression::expressionList() const {
  return expressionList_;
}

AstExpressionStatement::AstExpressionStatement(AstExpression *expression)
    : AstStatement(AstType::EXPR_STMT), expression_(expression) {}

AstExpressionStatement::~AstExpressionStatement() {
  delete expression_;
  expression_ = nullptr;
}

std::string AstExpressionStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstExpressionStatement expression_:{} ]", exprStr);
}

AstExpression *AstExpressionStatement::expression() const {
  return expression_;
}

AstCompoundStatement::AstCompoundStatement(AstStatementList *statementList)
    : AstStatement(AstType::COMP_STMT), statementList_(statementList) {}

AstCompoundStatement::~AstCompoundStatement() {
  delete statementList_;
  statementList_ = nullptr;
}

std::string AstCompoundStatement::toString() const {
  return fmt::format("[ @AstCompoundStatement statementList_:{} ]",
                     statementList_ ? statementList_->toString() : "null");
}

AstStatementList *AstCompoundStatement::statementList() const {
  return statementList_;
}

AstIfStatement::AstIfStatement(AstExpression *condition, AstStatement *hit,
                               AstStatement *miss)
    : AstStatement(AstType::IF_STMT), condition_(condition), hit_(hit),
      miss_(miss) {}

AstIfStatement::~AstIfStatement() {
  delete condition_;
  condition_ = nullptr;
  delete hit_;
  hit_ = nullptr;
  delete miss_;
  miss_ = nullptr;
}

std::string AstIfStatement::toString() const {
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string hStr = hit_ ? hit_->toString() : "null";
  std::string mStr = miss_ ? miss_->toString() : "null";
  return fmt::format("[ @AstIfStatement condition_:{}, hit_:{}, miss_:{} ]",
                     condStr, hStr, mStr);
}

AstExpression *AstIfStatement::condition() const { return condition_; }

AstStatement *AstIfStatement::hit() const { return hit_; }

AstStatement *AstIfStatement::miss() const { return miss_; }

AstWhileStatement::AstWhileStatement(AstExpression *condition,
                                     AstStatement *statement)
    : AstStatement(AstType::WHILE_STMT), condition_(condition),
      statement_(statement) {}

AstWhileStatement::~AstWhileStatement() {
  delete condition_;
  condition_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

std::string AstWhileStatement::toString() const {
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format("[ @AstWhileStatement condition_:{}, statement_:{} ]",
                     condStr, stmtStr);
}

AstExpression *AstWhileStatement::condition() const { return condition_; }

AstStatement *AstWhileStatement::statement() const { return statement_; }

AstForStatement::AstForStatement(AstStatement *initial, AstStatement *condition,
                                 AstExpression *post, AstStatement *statement)
    : AstStatement(AstType::FOR_STMT), initial_(initial), condition_(condition),
      post_(post), statement_(statement) {}

AstForStatement::~AstForStatement() {
  delete initial_;
  initial_ = nullptr;
  delete condition_;
  condition_ = nullptr;
  delete post_;
  post_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

std::string AstForStatement::toString() const {
  std::string initStr = initial_ ? initial_->toString() : "null";
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string postStr = post_ ? post_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format("[ @AstForStatement initial_:{}, condition_:{}, "
                     "post_:{}, statement_:{} ]",
                     initStr, condStr, postStr, stmtStr);
}

AstStatement *AstForStatement::initial() const { return initial_; }

AstStatement *AstForStatement::condition() const { return condition_; }

AstExpression *AstForStatement::post() const { return post_; }

AstStatement *AstForStatement::statement() const { return statement_; }

AstContinueStatement::AstContinueStatement()
    : AstStatement(AstType::CONT_STMT) {}

AstContinueStatement::~AstContinueStatement() {}

std::string AstContinueStatement::toString() const {
  return fmt::format("[ @AstContinueStatement ]");
}

AstBreakStatement::AstBreakStatement() : AstStatement(AstType::BRK_STMT) {}

AstBreakStatement::~AstBreakStatement() {}

std::string AstBreakStatement::toString() const {
  return fmt::format("[ @AstBreakStatement ]");
}

AstReturnStatement::AstReturnStatement(AstExpression *expression)
    : AstStatement(AstType::RET_STMT), expression_(expression) {}

AstReturnStatement::~AstReturnStatement() {
  delete expression_;
  expression_ = nullptr;
}

std::string AstReturnStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstReturnStatement expression_:{} ]", exprStr);
}

AstExpression *AstReturnStatement::expression() const { return expression_; }

AstEmptyStatement::AstEmptyStatement() : AstStatement(AstType::EMP_STMT) {}

AstEmptyStatement::~AstEmptyStatement() {}

std::string AstEmptyStatement::toString() const {
  return fmt::format("[ @AstEmptyStatement ]");
}

AstVariableDeclaration::AstVariableDeclaration(
    AstDeclarationList *declarationList)
    : AstDeclaration(AstType::VAR_DECL), declarationList_(declarationList) {}

AstVariableDeclaration::~AstVariableDeclaration() {
  delete declarationList_;
  declarationList_ = nullptr;
}

std::string AstVariableDeclaration::toString() const {
  return fmt::format("[ @AstVariableDeclaration declarationList_:{} ]",
                     declarationList_ ? declarationList_->toString() : "null");
}

AstDeclarationList *AstVariableDeclaration::declarationList() const {
  return declarationList_;
}

AstVariableAssignmentDeclaration::AstVariableAssignmentDeclaration(
    const char *identifier, AstExpression *expression)
    : AstDeclaration(AstType::VAR_ASSIGN_DECL), identifier_(identifier),
      expression_(expression) {}

AstVariableAssignmentDeclaration::~AstVariableAssignmentDeclaration() {
  delete expression_;
  expression_ = nullptr;
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
    : AstDeclaration(AstType::FUNC_DECL), identifier_(identifier),
      argumentList_(argumentList), result_(result), statement_(statement) {}

AstFunctionDeclaration::~AstFunctionDeclaration() {
  delete argumentList_;
  argumentList_ = nullptr;
  delete result_;
  result_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

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
    : AstDeclaration(AstType::FUNC_ARG_DECL), value_(value) {}

AstFunctionArgumentDeclaration::~AstFunctionArgumentDeclaration() {}

std::string AstFunctionArgumentDeclaration::toString() const {
  return fmt::format("[ @AstFunctionArgumentDeclaration value_:{} ]", value_);
}

const std::string &AstFunctionArgumentDeclaration::value() const {
  return value_;
}

#undef GEN_NAME
#undef GEN_NAME_VAL
