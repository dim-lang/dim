// Copyright 2019- <rooster-lang>
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

Ast::Ast(const std::string &name) : name_(name) {}

std::string Ast::name() const { return name_; }

AstExpression::AstExpression(const std::string &name) : Ast(name) {}

AstStatement::AstStatement(const std::string &name) : Ast(name) {}

AstDeclaration::AstDeclaration(const std::string &name) : AstStatement(name) {}

AstExpressionList::AstExpressionList()
    : detail::AstList<AstExpression>(nameGen("A_ExpList")) {}

std::string AstExpressionList::stringify() const { return "AstExpressionList"; }

AstType AstExpressionList::type() const { return AstType::ExpressionList; }

AstStatementList::AstStatementList()
    : detail::AstList<AstStatement>(nameGen("A_StmList")) {}

std::string AstStatementList::stringify() const { return "AstStatementList"; }

AstType AstStatementList::type() const { return AstType::StatementList; }

AstDeclarationList::AstDeclarationList()
    : detail::AstList<AstDeclaration>(nameGen("A_DeclList")) {}

std::string AstDeclarationList::stringify() const {
  return "AstDeclarationList";
}

AstType AstDeclarationList::type() const { return AstType::DeclarationList; }

AstTranslateUnit::AstTranslateUnit()
    : detail::AstList<AstDeclaration>(nameGen("A_Unit")) {}

AstType AstTranslateUnit::type() const { return AstType::TranslateUnit; }

std::string AstTranslateUnit::stringify() const { return "AstTranslateUnit"; }

AstIdentifierConstant::AstIdentifierConstant(const char *value)
    : AstExpression(nameGenWith(value, "A_id")), value_(value) {}

AstType AstIdentifierConstant::type() const {
  return AstType::IdentifierConstant;
}

std::string AstIdentifierConstant::toString() const {
  return fmt::format("[ @AstIdentifierConstant value_:{} ]", value_);
}

const std::string &AstIdentifierConstant::value() const { return value_; }

AstI8Constant::AstI8Constant(const int8_t &value)
    : AstExpression(nameGenWith(value, "A_i8")), value_(value) {}

AstType AstI8Constant::type() const { return AstType::I8Constant; }

std::string AstI8Constant::toString() const {
  return fmt::format("[ @AstI8Constant value_:{} ]", (int)value_);
}

const int8_t &AstI8Constant::value() const { return value_; }

AstU8Constant::AstU8Constant(const uint8_t &value)
    : AstExpression(nameGenWith(value, "A_u8")), value_(value) {}

AstType AstU8Constant::type() const { return AstType::U8Constant; }

std::string AstU8Constant::toString() const {
  return fmt::format("[ @AstU8Constant value_:{} ]", (int)value_);
}

const uint8_t &AstU8Constant::value() const { return value_; }

AstI16Constant::AstI16Constant(const int16_t &value)
    : AstExpression(nameGenWith(value, "A_i16")), value_(value) {}

AstType AstI16Constant::type() const { return AstType::I16Constant; }

std::string AstI16Constant::toString() const {
  return fmt::format("[ @AstI16Constant value_:{} ]", value_);
}

const int16_t &AstI16Constant::value() const { return value_; }

AstU16Constant::AstU16Constant(const uint16_t &value)
    : AstExpression(nameGenWith(value, "A_u16")), value_(value) {}

AstType AstU16Constant::type() const { return AstType::U16Constant; }

std::string AstU16Constant::toString() const {
  return fmt::format("[ @AstU16Constant value_:{} ]", value_);
}

const uint16_t &AstU16Constant::value() const { return value_; }

AstI32Constant::AstI32Constant(const int32_t &value)
    : AstExpression(nameGenWith(value, "A_i32")), value_(value) {}

AstType AstI32Constant::type() const { return AstType::I32Constant; }

std::string AstI32Constant::toString() const {
  return fmt::format("[ @AstI32Constant value_:{} ]", value_);
}

const int32_t &AstI32Constant::value() const { return value_; }

AstU32Constant::AstU32Constant(const uint32_t &value)
    : AstExpression(nameGenWith(value, "A_u32")), value_(value) {}

AstType AstU32Constant::type() const { return AstType::U32Constant; }

std::string AstU32Constant::toString() const {
  return fmt::format("[ @AstU32Constant value_:{} ]", value_);
}

const uint32_t &AstU32Constant::value() const { return value_; }

AstI64Constant::AstI64Constant(const int64_t &value)
    : AstExpression(nameGenWith(value, "A_i64")), value_(value) {}

AstType AstI64Constant::type() const { return AstType::I64Constant; }

std::string AstI64Constant::toString() const {
  return fmt::format("[ @AstI64Constant value_:{} ]", value_);
}

const int64_t &AstI64Constant::value() const { return value_; }

AstU64Constant::AstU64Constant(const uint64_t &value)
    : AstExpression(nameGenWith(value, "A_u64")), value_(value) {}

AstType AstU64Constant::type() const { return AstType::U64Constant; }

std::string AstU64Constant::toString() const {
  return fmt::format("[ @AstU64Constant value_:{} ]", value_);
}

const uint64_t &AstU64Constant::value() const { return value_; }

AstF32Constant::AstF32Constant(const float &value)
    : AstExpression(nameGenWith(value, "A_f32")), value_(value) {}

AstType AstF32Constant::type() const { return AstType::F32Constant; }

std::string AstF32Constant::toString() const {
  return fmt::format("[ @AstF32Constant value_:{} ]", value_);
}

const float &AstF32Constant::value() const { return value_; }

AstF64Constant::AstF64Constant(const double &value)
    : AstExpression(nameGenWith(value, "A_f64")), value_(value) {}

AstType AstF64Constant::type() const { return AstType::F64Constant; }

std::string AstF64Constant::toString() const {
  return fmt::format("[ @AstF64Constant value_:{} ]", value_);
}

const double &AstF64Constant::value() const { return value_; }

AstStringConstant::AstStringConstant(const char *value)
    : AstExpression(nameGenWith(value, "A_string")), value_(value) {}

AstType AstStringConstant::type() const { return AstType::StringConstant; }

std::string AstStringConstant::toString() const {
  return fmt::format("[ @AstStringConstant value_:{} ]", value_);
}

const std::string &AstStringConstant::value() const { return value_; }

void AstStringConstant::add(const char *value) {
  value_ = std::string(value) + value_;
}

AstBooleanConstant::AstBooleanConstant(const bool &value)
    : AstExpression(nameGenWith((value ? "true" : "false"), "A_Bool")),
      value_(value) {}

AstType AstBooleanConstant::type() const { return AstType::BooleanConstant; }

std::string AstBooleanConstant::toString() const {
  return fmt::format("[ @AstBooleanConstant value_:{} ]", value_);
}

const bool &AstBooleanConstant::value() const { return value_; }

AstCallExpression::AstCallExpression(const char *identifier,
                                     AstExpressionList *argumentList)
    : AstExpression(nameGenWith(identifier, "A_Call")), identifier_(identifier),
      argumentList_(argumentList) {}

AstCallExpression::~AstCallExpression() {
  delete argumentList_;
  argumentList_ = nullptr;
}

AstType AstCallExpression::type() const { return AstType::CallExpression; }

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
    : AstExpression(nameGen("A_UnrExp")), token_(token),
      expression_(expression) {}

AstUnaryExpression::~AstUnaryExpression() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstUnaryExpression::type() const { return AstType::UnaryExpression; }

std::string AstUnaryExpression::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstUnaryExpression token_:{}, expression_:{} ]",
                     tokenName(token_), exprStr);
}

int AstUnaryExpression::token() const { return token_; }

AstExpression *AstUnaryExpression::expression() const { return expression_; }

AstBinaryExpression::AstBinaryExpression(AstExpression *left, int token,
                                         AstExpression *right)
    : AstExpression(nameGen("A_BinExp")), left_(left), token_(token),
      right_(right) {}

AstBinaryExpression::~AstBinaryExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

AstType AstBinaryExpression::type() const { return AstType::BinaryExpression; }

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
    : AstExpression(nameGen("A_CondExp")), condition_(condition), hit_(hit),
      miss_(miss) {}

AstConditionalExpression::~AstConditionalExpression() {
  delete condition_;
  condition_ = nullptr;
  delete hit_;
  hit_ = nullptr;
  delete miss_;
  miss_ = nullptr;
}

AstType AstConditionalExpression::type() const {
  return AstType::ConditionalExpression;
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
    : AstExpression(nameGen("A_AssExp")), variable_(variable), token_(token),
      value_(value) {}

AstAssignmentExpression::~AstAssignmentExpression() {
  delete variable_;
  variable_ = nullptr;
  delete value_;
  value_ = nullptr;
}

AstType AstAssignmentExpression::type() const {
  return AstType::AssignmentExpression;
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
    : AstExpression(nameGen("A_SeqExp")), expressionList_(expressionList) {}

AstSequelExpression::~AstSequelExpression() {
  delete expressionList_;
  expressionList_ = nullptr;
}

AstType AstSequelExpression::type() const { return AstType::SequelExpression; }

std::string AstSequelExpression::toString() const {
  return fmt::format("[ @AstSequelExpression expressionList_:{} ]",
                     expressionList_ ? expressionList_->toString() : "null");
}

AstExpressionList *AstSequelExpression::expressionList() const {
  return expressionList_;
}

AstExpressionStatement::AstExpressionStatement(AstExpression *expression)
    : AstStatement(nameGen("A_ExpStm")), expression_(expression) {}

AstExpressionStatement::~AstExpressionStatement() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstExpressionStatement::type() const {
  return AstType::ExpressionStatement;
}

std::string AstExpressionStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstExpressionStatement expression_:{} ]", exprStr);
}

AstExpression *AstExpressionStatement::expression() const {
  return expression_;
}

AstCompoundStatement::AstCompoundStatement(AstStatementList *statementList)
    : AstStatement(nameGen("A_Compound")), statementList_(statementList) {}

AstCompoundStatement::~AstCompoundStatement() {
  delete statementList_;
  statementList_ = nullptr;
}

AstType AstCompoundStatement::type() const {
  return AstType::CompoundStatement;
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
    : AstStatement(nameGen("A_If")), condition_(condition), hit_(hit),
      miss_(miss) {}

AstIfStatement::~AstIfStatement() {
  delete condition_;
  condition_ = nullptr;
  delete hit_;
  hit_ = nullptr;
  delete miss_;
  miss_ = nullptr;
}

AstType AstIfStatement::type() const { return AstType::IfStatement; }

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
    : AstStatement(nameGen("A_While")), condition_(condition),
      statement_(statement) {}

AstWhileStatement::~AstWhileStatement() {
  delete condition_;
  condition_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

AstType AstWhileStatement::type() const { return AstType::WhileStatement; }

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
    : AstStatement(nameGen("A_For")), initial_(initial), condition_(condition),
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

AstType AstForStatement::type() const { return AstType::ForStatement; }

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
    : AstStatement(nameGen("A_continue")) {}

AstContinueStatement::~AstContinueStatement() {}

AstType AstContinueStatement::type() const {
  return AstType::ContinueStatement;
}

std::string AstContinueStatement::toString() const {
  return fmt::format("[ @AstContinueStatement ]");
}

AstBreakStatement::AstBreakStatement() : AstStatement(nameGen("A_break")) {}

AstBreakStatement::~AstBreakStatement() {}

AstType AstBreakStatement::type() const { return AstType::BreakStatement; }

std::string AstBreakStatement::toString() const {
  return fmt::format("[ @AstBreakStatement ]");
}

AstReturnStatement::AstReturnStatement(AstExpression *expression)
    : AstStatement(nameGen("A_return")), expression_(expression) {}

AstReturnStatement::~AstReturnStatement() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstReturnStatement::type() const { return AstType::ReturnStatement; }

std::string AstReturnStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstReturnStatement expression_:{} ]", exprStr);
}

AstExpression *AstReturnStatement::expression() const { return expression_; }

AstEmptyStatement::AstEmptyStatement() : AstStatement(nameGen("A_EmpStm")) {}

AstEmptyStatement::~AstEmptyStatement() {}

AstType AstEmptyStatement::type() const { return AstType::EmptyStatement; }

std::string AstEmptyStatement::toString() const {
  return fmt::format("[ @AstEmptyStatement ]");
}

AstVariableDeclaration::AstVariableDeclaration(
    AstDeclarationList *declarationList)
    : AstDeclaration(nameGen("A_VarDecl")), declarationList_(declarationList) {}

AstVariableDeclaration::~AstVariableDeclaration() {
  delete declarationList_;
  declarationList_ = nullptr;
}

AstType AstVariableDeclaration::type() const {
  return AstType::VariableDeclaration;
}

std::string AstVariableDeclaration::toString() const {
  return fmt::format("[ @AstVariableDeclaration declarationList_:{} ]",
                     declarationList_ ? declarationList_->toString() : "null");
}

AstDeclarationList *AstVariableDeclaration::declarationList() const {
  return declarationList_;
}

AstVariableInitialDeclaration::AstVariableInitialDeclaration(
    const char *identifier, AstExpression *expression)
    : AstDeclaration(nameGen("A_VarAssDecl")), identifier_(identifier),
      expression_(expression) {}

AstVariableInitialDeclaration::~AstVariableInitialDeclaration() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstVariableInitialDeclaration::type() const {
  return AstType::VariableInitialDeclaration;
}

std::string AstVariableInitialDeclaration::toString() const {
  std::string expr = expression_ ? expression_->toString() : "null";
  return fmt::format(
      "[ @AstVariableInitialDeclaration identifier_:{}, expression_:{} ]",
      identifier_, expr);
}

const std::string &AstVariableInitialDeclaration::identifier() const {
  return identifier_;
}

AstExpression *AstVariableInitialDeclaration::expression() const {
  return expression_;
}

AstFunctionDeclaration::AstFunctionDeclaration(
    AstFunctionSignatureDeclaration *signature, AstStatement *statement)
    : AstDeclaration(nameGenWith((signature ? signature->identifier() : "null"),
                                 "A_FuncDecl")),
      signature_(signature), statement_(statement) {}

AstFunctionDeclaration::~AstFunctionDeclaration() {
  delete signature_;
  signature_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

AstType AstFunctionDeclaration::type() const {
  return AstType::FunctionDeclaration;
}

std::string AstFunctionDeclaration::toString() const {
  return fmt::format("[ @AstFunctionDeclaration signature_:{}, statement_:{} ]",
                     signature_ ? signature_->toString() : "null",
                     statement_ ? statement_->toString() : "null");
}

AstFunctionSignatureDeclaration *AstFunctionDeclaration::signature() const {
  return signature_;
}

AstStatement *AstFunctionDeclaration::statement() const { return statement_; }

AstFunctionSignatureDeclaration::AstFunctionSignatureDeclaration(
    const char *identifier, AstDeclarationList *argumentList,
    AstExpression *result)
    : AstDeclaration(nameGenWith(identifier, "A_FuncSignDecl")),
      identifier_(identifier), argumentList_(argumentList), result_(result) {}

AstFunctionSignatureDeclaration::~AstFunctionSignatureDeclaration() {
  delete argumentList_;
  argumentList_ = nullptr;
  delete result_;
  result_ = nullptr;
}

const std::string &AstFunctionSignatureDeclaration::identifier() const {
  return identifier_;
}

AstDeclarationList *AstFunctionSignatureDeclaration::argumentList() const {
  return argumentList_;
}

AstExpression *AstFunctionSignatureDeclaration::result() const {
  return result_;
}

AstType AstFunctionSignatureDeclaration::type() const {
  return AstType::FunctionSignatureDeclaration;
}

std::string AstFunctionSignatureDeclaration::toString() const {
  return fmt::format("[ @AstFunctionSignatureDeclaration identifier_:{}, "
                     "argumentList_:{}, result_:{} ]",
                     identifier_,
                     argumentList_ ? argumentList_->toString() : "null",
                     result_ ? result_->toString() : "null");
}

AstFunctionArgumentDeclaration::AstFunctionArgumentDeclaration(
    const char *value)
    : AstDeclaration(nameGenWith(value, "A_FuncArgDecl")), value_(value) {}

AstFunctionArgumentDeclaration::~AstFunctionArgumentDeclaration() {}

AstType AstFunctionArgumentDeclaration::type() const {
  return AstType::FunctionArgumentDeclaration;
}

std::string AstFunctionArgumentDeclaration::toString() const {
  return fmt::format("[ @AstFunctionArgumentDeclaration value_:{} ]", value_);
}

const std::string &AstFunctionArgumentDeclaration::value() const {
  return value_;
}

#undef GEN_NAME
#undef GEN_NAME_VAL
