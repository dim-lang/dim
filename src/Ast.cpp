// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Counter.h"
#include "NameGenerator.h"
#include "TokenName.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>

static NameGenerator nameGenerator;

Ast::Ast(const std::string &name, int firstLine, int firstColumn, int lastLine,
         int lastColumn)
    : Position(firstLine, firstColumn, lastLine, lastColumn), name_(name) {}

Ast::Ast(const std::string &name, const Position &position)
    : Position(position), name_(name) {}

std::string Ast::name() const { return name_; }

bool Ast::isConstant(const Ast *node) {
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::Int8Constant:
  case AstType::UInt8Constant:
  case AstType::Int16Constant:
  case AstType::UInt16Constant:
  case AstType::Int32Constant:
  case AstType::UInt32Constant:
  case AstType::Int64Constant:
  case AstType::UInt64Constant:
  case AstType::Float32Constant:
  case AstType::Float64Constant:
  case AstType::IdentifierConstant:
  case AstType::StringConstant:
  case AstType::BooleanConstant:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isExpression(const Ast *node) {
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::CallExpression:
  case AstType::UnaryExpression:
  case AstType::BinaryExpression:
  case AstType::ConditionalExpression:
  case AstType::AssignmentExpression:
  case AstType::SequelExpression:
  case AstType::VoidExpression:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isStatement(const Ast *node) {
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::ExpressionStatement:
  case AstType::CompoundStatement:
  case AstType::IfStatement:
  case AstType::WhileStatement:
  case AstType::ForStatement:
  case AstType::ContinueStatement:
  case AstType::BreakStatement:
  case AstType::ReturnStatement:
  case AstType::EmptyStatement:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isDefinition(const Ast *node) {
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::VariableDefinition:
  case AstType::VariableInitialDefinition:
  case AstType::FunctionDefinition:
  case AstType::FunctionSignatureDefinition:
  case AstType::FunctionArgumentDefinition:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isFloatConstant(const Ast *node) {
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::Float32Constant:
  case AstType::Float64Constant:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isIntegerConstant(const Ast *node) {
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::Int8Constant:
  case AstType::UInt8Constant:
  case AstType::Int16Constant:
  case AstType::UInt16Constant:
  case AstType::Int32Constant:
  case AstType::UInt32Constant:
  case AstType::Int64Constant:
  case AstType::UInt64Constant:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isSignedIntegerConstant(const Ast *node) {
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::Int8Constant:
  case AstType::Int16Constant:
  case AstType::Int32Constant:
  case AstType::Int64Constant:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isUnsignedIntegerConstant(const Ast *node) {
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::UInt8Constant:
  case AstType::UInt16Constant:
  case AstType::UInt32Constant:
  case AstType::UInt64Constant:
    return true;
  default:
    return false;
  }
  return false;
}

AstExpression::AstExpression(const std::string &name, int firstLine,
                             int firstColumn, int lastLine, int lastColumn)
    : Ast(name, firstLine, firstColumn, lastLine, lastColumn) {}

AstExpression::AstExpression(const std::string &name, const Position &position)
    : Ast(name, position) {}

AstConstant::AstConstant(const std::string &name, int firstLine,
                         int firstColumn, int lastLine, int lastColumn)
    : AstExpression(name, firstLine, firstColumn, lastLine, lastColumn) {}

AstConstant::AstConstant(const std::string &name, const Position &position)
    : AstExpression(name, position) {}

AstStatement::AstStatement(const std::string &name, int firstLine,
                           int firstColumn, int lastLine, int lastColumn)
    : Ast(name, firstLine, firstColumn, lastLine, lastColumn) {}

AstStatement::AstStatement(const std::string &name, const Position &position)
    : Ast(name, position) {}

AstDefinition::AstDefinition(const std::string &name, int firstLine,
                             int firstColumn, int lastLine, int lastColumn)
    : AstStatement(name, firstLine, firstColumn, lastLine, lastColumn) {}

AstDefinition::AstDefinition(const std::string &name, const Position &position)
    : AstStatement(name, position) {}

AstExpressionList::AstExpressionList()
    : detail::AstList<AstExpression>(nameGenerator.generate("A_ExpList")) {}

std::string AstExpressionList::stringify() const { return "AstExpressionList"; }

AstType AstExpressionList::type() const { return AstType::ExpressionList; }

AstStatementList::AstStatementList()
    : detail::AstList<AstStatement>(nameGenerator.generate("A_StmList")) {}

std::string AstStatementList::stringify() const { return "AstStatementList"; }

AstType AstStatementList::type() const { return AstType::StatementList; }

AstDefinitionList::AstDefinitionList()
    : detail::AstList<AstDefinition>(nameGenerator.generate("A_DeclList")) {}

std::string AstDefinitionList::stringify() const { return "AstDefinitionList"; }

AstType AstDefinitionList::type() const { return AstType::DefinitionList; }

AstTranslateUnit::AstTranslateUnit()
    : detail::AstList<AstDefinition>(nameGenerator.generate("A_Unit")) {}

AstType AstTranslateUnit::type() const { return AstType::TranslateUnit; }

std::string AstTranslateUnit::stringify() const { return "AstTranslateUnit"; }

AstIdentifierConstant::AstIdentifierConstant(const char *value, int firstLine,
                                             int firstColumn, int lastLine,
                                             int lastColumn)
    : AstConstant(nameGenerator.generateWith(value, "A_id"), firstLine,
                  firstColumn, lastLine, lastColumn),
      value_(value) {
  LOG_ASSERT(value, "value is null");
}

AstType AstIdentifierConstant::type() const {
  return AstType::IdentifierConstant;
}

std::string AstIdentifierConstant::toString() const {
  return fmt::format("[@AstIdentifierConstant value_:{}]", value_);
}

const std::string &AstIdentifierConstant::value() const { return value_; }

AstInt8Constant::AstInt8Constant(const int8_t &value, int firstLine,
                                 int firstColumn, int lastLine, int lastColumn)
    : AstConstant(nameGenerator.generateWith(value, "A_i8"), firstLine,
                  firstColumn, lastLine, lastColumn),
      value_(value) {}

AstType AstInt8Constant::type() const { return AstType::Int8Constant; }

std::string AstInt8Constant::toString() const {
  return fmt::format("[@AstInt8Constant value_:{}]", (int)value_);
}

const int8_t &AstInt8Constant::value() const { return value_; }

AstUInt8Constant::AstUInt8Constant(const uint8_t &value, int firstLine,
                                   int firstColumn, int lastLine,
                                   int lastColumn)
    : AstConstant(nameGenerator.generateWith(value, "A_u8"), firstLine,
                  firstColumn, lastLine, lastColumn),
      value_(value) {}

AstType AstUInt8Constant::type() const { return AstType::UInt8Constant; }

std::string AstUInt8Constant::toString() const {
  return fmt::format("[@AstUInt8Constant value_:{}]", (int)value_);
}

const uint8_t &AstUInt8Constant::value() const { return value_; }

AstInt16Constant::AstInt16Constant(const int16_t &value, int firstLine,
                                   int firstColumn, int lastLine,
                                   int lastColumn)
    : AstConstant(nameGenerator.generateWith(value, "A_i16"), firstLine,
                  firstColumn, lastLine, lastColumn),
      value_(value) {}

AstType AstInt16Constant::type() const { return AstType::Int16Constant; }

std::string AstInt16Constant::toString() const {
  return fmt::format("[@AstInt16Constant value_:{}]", value_);
}

const int16_t &AstInt16Constant::value() const { return value_; }

AstUInt16Constant::AstUInt16Constant(const uint16_t &value, int firstLine,
                                     int firstColumn, int lastLine,
                                     int lastColumn)
    : AstConstant(nameGenerator.generateWith(value, "A_u16"), firstLine,
                  firstColumn, lastLine, lastColumn),
      value_(value) {}

AstType AstUInt16Constant::type() const { return AstType::UInt16Constant; }

std::string AstUInt16Constant::toString() const {
  return fmt::format("[@AstUInt16Constant value_:{}]", value_);
}

const uint16_t &AstUInt16Constant::value() const { return value_; }

AstInt32Constant::AstInt32Constant(const int32_t &value, int firstLine,
                                   int firstColumn, int lastLine,
                                   int lastColumn)
    : AstConstant(nameGenerator.generateWith(value, "A_i32"), firstLine,
                  firstColumn, lastLine, lastColumn),
      value_(value) {}

AstType AstInt32Constant::type() const { return AstType::Int32Constant; }

std::string AstInt32Constant::toString() const {
  return fmt::format("[@AstInt32Constant value_:{}]", value_);
}

const int32_t &AstInt32Constant::value() const { return value_; }

AstUInt32Constant::AstUInt32Constant(const uint32_t &value, int firstLine,
                                     int firstColumn, int lastLine,
                                     int lastColumn)
    : AstConstant(nameGenerator.generateWith(value, "A_u32"), firstLine,
                  firstColumn, lastLine, lastColumn),
      value_(value) {}

AstType AstUInt32Constant::type() const { return AstType::UInt32Constant; }

std::string AstUInt32Constant::toString() const {
  return fmt::format("[@AstUInt32Constant value_:{}]", value_);
}

const uint32_t &AstUInt32Constant::value() const { return value_; }

AstInt64Constant::AstInt64Constant(const int64_t &value, int firstLine,
                                   int firstColumn, int lastLine,
                                   int lastColumn)
    : AstConstant(nameGenerator.generateWith(value, "A_i64"), firstLine,
                  firstColumn, lastLine, lastColumn),
      value_(value) {}

AstType AstInt64Constant::type() const { return AstType::Int64Constant; }

std::string AstInt64Constant::toString() const {
  return fmt::format("[@AstInt64Constant value_:{}]", value_);
}

const int64_t &AstInt64Constant::value() const { return value_; }

AstUInt64Constant::AstUInt64Constant(const uint64_t &value, int firstLine,
                                     int firstColumn, int lastLine,
                                     int lastColumn)
    : AstConstant(nameGenerator.generateWith(value, "A_u64"), firstLine,
                  firstColumn, lastLine, lastColumn),
      value_(value) {}

AstType AstUInt64Constant::type() const { return AstType::UInt64Constant; }

std::string AstUInt64Constant::toString() const {
  return fmt::format("[@AstUInt64Constant value_:{}]", value_);
}

const uint64_t &AstUInt64Constant::value() const { return value_; }

AstFloat32Constant::AstFloat32Constant(const float &value)
    : AstConstant(nameGenerator.generateWith(value, "A_f32")), value_(value) {}

AstType AstFloat32Constant::type() const { return AstType::Float32Constant; }

std::string AstFloat32Constant::toString() const {
  return fmt::format("[@AstFloat32Constant value_:{}]", value_);
}

const float &AstFloat32Constant::value() const { return value_; }

AstFloat64Constant::AstFloat64Constant(const double &value)
    : AstConstant(nameGenerator.generateWith(value, "A_f64")), value_(value) {}

AstType AstFloat64Constant::type() const { return AstType::Float64Constant; }

std::string AstFloat64Constant::toString() const {
  return fmt::format("[@AstFloat64Constant value_:{}]", value_);
}

const double &AstFloat64Constant::value() const { return value_; }

AstStringConstant::AstStringConstant(const char *value)
    : AstConstant(nameGenerator.generateWith(value, "A_string")),
      value_(value) {
  LOG_ASSERT(value, "value is null");
}

AstType AstStringConstant::type() const { return AstType::StringConstant; }

std::string AstStringConstant::toString() const {
  return fmt::format("[@AstStringConstant value_:{}]", value_);
}

const std::string &AstStringConstant::value() const { return value_; }

void AstStringConstant::add(const char *value) {
  value_ = std::string(value) + value_;
}

AstBooleanConstant::AstBooleanConstant(const bool &value)
    : AstConstant(
          nameGenerator.generateWith((value ? "true" : "false"), "A_Bool")),
      value_(value) {}

AstType AstBooleanConstant::type() const { return AstType::BooleanConstant; }

std::string AstBooleanConstant::toString() const {
  return fmt::format("[@AstBooleanConstant value_:{}]", value_);
}

const bool &AstBooleanConstant::value() const { return value_; }

AstCallExpression::AstCallExpression(const char *identifier,
                                     AstExpressionList *argumentList)
    : AstExpression(nameGenerator.generateWith(identifier, "A_Call")),
      identifier_(identifier), argumentList_(argumentList) {
  LOG_ASSERT(identifier, "identifier is null");
  LOG_ASSERT(argumentList_, "argumentList_ is null");
}

AstCallExpression::~AstCallExpression() {
  delete argumentList_;
  argumentList_ = nullptr;
}

AstType AstCallExpression::type() const { return AstType::CallExpression; }

std::string AstCallExpression::toString() const {
  return fmt::format("[@AstCallExpression identifier_:{}, argumentList_:{}]",
                     identifier_, argumentList_->toString());
}

const std::string &AstCallExpression::identifier() const { return identifier_; }

AstExpressionList *AstCallExpression::argumentList() const {
  return argumentList_;
}

AstUnaryExpression::AstUnaryExpression(int token, AstExpression *expression)
    : AstExpression(nameGenerator.generate("A_UnrExp")), token_(token),
      expression_(expression) {
  LOG_ASSERT(expression_, "expression_ is null");
  LOG_ASSERT(tokenNameExist(token_), "invalid token_:{}, expression:{}", token_,
             expression_->toString());
}

AstUnaryExpression::~AstUnaryExpression() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstUnaryExpression::type() const { return AstType::UnaryExpression; }

std::string AstUnaryExpression::toString() const {
  return fmt::format("[@AstUnaryExpression token_:{}, expression_:{}]",
                     tokenName(token_), expression_->toString());
}

int AstUnaryExpression::token() const { return token_; }

AstExpression *AstUnaryExpression::expression() const { return expression_; }

AstBinaryExpression::AstBinaryExpression(AstExpression *left, int token,
                                         AstExpression *right)
    : AstExpression(nameGenerator.generate("A_BinExp")), left_(left),
      token_(token), right_(right) {
  LOG_ASSERT(left_, "left_ is null");
  LOG_ASSERT(right_, "right_ is null");
  LOG_ASSERT(tokenNameExist(token_), "invalid token_:{}, left_:{}, right_:{}",
             token_, left_->toString(), right_->toString());
}

AstBinaryExpression::~AstBinaryExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

AstType AstBinaryExpression::type() const { return AstType::BinaryExpression; }

std::string AstBinaryExpression::toString() const {
  return fmt::format("[@AstBinaryExpression left_:{}, token_:{}, right_:{}]",
                     left_->toString(), tokenName(token_), right_->toString());
}

AstExpression *AstBinaryExpression::left() const { return left_; }

int AstBinaryExpression::token() const { return token_; }

AstExpression *AstBinaryExpression::right() const { return right_; }

AstConditionalExpression::AstConditionalExpression(AstExpression *condition,
                                                   AstExpression *thens,
                                                   AstExpression *elses)
    : AstExpression(nameGenerator.generate("A_CondExp")), condition_(condition),
      thens_(thens), elses_(elses) {
  LOG_ASSERT(condition_, "condition_ is null");
  LOG_ASSERT(thens_, "thens_ is null");
  LOG_ASSERT(elses_, "elses_ is null");
}

AstConditionalExpression::~AstConditionalExpression() {
  delete condition_;
  condition_ = nullptr;
  delete thens_;
  thens_ = nullptr;
  delete elses_;
  elses_ = nullptr;
}

AstType AstConditionalExpression::type() const {
  return AstType::ConditionalExpression;
}

std::string AstConditionalExpression::toString() const {
  return fmt::format(
      "[@AstConditionalExpression condition_:{}, thens_:{}, elses_:{}]",
      condition_->toString(), thens_->toString(), elses_->toString());
}

AstExpression *AstConditionalExpression::condition() const {
  return condition_;
}

AstExpression *AstConditionalExpression::thens() const { return thens_; }

AstExpression *AstConditionalExpression::elses() const { return elses_; }

AstAssignmentExpression::AstAssignmentExpression(AstExpression *variable,
                                                 int token,
                                                 AstExpression *value)
    : AstExpression(nameGenerator.generate("A_AssExp")), variable_(variable),
      token_(token), value_(value) {
  LOG_ASSERT(variable_, "variable_ is null");
  LOG_ASSERT(value_, "value_ is null");
  LOG_ASSERT(tokenNameExist(token_),
             "invalid token_:{}, variable_:{}, value_:{}", token_,
             variable_->toString(), value_->toString());
}

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
  return fmt::format(
      "[@AstAssignmentExpression variable_:{}, token_:{}, value_:{}]",
      variable_->toString(), tokenName(token_), value_->toString());
}

AstExpression *AstAssignmentExpression::variable() const { return variable_; }

int AstAssignmentExpression::token() const { return token_; }

AstExpression *AstAssignmentExpression::value() const { return value_; }

AstSequelExpression::AstSequelExpression(AstExpressionList *expressionList)
    : AstExpression(nameGenerator.generate("A_SeqExp")),
      expressionList_(expressionList) {
  LOG_ASSERT(expressionList_, "expressionList_ is null");
}

AstSequelExpression::~AstSequelExpression() {
  delete expressionList_;
  expressionList_ = nullptr;
}

AstType AstSequelExpression::type() const { return AstType::SequelExpression; }

std::string AstSequelExpression::toString() const {
  return fmt::format("[@AstSequelExpression expressionList_:{}]",
                     expressionList_->toString());
}

AstExpressionList *AstSequelExpression::expressionList() const {
  return expressionList_;
}

AstVoidExpression::AstVoidExpression()
    : AstExpression(nameGenerator.generate("A_VoidExp")) {}

AstType AstVoidExpression::type() const { return AstType::VoidExpression; }

std::string AstVoidExpression::toString() const {
  return fmt::format("[@AstVoidExpression]");
}

AstExpressionStatement::AstExpressionStatement(AstExpression *expression)
    : AstStatement(nameGenerator.generate("A_ExpStm")),
      expression_(expression) {
  LOG_ASSERT(expression_, "expression_ is null");
}

AstExpressionStatement::~AstExpressionStatement() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstExpressionStatement::type() const {
  return AstType::ExpressionStatement;
}

std::string AstExpressionStatement::toString() const {
  return fmt::format("[@AstExpressionStatement expression_:{}]",
                     expression_->toString());
}

AstExpression *AstExpressionStatement::expression() const {
  return expression_;
}

AstCompoundStatement::AstCompoundStatement(AstStatementList *statementList)
    : AstStatement(nameGenerator.generate("A_Compound")),
      statementList_(statementList) {
  LOG_ASSERT(statementList_, "statementList_ is null");
}

AstCompoundStatement::~AstCompoundStatement() {
  delete statementList_;
  statementList_ = nullptr;
}

AstType AstCompoundStatement::type() const {
  return AstType::CompoundStatement;
}

std::string AstCompoundStatement::toString() const {
  return fmt::format("[@AstCompoundStatement statementList_:{}]",
                     statementList_->toString());
}

AstStatementList *AstCompoundStatement::statementList() const {
  return statementList_;
}

AstIfStatement::AstIfStatement(AstExpression *condition, AstStatement *thens,
                               AstStatement *elses)
    : AstStatement(nameGenerator.generate("A_If")), condition_(condition),
      thens_(thens), elses_(elses) {
  LOG_ASSERT(condition_, "condition_ is null");
  LOG_ASSERT(thens_, "thens_ is null");
  LOG_ASSERT(elses_, "elses_ is null");
}

AstIfStatement::~AstIfStatement() {
  delete condition_;
  condition_ = nullptr;
  delete thens_;
  thens_ = nullptr;
  delete elses_;
  elses_ = nullptr;
}

AstType AstIfStatement::type() const { return AstType::IfStatement; }

std::string AstIfStatement::toString() const {
  return fmt::format("[@AstIfStatement condition_:{}, thens_:{}, elses_:{}]",
                     condition_->toString(), thens_->toString(),
                     elses_->toString());
}

AstExpression *AstIfStatement::condition() const { return condition_; }

AstStatement *AstIfStatement::thens() const { return thens_; }

AstStatement *AstIfStatement::elses() const { return elses_; }

AstWhileStatement::AstWhileStatement(AstExpression *condition,
                                     AstStatement *statement)
    : AstStatement(nameGenerator.generate("A_While")), condition_(condition),
      statement_(statement) {
  LOG_ASSERT(condition_, "condition_ is null");
  LOG_ASSERT(statement_, "statement_ is null");
}

AstWhileStatement::~AstWhileStatement() {
  delete condition_;
  condition_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

AstType AstWhileStatement::type() const { return AstType::WhileStatement; }

std::string AstWhileStatement::toString() const {
  return fmt::format("[@AstWhileStatement condition_:{}, statement_:{}]",
                     condition_->toString(), statement_->toString());
}

AstExpression *AstWhileStatement::condition() const { return condition_; }

AstStatement *AstWhileStatement::statement() const { return statement_; }

AstForStatement::AstForStatement(AstStatement *start, AstStatement *step,
                                 AstExpression *end, AstStatement *statement)
    : AstStatement(nameGenerator.generate("A_For")), start_(start), step_(step),
      end_(end), statement_(statement) {
  LOG_ASSERT(start_, "start_ is null");
  LOG_ASSERT(step_, "step_ is null");
  LOG_ASSERT(end_, "end_ is null");
  LOG_ASSERT(statement_, "statement_ is null");
}

AstForStatement::~AstForStatement() {
  delete start_;
  start_ = nullptr;
  delete step_;
  step_ = nullptr;
  delete end_;
  end_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

AstType AstForStatement::type() const { return AstType::ForStatement; }

std::string AstForStatement::toString() const {
  return fmt::format(
      "[@AstForStatement start_:{}, step_:{}, end_:{}, statement_:{}]",
      start_->toString(), step_->toString(), end_->toString(),
      statement_->toString());
}

AstStatement *AstForStatement::start() const { return start_; }

AstStatement *AstForStatement::step() const { return step_; }

AstExpression *AstForStatement::end() const { return end_; }

AstStatement *AstForStatement::statement() const { return statement_; }

AstContinueStatement::AstContinueStatement()
    : AstStatement(nameGenerator.generate("A_continue")) {}

AstType AstContinueStatement::type() const {
  return AstType::ContinueStatement;
}

std::string AstContinueStatement::toString() const {
  return fmt::format("[@AstContinueStatement ]");
}

AstBreakStatement::AstBreakStatement()
    : AstStatement(nameGenerator.generate("A_break")) {}

AstType AstBreakStatement::type() const { return AstType::BreakStatement; }

std::string AstBreakStatement::toString() const {
  return fmt::format("[@AstBreakStatement ]");
}

AstReturnStatement::AstReturnStatement(AstExpression *expression)
    : AstStatement(nameGenerator.generate("A_return")),
      expression_(expression) {
  LOG_ASSERT(expression_, "expression_ is null");
}

AstReturnStatement::~AstReturnStatement() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstReturnStatement::type() const { return AstType::ReturnStatement; }

std::string AstReturnStatement::toString() const {
  return fmt::format("[@AstReturnStatement expression_:{}]",
                     expression_->toString());
}

AstExpression *AstReturnStatement::expression() const { return expression_; }

AstEmptyStatement::AstEmptyStatement()
    : AstStatement(nameGenerator.generate("A_EmpStm")) {}

AstType AstEmptyStatement::type() const { return AstType::EmptyStatement; }

std::string AstEmptyStatement::toString() const {
  return fmt::format("[@AstEmptyStatement]");
}

AstVariableDefinition::AstVariableDefinition(AstDefinitionList *definitionList)
    : AstDefinition(nameGenerator.generate("A_VarDecl")),
      definitionList_(definitionList) {
  LOG_ASSERT(definitionList_, "definitionList_ is null");
}

AstVariableDefinition::~AstVariableDefinition() {
  delete definitionList_;
  definitionList_ = nullptr;
}

AstType AstVariableDefinition::type() const {
  return AstType::VariableDefinition;
}

std::string AstVariableDefinition::toString() const {
  return fmt::format("[@AstVariableDefinition definitionList_:{}]",
                     definitionList_->toString());
}

AstDefinitionList *AstVariableDefinition::definitionList() const {
  return definitionList_;
}

AstVariableInitialDefinition::AstVariableInitialDefinition(
    const char *identifier, AstExpression *expression)
    : AstDefinition(nameGenerator.generate("A_VarAssDecl")),
      identifier_(identifier), expression_(expression) {
  LOG_ASSERT(identifier, "identifier is null");
  LOG_ASSERT(expression_, "expression_ is null");
}

AstVariableInitialDefinition::~AstVariableInitialDefinition() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstVariableInitialDefinition::type() const {
  return AstType::VariableInitialDefinition;
}

std::string AstVariableInitialDefinition::toString() const {
  return fmt::format(
      "[@AstVariableInitialDefinition identifier_:{}, expression_:{}]",
      identifier_, expression_->toString());
}

const std::string &AstVariableInitialDefinition::identifier() const {
  return identifier_;
}

AstExpression *AstVariableInitialDefinition::expression() const {
  return expression_;
}

AstFunctionDefinition::AstFunctionDefinition(
    AstFunctionSignatureDefinition *signature, AstStatement *statement)
    : AstDefinition(nameGenerator.generateWith(
          (signature ? signature->identifier() : "null"), "A_FuncDecl")),
      signature_(signature), statement_(statement) {
  LOG_ASSERT(signature_, "signature_ is null");
  LOG_ASSERT(statement_, "statement_ is null");
}

AstFunctionDefinition::~AstFunctionDefinition() {
  delete signature_;
  signature_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

AstType AstFunctionDefinition::type() const {
  return AstType::FunctionDefinition;
}

std::string AstFunctionDefinition::toString() const {
  return fmt::format("[@AstFunctionDefinition signature_:{}, statement_:{}]",
                     signature_->toString(), statement_->toString());
}

AstFunctionSignatureDefinition *AstFunctionDefinition::signature() const {
  return signature_;
}

AstStatement *AstFunctionDefinition::statement() const { return statement_; }

AstFunctionSignatureDefinition::AstFunctionSignatureDefinition(
    const char *identifier, AstDefinitionList *argumentList,
    AstExpression *result)
    : AstDefinition(nameGenerator.generateWith(identifier, "A_FuncSignDecl")),
      identifier_(identifier), argumentList_(argumentList), result_(result) {
  LOG_ASSERT(identifier, "identifier is null");
  LOG_ASSERT(argumentList_, "argumentList is null");
  // LOG_ASSERT(result_, "result_ is null");
}

AstFunctionSignatureDefinition::~AstFunctionSignatureDefinition() {
  delete argumentList_;
  argumentList_ = nullptr;
  delete result_;
  result_ = nullptr;
}

const std::string &AstFunctionSignatureDefinition::identifier() const {
  return identifier_;
}

AstDefinitionList *AstFunctionSignatureDefinition::argumentList() const {
  return argumentList_;
}

AstExpression *AstFunctionSignatureDefinition::result() const {
  return result_;
}

AstType AstFunctionSignatureDefinition::type() const {
  return AstType::FunctionSignatureDefinition;
}

std::string AstFunctionSignatureDefinition::toString() const {
  return fmt::format("[@AstFunctionSignatureDefinition identifier_:{}, "
                     "argumentList_:{}, result_:null]",
                     identifier_, argumentList_->toString());
}

AstFunctionArgumentDefinition::AstFunctionArgumentDefinition(const char *value)
    : AstDefinition(nameGenerator.generateWith(value, "A_FuncArgDecl")),
      value_(value) {
  LOG_ASSERT(value, "value is null");
}

AstType AstFunctionArgumentDefinition::type() const {
  return AstType::FunctionArgumentDefinition;
}

std::string AstFunctionArgumentDefinition::toString() const {
  return fmt::format("[@AstFunctionArgumentDefinition value_:{}]", value_);
}

const std::string &AstFunctionArgumentDefinition::value() const {
  return value_;
}
