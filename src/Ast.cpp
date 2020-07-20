// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Counter.h"
#include "NameGenerator.h"
#include "TokenName.h"
#include <algorithm>
#include <string>
#include <utility>

static NameGenerator nameGen("", "_", "@");

Ast::Ast(const std::string &name) : name_(name) {}

Ast::Ast(const std::string &name, const Position &position)
    : position_(position), name_(name) {}

std::string Ast::name() const { return name_; }

const Position &Ast::position() const { return position_; }

bool Ast::isConstant(const Ast *node) {
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::Int8Literal:
  case AstType::UInt8Literal:
  case AstType::Int16Literal:
  case AstType::UInt16Literal:
  case AstType::Int32Literal:
  case AstType::UInt32Literal:
  case AstType::Int64Literal:
  case AstType::UInt64Literal:
  case AstType::Float32Literal:
  case AstType::Float64Literal:
  case AstType::Identifier:
  case AstType::StringLiteral:
  case AstType::BooleanLiteral:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isExpression(const Ast *node) {
  EX_ASSERT(node, "node is null");
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
  EX_ASSERT(node, "node is null");
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
  EX_ASSERT(node, "node is null");
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
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::Float32Literal:
  case AstType::Float64Literal:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isIntegerConstant(const Ast *node) {
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::Int8Literal:
  case AstType::UInt8Literal:
  case AstType::Int16Literal:
  case AstType::UInt16Literal:
  case AstType::Int32Literal:
  case AstType::UInt32Literal:
  case AstType::Int64Literal:
  case AstType::UInt64Literal:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isSignedIntegerConstant(const Ast *node) {
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::Int8Literal:
  case AstType::Int16Literal:
  case AstType::Int32Literal:
  case AstType::Int64Literal:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isUnsignedIntegerConstant(const Ast *node) {
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::UInt8Literal:
  case AstType::UInt16Literal:
  case AstType::UInt32Literal:
  case AstType::UInt64Literal:
    return true;
  default:
    return false;
  }
  return false;
}

AstExpression::AstExpression(const std::string &name) : Ast(name) {}

AstExpression::AstExpression(const std::string &name, const Position &position)
    : Ast(name, position) {}

AstLiteral::AstLiteral(const std::string &name) : AstExpression(name) {}

AstLiteral::AstLiteral(const std::string &name, const Position &position)
    : AstExpression(name, position) {}

AstStatement::AstStatement(const std::string &name) : Ast(name) {}

AstStatement::AstStatement(const std::string &name, const Position &position)
    : Ast(name, position) {}

AstDefinition::AstDefinition(const std::string &name) : AstStatement(name) {}

AstDefinition::AstDefinition(const std::string &name, const Position &position)
    : AstStatement(name, position) {}

AstExpressionList::AstExpressionList()
    : detail::AstList<AstExpression>(nameGen.generate("A_ExpList")) {}

std::string AstExpressionList::stringify() const { return "AstExpressionList"; }

AstType AstExpressionList::type() const { return AstType::ExpressionList; }

AstStatementList::AstStatementList()
    : detail::AstList<AstStatement>(nameGen.generate("A_StmList")) {}

std::string AstStatementList::stringify() const { return "AstStatementList"; }

AstType AstStatementList::type() const { return AstType::StatementList; }

AstDefinitionList::AstDefinitionList()
    : detail::AstList<AstDefinition>(nameGen.generate("A_DeclList")) {}

std::string AstDefinitionList::stringify() const { return "AstDefinitionList"; }

AstType AstDefinitionList::type() const { return AstType::DefinitionList; }

AstTranslateUnit::AstTranslateUnit()
    : detail::AstList<AstDefinition>(nameGen.generate("A_TUnit")) {}

AstType AstTranslateUnit::type() const { return AstType::TranslateUnit; }

std::string AstTranslateUnit::stringify() const { return "AstTranslateUnit"; }

AstIdentifier::AstIdentifier(const char *value, const Position &position)
    : AstExpression(nameGen.generateWith(value, "A_id"), position),
      value_(value) {
  EX_ASSERT(value, "value is null");
}

AstType AstIdentifier::type() const { return AstType::Identifier; }

std::string AstIdentifier::toString() const {
  return fmt::format("[@AstIdentifier {} value_:{}]", position_.toString(),
                     value_);
}

const std::string &AstIdentifier::value() const { return value_; }

AstInt8Literal::AstInt8Literal(const int8_t &value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_i8"), position), value_(value) {
}

AstType AstInt8Literal::type() const { return AstType::Int8Literal; }

std::string AstInt8Literal::toString() const {
  return fmt::format("[@AstInt8Literal {} value_:{}]", position_.toString(),
                     (int)value_);
}

const int8_t &AstInt8Literal::value() const { return value_; }

AstUInt8Literal::AstUInt8Literal(const uint8_t &value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_u8"), position), value_(value) {
}

AstType AstUInt8Literal::type() const { return AstType::UInt8Literal; }

std::string AstUInt8Literal::toString() const {
  return fmt::format("[@AstUInt8Literal {} value_:{}]", position_.toString(),
                     (int)value_);
}

const uint8_t &AstUInt8Literal::value() const { return value_; }

AstInt16Literal::AstInt16Literal(const int16_t &value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_i16"), position),
      value_(value) {}

AstType AstInt16Literal::type() const { return AstType::Int16Literal; }

std::string AstInt16Literal::toString() const {
  return fmt::format("[@AstInt16Literal {} value_:{}]", position_.toString(),
                     value_);
}

const int16_t &AstInt16Literal::value() const { return value_; }

AstUInt16Literal::AstUInt16Literal(const uint16_t &value,
                                   const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_u16"), position),
      value_(value) {}

AstType AstUInt16Literal::type() const { return AstType::UInt16Literal; }

std::string AstUInt16Literal::toString() const {
  return fmt::format("[@AstUInt16Literal {} value_:{}]", position_.toString(),
                     value_);
}

const uint16_t &AstUInt16Literal::value() const { return value_; }

AstInt32Literal::AstInt32Literal(const int32_t &value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_i32"), position),
      value_(value) {}

AstType AstInt32Literal::type() const { return AstType::Int32Literal; }

std::string AstInt32Literal::toString() const {
  return fmt::format("[@AstInt32Literal {} value_:{}]", position_.toString(),
                     value_);
}

const int32_t &AstInt32Literal::value() const { return value_; }

AstUInt32Literal::AstUInt32Literal(const uint32_t &value,
                                   const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_u32"), position),
      value_(value) {}

AstType AstUInt32Literal::type() const { return AstType::UInt32Literal; }

std::string AstUInt32Literal::toString() const {
  return fmt::format("[@AstUInt32Literal {} value_:{}]", position_.toString(),
                     value_);
}

const uint32_t &AstUInt32Literal::value() const { return value_; }

AstInt64Literal::AstInt64Literal(const int64_t &value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_i64"), position),
      value_(value) {}

AstType AstInt64Literal::type() const { return AstType::Int64Literal; }

std::string AstInt64Literal::toString() const {
  return fmt::format("[@AstInt64Literal {} value_:{}]", position_.toString(),
                     value_);
}

const int64_t &AstInt64Literal::value() const { return value_; }

AstUInt64Literal::AstUInt64Literal(const uint64_t &value,
                                   const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_u64"), position),
      value_(value) {}

AstType AstUInt64Literal::type() const { return AstType::UInt64Literal; }

std::string AstUInt64Literal::toString() const {
  return fmt::format("[@AstUInt64Literal {} value_:{}]", position_.toString(),
                     value_);
}

const uint64_t &AstUInt64Literal::value() const { return value_; }

AstFloat32Literal::AstFloat32Literal(const float &value,
                                     const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_f32"), position),
      value_(value) {}

AstType AstFloat32Literal::type() const { return AstType::Float32Literal; }

std::string AstFloat32Literal::toString() const {
  return fmt::format("[@AstFloat32Literal {} value_:{}]", position_.toString(),
                     value_);
}

const float &AstFloat32Literal::value() const { return value_; }

AstFloat64Literal::AstFloat64Literal(const double &value,
                                     const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_f64"), position),
      value_(value) {}

AstType AstFloat64Literal::type() const { return AstType::Float64Literal; }

std::string AstFloat64Literal::toString() const {
  return fmt::format("[@AstFloat64Literal {} value_:{}]", position_.toString(),
                     value_);
}

const double &AstFloat64Literal::value() const { return value_; }

AstStringLiteral::AstStringLiteral(const char *value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_string"), position),
      value_(value) {
  EX_ASSERT(value, "value is null");
}

AstType AstStringLiteral::type() const { return AstType::StringLiteral; }

std::string AstStringLiteral::toString() const {
  return fmt::format("[@AstStringLiteral {} value_:{}]", position_.toString(),
                     value_);
}

const std::string &AstStringLiteral::value() const { return value_; }

void AstStringLiteral::add(const char *value, const Position &position) {
  value_ = std::string(value) + value_;
  position_.updatePosition(position);
}

AstBooleanLiteral::AstBooleanLiteral(const bool &value,
                                     const Position &position)
    : AstLiteral(nameGen.generateWith((value ? "true" : "false"), "A_Bool"),
                 position),
      value_(value) {}

AstType AstBooleanLiteral::type() const { return AstType::BooleanLiteral; }

std::string AstBooleanLiteral::toString() const {
  return fmt::format("[@AstBooleanLiteral {} value_:{}]", position_.toString(),
                     value_);
}

const bool &AstBooleanLiteral::value() const { return value_; }

AstCallExpression::AstCallExpression(const char *identifier,
                                     AstExpressionList *argumentList,
                                     const Position &identifierPosition)
    : AstExpression(nameGen.generateWith(identifier, "A_Call"),
                    identifierPosition),
      identifier_(identifier), argumentList_(argumentList) {
  EX_ASSERT(identifier, "identifier is null");
  EX_ASSERT(argumentList_, "argumentList_ is null");
  for (int i = 0; i < argumentList_->size(); i++) {
    position_.updatePosition(argumentList_->get(i)->position());
  }
}

AstCallExpression::~AstCallExpression() {
  delete argumentList_;
  argumentList_ = nullptr;
}

AstType AstCallExpression::type() const { return AstType::CallExpression; }

std::string AstCallExpression::toString() const {
  return fmt::format("[@AstCallExpression {} identifier_:{}, argumentList_:{}]",
                     position_.toString(), identifier_,
                     argumentList_->toString());
}

const std::string &AstCallExpression::identifier() const { return identifier_; }

AstExpressionList *AstCallExpression::argumentList() const {
  return argumentList_;
}

AstUnaryExpression::AstUnaryExpression(int token, AstExpression *expression,
                                       const Position &tokenPosition)
    : AstExpression(nameGen.generate("A_UnrExp"), tokenPosition), token_(token),
      expression_(expression) {
  EX_ASSERT(expression_, "expression_ is null");
  EX_ASSERT(tokenNameExist(token_), "invalid token_:{}, expression:{}", token_,
            expression_->toString());
  position_.updatePosition(expression_->position());
}

AstUnaryExpression::~AstUnaryExpression() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstUnaryExpression::type() const { return AstType::UnaryExpression; }

std::string AstUnaryExpression::toString() const {
  return fmt::format("[@AstUnaryExpression {} token_:{}, expression_:{}]",
                     position_.toString(), tokenName(token_),
                     expression_->toString());
}

int AstUnaryExpression::token() const { return token_; }

AstExpression *AstUnaryExpression::expression() const { return expression_; }

AstBinaryExpression::AstBinaryExpression(AstExpression *left, int token,
                                         AstExpression *right,
                                         const Position &tokenPosition)
    : AstExpression(nameGen.generate("A_BinExp"), tokenPosition), left_(left),
      token_(token), right_(right) {
  EX_ASSERT(left_, "left_ is null");
  EX_ASSERT(right_, "right_ is null");
  EX_ASSERT(tokenNameExist(token_), "invalid token_:{}, left_:{}, right_:{}",
            token_, left_->toString(), right_->toString());
  position_.updatePosition(left_->position());
  position_.updatePosition(right_->position());
}

AstBinaryExpression::~AstBinaryExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

AstType AstBinaryExpression::type() const { return AstType::BinaryExpression; }

std::string AstBinaryExpression::toString() const {
  return fmt::format("[@AstBinaryExpression {} left_:{}, token_:{}, right_:{}]",
                     position_.toString(), left_->toString(), tokenName(token_),
                     right_->toString());
}

AstExpression *AstBinaryExpression::left() const { return left_; }

int AstBinaryExpression::token() const { return token_; }

AstExpression *AstBinaryExpression::right() const { return right_; }

AstConditionalExpression::AstConditionalExpression(AstExpression *condition,
                                                   AstExpression *thens,
                                                   AstExpression *elses)
    : AstExpression(nameGen.generate("A_CondExp")), condition_(condition),
      thens_(thens), elses_(elses) {
  EX_ASSERT(condition_, "condition_ is null");
  EX_ASSERT(thens_, "thens_ is null");
  EX_ASSERT(elses_, "elses_ is null");
  position_.updatePosition(condition_->position());
  position_.updatePosition(thens_->position());
  position_.updatePosition(elses_->position());
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
      "[@AstConditionalExpression {} condition_:{}, thens_:{}, elses_:{}]",
      position_.toString(), condition_->toString(), thens_->toString(),
      elses_->toString());
}

AstExpression *AstConditionalExpression::condition() const {
  return condition_;
}

AstExpression *AstConditionalExpression::thens() const { return thens_; }

AstExpression *AstConditionalExpression::elses() const { return elses_; }

AstAssignmentExpression::AstAssignmentExpression(AstExpression *variable,
                                                 int token,
                                                 AstExpression *value,
                                                 const Position &tokenPosition)
    : AstExpression(nameGen.generate("A_AssExp"), tokenPosition),
      variable_(variable), token_(token), value_(value) {
  EX_ASSERT(variable_, "variable_ is null");
  EX_ASSERT(value_, "value_ is null");
  EX_ASSERT(tokenNameExist(token_),
            "invalid token_:{}, variable_:{}, value_:{}", token_,
            variable_->toString(), value_->toString());
  position_.updatePosition(variable_->position());
  position_.updatePosition(value_->position());
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
      "[@AstAssignmentExpression {} variable_:{}, token_:{}, value_:{}]",
      position_.toString(), variable_->toString(), tokenName(token_),
      value_->toString());
}

AstExpression *AstAssignmentExpression::variable() const { return variable_; }

int AstAssignmentExpression::token() const { return token_; }

AstExpression *AstAssignmentExpression::value() const { return value_; }

AstSequelExpression::AstSequelExpression(AstExpressionList *expressionList)
    : AstExpression(nameGen.generate("A_SeqExp")),
      expressionList_(expressionList) {
  EX_ASSERT(expressionList_, "expressionList_ must not null");
  for (int i = 0; i < expressionList_->size(); i++) {
    position_.updatePosition(expressionList_->get(i)->position());
  }
}

AstSequelExpression::~AstSequelExpression() {
  delete expressionList_;
  expressionList_ = nullptr;
}

AstType AstSequelExpression::type() const { return AstType::SequelExpression; }

std::string AstSequelExpression::toString() const {
  return fmt::format("[@AstSequelExpression {} expressionList_:{}]",
                     position_.toString(), expressionList_->toString());
}

AstExpressionList *AstSequelExpression::expressionList() const {
  return expressionList_;
}

AstVoidExpression::AstVoidExpression()
    : AstExpression(nameGen.generate("A_VoidExp")) {}

AstType AstVoidExpression::type() const { return AstType::VoidExpression; }

std::string AstVoidExpression::toString() const {
  return fmt::format("[@AstVoidExpression]");
}

AstExpressionStatement::AstExpressionStatement(
    AstExpression *expression, const Position &semiTokenPosition)
    : AstStatement(nameGen.generate("A_ExpStm"), semiTokenPosition),
      expression_(expression) {
  EX_ASSERT(expression_, "expression_ is null");
  position_.updatePosition(expression_->position());
}

AstExpressionStatement::~AstExpressionStatement() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstExpressionStatement::type() const {
  return AstType::ExpressionStatement;
}

std::string AstExpressionStatement::toString() const {
  return fmt::format("[@AstExpressionStatement {} expression_:{}]",
                     position_.toString(), expression_->toString());
}

AstExpression *AstExpressionStatement::expression() const {
  return expression_;
}

AstCompoundStatement::AstCompoundStatement(AstStatementList *statementList,
                                           const Position &lparenTokenPosition,
                                           const Position &rparenTokenPosition)
    : AstStatement(nameGen.generate("A_Compound")),
      statementList_(statementList) {
  EX_ASSERT(statementList_, "statementList_ must not null");
  position_.updatePosition(lparenTokenPosition);
  position_.updatePosition(rparenTokenPosition);
  for (int i = 0; i < statementList_->size(); i++) {
    position_.updatePosition(statementList_->get(i)->position());
  }
}

AstCompoundStatement::~AstCompoundStatement() {
  delete statementList_;
  statementList_ = nullptr;
}

AstType AstCompoundStatement::type() const {
  return AstType::CompoundStatement;
}

std::string AstCompoundStatement::toString() const {
  return fmt::format("[@AstCompoundStatement {} statementList_:{}]",
                     position_.toString(), statementList_->toString());
}

AstStatementList *AstCompoundStatement::statementList() const {
  return statementList_;
}

AstIfStatement::AstIfStatement(AstExpression *condition, AstStatement *thens,
                               AstStatement *elses,
                               const Position &ifTokenPosition)
    : AstStatement(nameGen.generate("A_If"), ifTokenPosition),
      condition_(condition), thens_(thens), elses_(elses) {
  EX_ASSERT(condition_, "condition_ is null");
  EX_ASSERT(thens_, "thens_ is null");
  EX_ASSERT(elses_, "elses_ is null");
  position_.updatePosition(condition_->position());
  position_.updatePosition(thens_->position());
  position_.updatePosition(elses_->position());
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
  return fmt::format("[@AstIfStatement {} condition_:{}, thens_:{}, elses_:{}]",
                     position_.toString(), condition_->toString(),
                     thens_->toString(), elses_->toString());
}

AstExpression *AstIfStatement::condition() const { return condition_; }

AstStatement *AstIfStatement::thens() const { return thens_; }

AstStatement *AstIfStatement::elses() const { return elses_; }

AstWhileStatement::AstWhileStatement(AstExpression *condition,
                                     AstStatement *statement,
                                     const Position &whileTokenPosition)
    : AstStatement(nameGen.generate("A_While"), whileTokenPosition),
      condition_(condition), statement_(statement) {
  EX_ASSERT(condition_, "condition_ is null");
  EX_ASSERT(statement_, "statement_ is null");
  position_.updatePosition(condition_->position());
  position_.updatePosition(statement_->position());
}

AstWhileStatement::~AstWhileStatement() {
  delete condition_;
  condition_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

AstType AstWhileStatement::type() const { return AstType::WhileStatement; }

std::string AstWhileStatement::toString() const {
  return fmt::format("[@AstWhileStatement {} condition_:{}, statement_:{}]",
                     position_.toString(), condition_->toString(),
                     statement_->toString());
}

AstExpression *AstWhileStatement::condition() const { return condition_; }

AstStatement *AstWhileStatement::statement() const { return statement_; }

AstForStatement::AstForStatement(AstStatement *start, AstStatement *step,
                                 AstExpression *end, AstStatement *statement,
                                 const Position &forTokenPosition)
    : AstStatement(nameGen.generate("A_For"), forTokenPosition), start_(start),
      step_(step), end_(end), statement_(statement) {
  EX_ASSERT(start_, "start_ is null");
  EX_ASSERT(step_, "step_ is null");
  EX_ASSERT(end_, "end_ is null");
  EX_ASSERT(statement_, "statement_ is null");
  position_.updatePosition(start_->position());
  position_.updatePosition(step_->position());
  position_.updatePosition(end_->position());
  position_.updatePosition(statement_->position());
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
      "[@AstForStatement {} start_:{}, step_:{}, end_:{}, statement_:{}]",
      position_.toString(), start_->toString(), step_->toString(),
      end_->toString(), statement_->toString());
}

AstStatement *AstForStatement::start() const { return start_; }

AstStatement *AstForStatement::step() const { return step_; }

AstExpression *AstForStatement::end() const { return end_; }

AstStatement *AstForStatement::statement() const { return statement_; }

AstContinueStatement::AstContinueStatement(
    const Position &continueTokenPosition, const Position &semiTokenPosition)
    : AstStatement(nameGen.generate("A_continue"), continueTokenPosition) {
  position_.updatePosition(semiTokenPosition);
}

AstType AstContinueStatement::type() const {
  return AstType::ContinueStatement;
}

std::string AstContinueStatement::toString() const {
  return fmt::format("[@AstContinueStatement {}]", position_.toString());
}

AstBreakStatement::AstBreakStatement(const Position &breakTokenPosition,
                                     const Position &semiTokenPosition)
    : AstStatement(nameGen.generate("A_break"), breakTokenPosition) {
  position_.updatePosition(semiTokenPosition);
}

AstType AstBreakStatement::type() const { return AstType::BreakStatement; }

std::string AstBreakStatement::toString() const {
  return fmt::format("[@AstBreakStatement {}]", position_.toString());
}

AstReturnStatement::AstReturnStatement(AstExpression *expression,
                                       const Position &returnTokenPosition,
                                       const Position &semiTokenPosition)
    : AstStatement(nameGen.generate("A_return"), returnTokenPosition),
      expression_(expression) {
  EX_ASSERT(expression_, "expression_ is null");
  position_.updatePosition(expression_->position());
  position_.updatePosition(semiTokenPosition);
}

AstReturnStatement::~AstReturnStatement() {
  delete expression_;
  expression_ = nullptr;
}

AstType AstReturnStatement::type() const { return AstType::ReturnStatement; }

std::string AstReturnStatement::toString() const {
  return fmt::format("[@AstReturnStatement {} expression_:{}]",
                     position_.toString(), expression_->toString());
}

AstExpression *AstReturnStatement::expression() const { return expression_; }

AstEmptyStatement::AstEmptyStatement()
    : AstStatement(nameGen.generate("A_EmpStm")) {}

AstEmptyStatement::AstEmptyStatement(const Position &semiTokenPosition)
    : AstStatement(nameGen.generate("A_EmpStm"), semiTokenPosition) {}

AstType AstEmptyStatement::type() const { return AstType::EmptyStatement; }

std::string AstEmptyStatement::toString() const {
  return fmt::format("[@AstEmptyStatement {}]", position_.toString());
}

AstVariableDefinition::AstVariableDefinition(AstDefinitionList *definitionList,
                                             const Position &varTokenPosition,
                                             const Position &semiTokenPosition)
    : AstDefinition(nameGen.generate("A_VarDecl"), varTokenPosition),
      definitionList_(definitionList) {
  EX_ASSERT(definitionList_, "definitionList_ is null");
  position_.updatePosition(semiTokenPosition);
  for (int i = 0; i < definitionList_->size(); i++) {
    position_.updatePosition(definitionList_->get(i)->position());
  }
}

AstVariableDefinition::~AstVariableDefinition() {
  delete definitionList_;
  definitionList_ = nullptr;
}

AstType AstVariableDefinition::type() const {
  return AstType::VariableDefinition;
}

std::string AstVariableDefinition::toString() const {
  return fmt::format("[@AstVariableDefinition {} definitionList_:{}]",
                     position_.toString(), definitionList_->toString());
}

AstDefinitionList *AstVariableDefinition::definitionList() const {
  return definitionList_;
}

AstVariableInitialDefinition::AstVariableInitialDefinition(
    const char *identifier, AstExpression *expression,
    const Position &identifierPosition)
    : AstDefinition(nameGen.generate("A_VarAssDecl"), identifierPosition),
      identifier_(identifier), expression_(expression) {
  EX_ASSERT(identifier, "identifier is null");
  EX_ASSERT(expression_, "expression_ is null");
  position_.updatePosition(expression_->position());
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
      "[@AstVariableInitialDefinition {} identifier_:{}, expression_:{}]",
      position_.toString(), identifier_, expression_->toString());
}

const std::string &AstVariableInitialDefinition::identifier() const {
  return identifier_;
}

AstExpression *AstVariableInitialDefinition::expression() const {
  return expression_;
}

AstFunctionDefinition::AstFunctionDefinition(
    AstFunctionSignatureDefinition *signature, AstStatement *statement)
    : AstDefinition(
          nameGen.generateWith((signature->identifier()), "A_FuncDecl")),
      signature_(signature), statement_(statement) {
  EX_ASSERT(signature_, "signature_ is null");
  EX_ASSERT(statement_, "statement_ is null");
  position_.updatePosition(signature_->position());
  position_.updatePosition(statement_->position());
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
  return fmt::format("[@AstFunctionDefinition {} signature_:{}, statement_:{}]",
                     position_.toString(), signature_->toString(),
                     statement_->toString());
}

AstFunctionSignatureDefinition *AstFunctionDefinition::signature() const {
  return signature_;
}

AstStatement *AstFunctionDefinition::statement() const { return statement_; }

AstFunctionSignatureDefinition::AstFunctionSignatureDefinition(
    const char *identifier, AstDefinitionList *argumentList,
    AstExpression *result, const Position &funcTokenPosition,
    const Position &identifierPosition)
    : AstDefinition(nameGen.generateWith(identifier, "A_FuncSignDecl"),
                    funcTokenPosition),
      identifier_(identifier), argumentList_(argumentList), result_(result) {
  EX_ASSERT(identifier, "identifier is null");
  EX_ASSERT(argumentList_, "argumentList is null");
  // EX_ASSERT(result_, "result_ is null");
  position_.updatePosition(identifierPosition);
  for (int i = 0; i < argumentList_->size(); i++) {
    position_.updatePosition(argumentList_->get(i)->position());
  }
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
  return fmt::format("[@AstFunctionSignatureDefinition {} identifier_:{}, "
                     "argumentList_:{}, result_:null]",
                     position_.toString(), identifier_,
                     argumentList_->toString());
}

AstFunctionArgumentDefinition::AstFunctionArgumentDefinition(
    const char *identifier, const Position &identifierPosition)
    : AstDefinition(nameGen.generateWith(identifier, "A_FuncArgDecl"),
                    identifierPosition),
      identifier_(identifier) {
  EX_ASSERT(identifier, "identifier is null");
}

AstType AstFunctionArgumentDefinition::type() const {
  return AstType::FunctionArgumentDefinition;
}

std::string AstFunctionArgumentDefinition::toString() const {
  return fmt::format("[@AstFunctionArgumentDefinition {} identifier_:{}]",
                     position_.toString(), identifier_);
}

const std::string &AstFunctionArgumentDefinition::identifier() const {
  return identifier_;
}
