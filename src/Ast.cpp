// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Counter.h"
#include "Log.h"
#include "Token.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>

static Counter NameCounter;

#define A_NAME(x)                                                              \
  (std::string(x) + std::string("@") + std::to_string(NameCounter.count()))

AstExpressionList::AstExpressionList() : name_(A_NAME("A_ExprList")) {}

AstExpressionList::~AstExpressionList() {}

std::string AstExpressionList::stringify() const { return "AstExpressionList"; }

std::string AstExpressionList::name() const { return name_; }

int AstExpressionList::type() const { return A_EXPRESSION_LIST; }

AstStatementList::AstStatementList() : name_(A_NAME("A_StmtList")) {}

AstStatementList::~AstStatementList() {}

std::string AstStatementList::stringify() const { return "AstStatementList"; }

std::string AstStatementList::name() const { return name_; }

int AstStatementList::type() const { return A_STATEMENT_LIST; }

AstDeclarationList::AstDeclarationList() : name_(A_NAME("A_DeclList")) {}

AstDeclarationList::~AstDeclarationList() {}

std::string AstDeclarationList::stringify() const {
  return "AstDeclarationList";
}

std::string AstDeclarationList::name() const { return name_; }

int AstDeclarationList::type() const { return A_DECLARATION_LIST; }

AstIdentifierConstant::AstIdentifierConstant(const char *value)
    : value_(value), name_(A_NAME("A_id")) {}

AstIdentifierConstant::~AstIdentifierConstant() {}

int AstIdentifierConstant::type() const { return A_IDENTIFIER_CONSTANT; }

std::string AstIdentifierConstant::toString() const {
  return fmt::format("[ @AstIdentifierConstant value_:{} ]", value_);
}

std::string AstIdentifierConstant::name() const { return name_; }

const std::string &AstIdentifierConstant::value() const { return value_; }

AstI8Constant::AstI8Constant(const int8_t &value)
    : value_(value), name_(A_NAME("A_i8")) {}

AstI8Constant::~AstI8Constant() {}

int AstI8Constant::type() const { return A_I8_CONSTANT; }

std::string AstI8Constant::toString() const {
  return fmt::format("[ @AstI8Constant value_:{} ]", (int)value_);
}

std::string AstI8Constant::name() const { return name_; }

const int8_t &AstI8Constant::value() const { return value_; }

AstU8Constant::AstU8Constant(const uint8_t &value)
    : value_(value), name_(A_NAME("A_u8")) {}

AstU8Constant::~AstU8Constant() {}

int AstU8Constant::type() const { return A_U8_CONSTANT; }

std::string AstU8Constant::toString() const {
  return fmt::format("[ @AstU8Constant value_:{} ]", (int)value_);
}

std::string AstU8Constant::name() const { return name_; }

const uint8_t &AstU8Constant::value() const { return value_; }

AstI16Constant::AstI16Constant(const int16_t &value)
    : value_(value), name_(A_NAME("A_i16")) {}

AstI16Constant::~AstI16Constant() {}

int AstI16Constant::type() const { return A_I16_CONSTANT; }

std::string AstI16Constant::toString() const {
  return fmt::format("[ @AstI16Constant value_:{} ]", value_);
}

std::string AstI16Constant::name() const { return name_; }

const int16_t &AstI16Constant::value() const { return value_; }

AstU16Constant::AstU16Constant(const uint16_t &value)
    : value_(value), name_(A_NAME("A_u16")) {}

AstU16Constant::~AstU16Constant() {}

int AstU16Constant::type() const { return A_U16_CONSTANT; }

std::string AstU16Constant::toString() const {
  return fmt::format("[ @AstU16Constant value_:{} ]", value_);
}

std::string AstU16Constant::name() const { return name_; }

const uint16_t &AstU16Constant::value() const { return value_; }

AstI32Constant::AstI32Constant(const int32_t &value)
    : value_(value), name_(A_NAME("A_i32")) {}

AstI32Constant::~AstI32Constant() {}

int AstI32Constant::type() const { return A_I32_CONSTANT; }

std::string AstI32Constant::toString() const {
  return fmt::format("[ @AstI32Constant value_:{} ]", value_);
}

std::string AstI32Constant::name() const { return name_; }

const int32_t &AstI32Constant::value() const { return value_; }

AstU32Constant::AstU32Constant(const uint32_t &value)
    : value_(value), name_(A_NAME("A_u32")) {}

AstU32Constant::~AstU32Constant() {}

int AstU32Constant::type() const { return A_U32_CONSTANT; }

std::string AstU32Constant::toString() const {
  return fmt::format("[ @AstU32Constant value_:{} ]", value_);
}

std::string AstU32Constant::name() const { return name_; }

const uint32_t &AstU32Constant::value() const { return value_; }

AstI64Constant::AstI64Constant(const int64_t &value)
    : value_(value), name_(A_NAME("A_i64")) {}

AstI64Constant::~AstI64Constant() {}

int AstI64Constant::type() const { return A_I64_CONSTANT; }

std::string AstI64Constant::toString() const {
  return fmt::format("[ @AstI64Constant value_:{} ]", value_);
}

std::string AstI64Constant::name() const { return name_; }

const int64_t &AstI64Constant::value() const { return value_; }

AstU64Constant::AstU64Constant(const uint64_t &value)
    : value_(value), name_(A_NAME("A_u64")) {}

AstU64Constant::~AstU64Constant() {}

int AstU64Constant::type() const { return A_U64_CONSTANT; }

std::string AstU64Constant::toString() const {
  return fmt::format("[ @AstU64Constant value_:{} ]", value_);
}

std::string AstU64Constant::name() const { return name_; }

const uint64_t &AstU64Constant::value() const { return value_; }

AstF32Constant::AstF32Constant(const float &value)
    : value_(value), name_(A_NAME("A_f32")) {}

AstF32Constant::~AstF32Constant() {}

int AstF32Constant::type() const { return A_F32_CONSTANT; }

std::string AstF32Constant::toString() const {
  return fmt::format("[ @AstF32Constant value_:{} ]", value_);
}

std::string AstF32Constant::name() const { return name_; }

const float &AstF32Constant::value() const { return value_; }

AstF64Constant::AstF64Constant(const double &value)
    : value_(value), name_(A_NAME("A_f64")) {}

AstF64Constant::~AstF64Constant() {}

int AstF64Constant::type() const { return A_F64_CONSTANT; }

std::string AstF64Constant::toString() const {
  return fmt::format("[ @AstF64Constant value_:{} ]", value_);
}

std::string AstF64Constant::name() const { return name_; }

const double &AstF64Constant::value() const { return value_; }

AstStringConstant::AstStringConstant(const char *value)
    : value_(value), name_(A_NAME("A_string")) {}

AstStringConstant::~AstStringConstant() {}

int AstStringConstant::type() const { return A_STRING_CONSTANT; }

std::string AstStringConstant::toString() const {
  return fmt::format("[ @AstStringConstant value_:{} ]", value_);
}

std::string AstStringConstant::name() const { return name_; }

const std::string &AstStringConstant::value() const { return value_; }

void AstStringConstant::append(const char *value) {
  value_.append(std::string(value));
}

AstBooleanConstant::AstBooleanConstant(const bool &value)
    : value_(value), name_(A_NAME("A_Boolean")) {}

AstBooleanConstant::~AstBooleanConstant() {}

int AstBooleanConstant::type() const { return A_BOOLEAN_CONSTANT; }

std::string AstBooleanConstant::toString() const {
  return fmt::format("[ @AstBooleanConstant value_:{} ]", value_);
}

std::string AstBooleanConstant::name() const { return name_; }

const bool &AstBooleanConstant::value() const { return value_; }

AstCallExpression::AstCallExpression(const char *identifier,
                                     AstExpressionList *argumentList)
    : identifier_(identifier), argumentList_(argumentList),
      name_(A_NAME("A_Call")) {}

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

std::string AstCallExpression::name() const { return name_; }

const std::string &AstCallExpression::identifier() const { return identifier_; }

AstExpressionList *AstCallExpression::argumentList() const {
  return argumentList_;
}

AstUnaryExpression::AstUnaryExpression(int token, AstExpression *expression)
    : token_(token), expression_(expression), name_(A_NAME("A_Unary")) {}

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

std::string AstUnaryExpression::name() const { return name_; }

int AstUnaryExpression::token() const { return token_; }

AstExpression *AstUnaryExpression::expression() const { return expression_; }

AstBinaryExpression::AstBinaryExpression(AstExpression *left, int token,
                                         AstExpression *right)
    : left_(left), token_(token), right_(right), name_(A_NAME("A_Binary")) {}

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

std::string AstBinaryExpression::name() const { return name_; }

AstExpression *AstBinaryExpression::left() const { return left_; }

int AstBinaryExpression::token() const { return token_; }

AstExpression *AstBinaryExpression::right() const { return right_; }

AstConditionalExpression::AstConditionalExpression(AstExpression *condition,
                                                   AstExpression *hit,
                                                   AstExpression *miss)
    : condition_(condition), hit_(hit), miss_(miss),
      name_(A_NAME("A_CondExpr")) {}

AstConditionalExpression::~AstConditionalExpression() {
  delete condition_;
  condition_ = nullptr;
  delete hit_;
  hit_ = nullptr;
  delete miss_;
  miss_ = nullptr;
}

int AstConditionalExpression::type() const { return A_CONDITIONAL_EXPRESSION; }

std::string AstConditionalExpression::toString() const {
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string hitStr = hit_ ? hit_->toString() : "null";
  std::string missStr = miss_ ? miss_->toString() : "null";
  return fmt::format(
      "[ @AstConditionalExpression condition_:{}, hit_:{}, miss_:{} ]", condStr,
      hitStr, missStr);
}

std::string AstConditionalExpression::name() const { return name_; }

AstExpression *AstConditionalExpression::condition() const {
  return condition_;
}

AstExpression *AstConditionalExpression::hit() const { return hit_; }

AstExpression *AstConditionalExpression::miss() const { return miss_; }

AstAssignmentExpression::AstAssignmentExpression(AstExpression *variable,
                                                 int token,
                                                 AstExpression *value)
    : variable_(variable), token_(token), value_(value),
      name_(A_NAME("A_AssExpr")) {}

AstAssignmentExpression::~AstAssignmentExpression() {
  delete variable_;
  variable_ = nullptr;
  delete value_;
  value_ = nullptr;
}

int AstAssignmentExpression::type() const { return A_ASSIGNMENT_EXPRESSION; }

std::string AstAssignmentExpression::toString() const {
  std::string varStr = variable_ ? variable_->toString() : "null";
  std::string valStr = value_ ? value_->toString() : "null";
  return fmt::format(
      "[ @AstAssignmentExpression variable_:{}, token_:{}, value_:{} ]", varStr,
      TokenName(token_), valStr);
}

std::string AstAssignmentExpression::name() const { return name_; }

AstExpression *AstAssignmentExpression::variable() const { return variable_; }

int AstAssignmentExpression::token() const { return token_; }

AstExpression *AstAssignmentExpression::value() const { return value_; }

AstEmptyExpression::AstEmptyExpression() : name_(A_NAME("A_EmptyExpr")) {}

AstEmptyExpression::~AstEmptyExpression() {}

int AstEmptyExpression::type() const { return A_EMPTY_EXPRESSION; }

std::string AstEmptyExpression::toString() const {
  return "[ @AstEmptyExpression ]";
}

std::string AstEmptyExpression::name() const { return name_; }

AstExpressionStatement::AstExpressionStatement(AstExpression *expression)
    : expression_(expression), name_(A_NAME("A_ExprStmt")) {}

AstExpressionStatement::~AstExpressionStatement() {
  delete expression_;
  expression_ = nullptr;
}

int AstExpressionStatement::type() const { return A_EXPRESSION_STATEMENT; }

std::string AstExpressionStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstExpressionStatement expression_:{} ]", exprStr);
}

std::string AstExpressionStatement::name() const { return name_; }

AstExpression *AstExpressionStatement::expression() const {
  return expression_;
}

AstCompoundStatement::AstCompoundStatement(AstStatementList *statementList)
    : statementList_(statementList), name_(A_NAME("A_Compound")) {}

AstCompoundStatement::~AstCompoundStatement() {
  delete statementList_;
  statementList_ = nullptr;
}

int AstCompoundStatement::type() const { return A_COMPOUND_STATEMENT; }

std::string AstCompoundStatement::toString() const {
  return fmt::format("[ @AstCompoundStatement statementList_:{} ]",
                     statementList_ ? statementList_->toString() : "null");
}

std::string AstCompoundStatement::name() const { return name_; }

AstStatementList *AstCompoundStatement::statementList() const {
  return statementList_;
}

AstIfStatement::AstIfStatement(AstExpression *condition, AstStatement *hit,
                               AstStatement *miss)
    : condition_(condition), hit_(hit), miss_(miss), name_(A_NAME("A_If")) {}

AstIfStatement::~AstIfStatement() {
  delete condition_;
  condition_ = nullptr;
  delete hit_;
  hit_ = nullptr;
  delete miss_;
  miss_ = nullptr;
}

int AstIfStatement::type() const { return A_IF_STATEMENT; }

std::string AstIfStatement::toString() const {
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string hStr = hit_ ? hit_->toString() : "null";
  std::string mStr = miss_ ? miss_->toString() : "null";
  return fmt::format("[ @AstIfStatement condition_:{}, hit_:{}, miss_:{} ]",
                     condStr, hStr, mStr);
}

std::string AstIfStatement::name() const { return name_; }

AstExpression *AstIfStatement::condition() const { return condition_; }

AstStatement *AstIfStatement::hit() const { return hit_; }

AstStatement *AstIfStatement::miss() const { return miss_; }

AstWhileStatement::AstWhileStatement(AstExpression *condition,
                                     AstStatement *statement)
    : condition_(condition), statement_(statement), name_(A_NAME("A_While")) {}

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

std::string AstWhileStatement::name() const { return name_; }

AstExpression *AstWhileStatement::condition() const { return condition_; }

AstStatement *AstWhileStatement::statement() const { return statement_; }

AstForStatement::AstForStatement(AstStatement *initial, AstStatement *condition,
                                 AstExpression *post, AstStatement *statement)
    : initial_(initial), condition_(condition), post_(post),
      statement_(statement), name_(A_NAME("A_For")) {}

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

int AstForStatement::type() const { return A_FOR_STATEMENT; }

std::string AstForStatement::toString() const {
  std::string initStr = initial_ ? initial_->toString() : "null";
  std::string condStr = condition_ ? condition_->toString() : "null";
  std::string postStr = post_ ? post_->toString() : "null";
  std::string stmtStr = statement_ ? statement_->toString() : "null";
  return fmt::format("[ @AstForStatement initial_:{}, condition_:{}, "
                     "post_:{}, statement_:{} ]",
                     initStr, condStr, postStr, stmtStr);
}

std::string AstForStatement::name() const { return name_; }

AstStatement *AstForStatement::initial() const { return initial_; }

AstStatement *AstForStatement::condition() const { return condition_; }

AstExpression *AstForStatement::post() const { return post_; }

AstStatement *AstForStatement::statement() const { return statement_; }

AstContinueStatement::AstContinueStatement() : name_(A_NAME("A_continue")) {}

AstContinueStatement::~AstContinueStatement() {}

int AstContinueStatement::type() const { return A_CONTINUE_STATEMENT; }

std::string AstContinueStatement::toString() const {
  return fmt::format("[ @AstContinueStatement ]");
}

std::string AstContinueStatement::name() const { return name_; }

AstBreakStatement::AstBreakStatement() : name_(A_NAME("A_break")) {}

AstBreakStatement::~AstBreakStatement() {}

int AstBreakStatement::type() const { return A_BREAK_STATEMENT; }

std::string AstBreakStatement::toString() const {
  return fmt::format("[ @AstBreakStatement ]");
}

std::string AstBreakStatement::name() const { return name_; }

AstReturnStatement::AstReturnStatement(AstExpression *expression)
    : expression_(expression), name_(A_NAME("A_return")) {}

AstReturnStatement::~AstReturnStatement() {
  delete expression_;
  expression_ = nullptr;
}

int AstReturnStatement::type() const { return A_RETURN_STATEMENT; }

std::string AstReturnStatement::toString() const {
  std::string exprStr = expression_ ? expression_->toString() : "null";
  return fmt::format("[ @AstReturnStatement expression_:{} ]", exprStr);
}

std::string AstReturnStatement::name() const { return name_; }

AstExpression *AstReturnStatement::expression() const { return expression_; }

AstVariableDeclaration::AstVariableDeclaration(
    AstDeclarationList *declarationList)
    : declarationList_(declarationList), name_(A_NAME("A_VarDecl")) {}

AstVariableDeclaration::~AstVariableDeclaration() {
  delete declarationList_;
  declarationList_ = nullptr;
}

int AstVariableDeclaration::type() const { return A_VARIABLE_DECLARATION; }

std::string AstVariableDeclaration::toString() const {
  return fmt::format("[ @AstVariableDeclaration declarationList_:{} ]",
                     declarationList_ ? declarationList_->toString() : "null");
}

std::string AstVariableDeclaration::name() const { return name_; }

AstDeclarationList *AstVariableDeclaration::declarationList() const {
  return declarationList_;
}

AstVariableAssignmentDeclaration::AstVariableAssignmentDeclaration(
    const char *identifier, AstExpression *expression)
    : identifier_(identifier), expression_(expression),
      name_(A_NAME("A_VarAssDecl")) {}

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

std::string AstVariableAssignmentDeclaration::name() const { return name_; }

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
      statement_(statement), name_(A_NAME("A_FuncDecl")) {}

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

std::string AstFunctionDeclaration::name() const { return name_; }

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
    : value_(value), name_(A_NAME("A_FuncArgDecl")) {}

AstFunctionArgumentDeclaration::~AstFunctionArgumentDeclaration() {}

int AstFunctionArgumentDeclaration::type() const {
  return A_FUNCTION_ARGUMENT_DECLARATION;
}

std::string AstFunctionArgumentDeclaration::toString() const {
  return fmt::format("[ @AstFunctionArgumentDeclaration value_:{} ]", value_);
}

std::string AstFunctionArgumentDeclaration::name() const { return name_; }

const std::string &AstFunctionArgumentDeclaration::value() const {
  return value_;
}

#undef A_NAME
