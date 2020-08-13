// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Counter.h"
#include "NameGenerator.h"
#include "Parser.tab.hpp"
#include "TokenName.h"
#include <algorithm>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

static NameGenerator nameGen("", "_", "@");

// string utility {

static bool stringStartsWith(const std::string &s,
                             const std::vector<std::string> &prefixList) {
  return std::all_of(
      prefixList.begin(), prefixList.end(), [&](const std::string &prefix) {
        return s.length() >= prefix.length() && s.find(prefix) == 0;
      });
}

static bool stringStartsWith(const std::string &s,
                             const std::vector<char> &prefixList) {
  return std::all_of(prefixList.begin(), prefixList.end(),
                     [&](const char &prefix) {
                       return s.length() >= 1 && s.find(prefix) == 0;
                     });
}

static bool stringEndsWith(const std::string &s,
                           const std::vector<std::string> &postfixList) {
  return std::all_of(
      postfixList.begin(), postfixList.end(), [&](const std::string &postfix) {
        return s.length() >= postfix.length() && s.find(postfix) == 0;
      });
}

static bool stringEndsWith(const std::string &s,
                           const std::vector<char> &postfixList) {
  return std::all_of(postfixList.begin(), postfixList.end(),
                     [&](const char &postfix) {
                       return s.length() >= 1 && s.find(postfix) == 0;
                     });
}

static std::string stringReplace(const std::string &s, char from, char to) {
  std::string r(s);
  std::replace(r.begin(), r.end(), from, to);
  return r;
}

static std::string stringTrim(const std::string &s, int n) {
  return s.length() > n ? s.substr(0, n) : s;
}

// string utility }

// AstNamely {

AstNamely::AstNamely(const std::string &name) : name_(Name::get(name)) {}

const Name &AstNamely::name() const { return name_; }

// AstNamely }

// AstPositional {

AstPositional::AstPositional() : position_() {}

AstPositional::AstPositional(const Position &position) : position_(position) {}

const Position &AstPositional::position() const { return position_; }

void AstPositional::locate(const Position &position) {
  position_.update(position);
}

// AstPositional }

// Ast {

Ast::Ast(const std::string &name)
    : AstNamely(name), AstPositional(Position()) {}

Ast::Ast(const std::string &name, const Position &position)
    : AstNamely(name), AstPositional(position) {}

// Ast }

// literal {

// A_Integer {

static const std::unordered_map<A_Integer::DecimalCategory, std::string>
    AIL_DC_Map = {
        {A_Integer::DecimalCategory::DEC, "DEC"},
        {A_Integer::DecimalCategory::HEX, "HEX"},
        {A_Integer::DecimalCategory::BIN, "BIN"},
        {A_Integer::DecimalCategory::OCT, "OCT"},
};

static const std::unordered_map<A_Integer::BitCategory, std::string>
    AIL_BC_Map = {
        {A_Integer::BitCategory::SIGNED, "SIGNED"},
        {A_Integer::BitCategory::UNSIGNED, "UNSIGNED"},
        {A_Integer::BitCategory::LONG, "LONG"},
        {A_Integer::BitCategory::ULONG, "ULONG"},
};

A_Integer::A_Integer(const std::string &literal, const Position &position)
    : AstExpr(literal, position), literal_(literal) {
  EX_ASSERT(literal_.length() > 0, "literal.length {} > 0", literal_.length());

  int startPosition = 0;
  if (stringStartsWith(literal_, std::vector<std::string>{"0x", "0X", "0o",
                                                          "0O", "0b", "0B"})) {
    switch (literal_[1]) {
    case 'x':
    case 'X':
      decimalCategory_ = A_Integer::DecimalCategory::HEX;
      base_ = 16;
      startPosition = 2;
      break;
    case 'o':
    case 'O':
      decimalCategory_ = A_Integer::DecimalCategory::OCT;
      base_ = 8;
      startPosition = 2;
      break;
    case 'b':
    case 'B':
      decimalCategory_ = A_Integer::DecimalCategory::BIN;
      base_ = 2;
      startPosition = 2;
      break;
    default:
      decimalCategory_ = A_Integer::DecimalCategory::DEC;
      base_ = 10;
      startPosition = 0;
      break;
    }
  }

  int endPosition = (int)literal_.length();
  if (stringEndsWith(literal_, std::vector<std::string>{"ul", "UL"})) {
    bitCategory_ = A_Integer::BitCategory::ULONG;
    bits_ = 64;
    endPosition = (int)literal_.length() - 2;
  } else if (stringEndsWith(literal_, std::vector<char>{'l', 'L'})) {
    bitCategory_ = A_Integer::BitCategory::LONG;
    bits_ = 64;
    endPosition = (int)literal_.length() - 1;
  } else if (stringEndsWith(literal_, std::vector<char>{'u', 'U'})) {
    bitCategory_ = A_Integer::BitCategory::UNSIGNED;
    bits_ = 32;
    endPosition = (int)literal_.length() - 1;
  } else {
    bitCategory_ = A_Integer::BitCategory::SIGNED;
    bits_ = 32;
    endPosition = (int)literal_.length();
  }

  parsed_ = literal_.substr(startPosition, endPosition - startPosition);
}

AstCategory A_Integer::category() const { return AstCategory::Integer; }

std::string A_Integer::toString() const {
  return fmt::format("[{} position:{}, literal:{}, parsed:{}, "
                     "decimalCategory:{}, bitCategory:{}]",
                     name().toSymbolName(), position().toString(), literal_,
                     parsed_, AIL_DC_Map.find(decimalCategory_)->second,
                     AIL_BC_Map.find(bitCategory_)->second);
}

const std::string &A_Integer::parsed() const { return parsed_; }

int A_Integer::bits() const { return bits_; }

int A_Integer::base() const { return base_; }

A_Integer::DecimalCategory A_Integer::decimalCategory() const {
  return decimalCategory_;
}

A_Integer::BitCategory A_Integer::bitCategory() const { return bitCategory_; }

const std::string &A_Integer::literal() const { return literal_; }

int32_t A_Integer::toInt32() const {
  return static_cast<int32_t>(std::stol(parsed_, nullptr, base_));
}

uint32_t A_Integer::toUInt32() const {
  return static_cast<uint32_t>(std::stoul(parsed_, nullptr, base_));
}

int64_t A_Integer::toInt64() const {
  return static_cast<int64_t>(std::stoll(parsed_, nullptr, base_));
}

uint64_t A_Integer::toUInt64() const {
  return static_cast<uint64_t>(std::stoull(parsed_, nullptr, base_));
}

// A_Integer }

// A_FloatingPointLiteral {

const static std::unordered_map<A_FloatingPointLiteral::BitCategory,
                                std::string>
    AFL_BC_Map = {
        {A_FloatingPointLiteral::BitCategory::FLT, "FLT"},
        {A_FloatingPointLiteral::BitCategory::DBL, "DBL"},
};

A_FloatingPointLiteral::A_FloatingPointLiteral(const std::string &literal,
                                               const Position &position)
    : AstExpr(literal, position), literal_(literal) {
  EX_ASSERT(literal_.length() > 0, "literal_.length {} > 0", literal_.length());

  int startPosition = 0;
  int endPosition = (int)literal_.length();
  if (stringEndsWith(literal_, std::vector<std::string>{"f", "F"})) {
    bitCategory_ = A_FloatingPointLiteral::BitCategory::FLT;
    bits_ = 32;
    endPosition = (int)literal_.length() - 1;
  } else {
    bitCategory_ = A_FloatingPointLiteral::BitCategory::DBL;
    bits_ = 64;
    endPosition = (int)literal_.length();
  }

  parsed_ = literal_.substr(startPosition, endPosition - startPosition);
}

AstCategory A_FloatingPointLiteral::category() const {
  return AstCategory::FloatingPointLiteral;
}

std::string A_FloatingPointLiteral::toString() const {
  return fmt::format("[{} position:{}, literal:{}, parsed:{}, bitCategory:{}]",
                     name().toSymbolName(), position().toString(), literal_,
                     parsed_, AFL_BC_Map.find(bitCategory_)->second);
}

const std::string &A_FloatingPointLiteral::parsed() const { return parsed_; }

int A_FloatingPointLiteral::bits() const { return bits_; }

A_FloatingPointLiteral::BitCategory
A_FloatingPointLiteral::bitCategory() const {
  return bitCategory_;
}

const std::string &A_FloatingPointLiteral::literal() const { return literal_; }

float A_FloatingPointLiteral::toFloat() const { return std::stof(parsed_); }

double A_FloatingPointLiteral::toDouble() const { return std::stod(parsed_); }

// A_FloatingPointLiteral }

// A_StringLiteral {

const static std::unordered_map<A_StringLiteral::QuoteCategory, std::string>
    ASL_QC_Map = {
        {A_StringLiteral::QuoteCategory::SINGLE, "SINGLE"},
        {A_StringLiteral::QuoteCategory::TRIPLE, "TRIPLE"},
};

A_StringLiteral::A_StringLiteral(const std::string &literal,
                                 const Position &position)
    : Ast(stringTrim(stringReplace(literal, '\"', '-'), 8), position),
      literal_(literal),
      parsed_(
          literal.length() >= 3 &&
                  stringStartsWith(literal, std::vector<std::string>{"\"\"\""})
              ? literal.substr(3, literal.length() - 6)
              : literal.substr(1, literal.length() - 2)),
      quoteCategory_(
          literal.length() >= 3 &&
                  stringStartsWith(literal, std::vector<std::string>{"\"\"\""})
              ? A_StringLiteral::QuoteCategory::TRIPLE
              : A_StringLiteral::QuoteCategory::SINGLE) {}

AstCategory A_StringLiteral::category() const {
  return AstCategory::StringLiteral;
}

std::string A_StringLiteral::toString() const {
  return fmt::format("[{} position:{}, parsed:{}, literal:{}]",
                     name().toSymbolName(), position().toString(), parsed_,
                     literal_);
}

A_StringLiteral::QuoteCategory A_StringLiteral::quoteCategory() const {
  return quoteCategory_;
}

const std::string &A_StringLiteral::parsed() const { return parsed_; }

const std::string &A_StringLiteral::literal() const { return literal_; }

// A_StringLiteral }

// A_CharacterLiteral {

A_CharacterLiteral::A_CharacterLiteral(const std::string &literal,
                                       const Position &position)
    : Ast(stringReplace(literal, '\'', '-'), position), literal_(literal),
      parsed_(literal[1]) {}

AstCategory A_CharacterLiteral::category() const {
  return AstCategory::CharacterLiteral;
}

std::string A_CharacterLiteral::toString() const {
  return fmt::format("[{} position:{}, parsed:{}]", name().toSymbolName(),
                     position().toString(), parsed_);
}

const std::string &A_CharacterLiteral::literal() const { return literal_; }

char A_CharacterLiteral::parsed() const { return parsed_; }

// A_CharacterLiteral }

// A_BooleanLiteral {

A_BooleanLiteral::A_BooleanLiteral(const std::string &literal,
                                   const Position &position)
    : Ast(literal, position), literal_(literal),
      parsed_(literal == tokenName(T_TRUE)) {}

AstCategory A_BooleanLiteral::category() const {
  return AstCategory::BooleanLiteral;
}

std::string A_BooleanLiteral::toString() const {
  return fmt::format("[{} position:{}, literal:{}, parsed:{}]",
                     name().toSymbolName(), position().toString(), literal_,
                     parsed_);
}

const std::string &A_BooleanLiteral::literal() const { return literal_; }

bool A_BooleanLiteral::parsed() const { return parsed_; }

// A_BooleanLiteral }

// A_NilLiteral {

A_NilLiteral::A_NilLiteral(const Position &position)
    : Ast(tokenName(T_NIL), position) {}

AstCategory A_NilLiteral::category() const { return AstCategory::NilLiteral; }

std::string A_NilLiteral::toString() const {
  return fmt::format("[{} position:{}]", name().toSymbolName(),
                     position().toString());
}

// A_NilLiteral }

// A_VoidLiteral {

A_VoidLiteral::A_VoidLiteral(const Position &position)
    : Ast(tokenName(T_VOID), position) {}

AstCategory A_VoidLiteral::category() const { return AstCategory::VoidLiteral; }

std::string A_VoidLiteral::toString() const {
  return fmt::format("[{} position:{}]", name().toSymbolName(),
                     position().toString());
}

// A_VoidLiteral }

// literal }

// id {

// A_VarId {

A_VarId::A_VarId(const std::string &literal, const Position &position)
    : AstId(literal, position) {}

AstCategory A_VarId::category() const { return AstCategory::VarId; }

std::string A_VarId::toString() const {
  return fmt::format("[{} position:{}]", name().toSymbolName(),
                     position().toString());
}

// A_VarId }

// id }

// expression {

AstExpr::AstExpr(const std::string &name) : Ast(name) {}

AstExpr::AstExpr(const std::string &name, const Position &position)
    : Ast(name, position) {}

// A_ThrowExpression {

A_ThrowExpression::A_ThrowExpression(const AstExpression *expression,
                                     const Position &position)
    : AstExpression(nameGen.generate("A_ThrowExpression"), extra),
      expression_(expression) {
  EX_ASSERT(expression_, "expression_ must not null");
  locate(expression_->position());
}

A_ThrowExpression::~A_ThrowExpression() {
  delete expression_;
  expression_ = nullptr;
}

AstCategory A_ThrowExpression::category() const {
  return AstCategory::ThrowExpression;
}

std::string A_ThrowExpression::toString() const {
  return fmt::format("[@{} position:{} expression:{}]", name(),
                     position().toString(), expression_->toString());
}

const AstExpression *A_ThrowExpression::expression() const {
  return expression_;
}

// A_ThrowExpression }

// A_ReturnExpression {

A_ReturnExpression::A_ReturnExpression(const AstExpression *expression,
                                       const Position &extra)
    : AstExpression(nameGen.generate("A_ReturnExpression"), extra),
      expression_(expression) {
  EX_ASSERT(expression_, "expression_ must not null");
  locate(expression_->position());
}

A_ReturnExpression::~A_ReturnExpression() {
  delete expression_;
  expression_ = nullptr;
}

AstCategory A_ReturnExpression::category() const {
  return AstCategory::ReturnExpression;
}

std::string A_ReturnExpression::toString() const {
  return fmt::format("[@{} position:{} expression:{}]", name(),
                     position().toString(), expression_->toString());
}

const AstExpression *A_ReturnExpression::expression() const {
  return expression_;
}

// A_ReturnExpression }

// A_AssignExpression {

A_AssignExpression::A_AssignExpression(const AstId *id,
                                       const AstId *equalOperator,
                                       const AstExpression *expression)
    : AstExpression(nameGen.generate("A_AssignExpression")), id_(id),
      equalOperator_(equalOperator), expression_(expression) {
  EX_ASSERT(id_, "id_ must not null");
  EX_ASSERT(equalOperator_, "equalOperator_ must not null");
  EX_ASSERT(expression_, "expression_ must not null");
  locate(id_->position());
  locate(equalOperator_->position());
  locate(expression_->position());
}

A_AssignExpression::~A_AssignExpression() {
  delete id_;
  delete equalOperator_;
  delete expression_;
  id_ = nullptr;
  equalOperator_ = nullptr;
  expression_ = nullptr;
}

AstCategory A_AssignExpression::category() const {
  return AstCategory::AssignExpression;
}

std::string A_AssignExpression::toString() const {
  return fmt::format("[@{} position:{} id:{} equalOperator:{} expression:{}]",
                     name(), position().toString(), id_->toString(),
                     equalOperator_->toString(), expression_->toString());
}

const AstId *A_AssignExpression::id() const { return id_; }

const AstId *A_AssignExpression::equalOperator() const {
  return equalOperator_;
}

const AstExpression *A_AssignExpression::expression() const {
  return expression_;
}

// A_AssignExpression }

// A_PostfixExpression {

A_PostfixExpression::A_PostfixExpression(const AstExpression *expression,
                                         const AstId *postfixOperator)
    : AstExpression(nameGen.generate("A_PostfixExpression")),
      expression_(expression), postfixOperator_(postfixOperator) {
  EX_ASSERT(expression_, "expression_ must not null");
  EX_ASSERT(postfixOperator_, "postfixOperator_ must not null");
}

A_PostfixExpression::~A_PostfixExpression() {
  delete expression_;
  delete postfixOperator_;
  expression_ = nullptr;
  postfixOperator_ = nullptr;
}

AstCategory A_PostfixExpression::category() const {
  return AstCategory::PostfixExpression;
}

std::string A_PostfixExpression::toString() const {
  return fmt::format("[@{} position:{} expression:{} postfixOperator:{}]",
                     name(), position().toString(), expression_->toString(),
                     postfixOperator_->toString());
}

const AstExpression *A_PostfixExpression::expression() const {
  return expression_;
}

const AstId *A_PostfixExpression::postfixOperator() const {
  return postfixOperator_;
}

// A_PostfixExpression }

// A_InfixExpression {

A_InfixExpression::A_InfixExpression(const AstExpression *left,
                                     const AstId *infixOperator,
                                     const AstExpression *right)
    : AstExpression(nameGen.generate("A_InfixExpression")), left_(left),
      infixOperator_(infixOperator), right_(right) {
  EX_ASSERT(left_, "left_ must not null");
  EX_ASSERT(infixOperator_, "infixOperator_ must not null");
  EX_ASSERT(right_, "right_ must not null");
  locate(left_->position());
  locate(infixOperator_->position());
  locate(right_->position());
}

A_InfixExpression::~A_InfixExpression() {
  delete left_;
  delete infixOperator_;
  delete right_;
  left_ = nullptr;
  infixOperator_ = nullptr;
  right_ = nullptr;
}

AstCategory A_InfixExpression::category() const {
  return AstCategory::InfixExpression;
}

std::string A_InfixExpression::toString() const {
  return fmt::format("[@{} position:{} left:{} infixOperator:{} right:{}]",
                     name(), position().toString(), left_->toString(),
                     infixOperator_->toString(), right_->toString());
}

const AstExpression *A_InfixExpression::left() const { return left_; }

const AstId *A_InfixExpression::infixOperator() const { return infixOperator_; }

const AstExpression *A_InfixExpression::right() const { return right_; }

// A_InfixExpression }

// A_PrefixExpression {

A_PrefixExpression::A_PrefixExpression(const AstId *prefixOperator,
                                       const AstExpression *expression)
    : AstExpression(nameGen.generate("A_PrefixExpression")),
      prefixOperator_(prefixOperator), expression_(expression) {
  EX_ASSERT(prefixOperator_, "prefixOperator_ must not null");
  EX_ASSERT(expression_, "expression_ must not null");
  locate(prefixOperator_->position());
  locate(expression_->position());
}

A_PrefixExpression::~A_PrefixExpression() {
  delete prefixOperator_;
  delete expression_;
  prefixOperator_ = nullptr;
  expression_ = nullptr;
}

AstCategory A_PrefixExpression::category() const {
  return AstCategory::PrefixExpression;
}

std::string A_PrefixExpression::toString() const {
  return fmt::format("[@{} position:{} prefixOperator:{} expression:{}]",
                     name(), position().toString(), prefixOperator_->toString(),
                     expression_->toString());
}

const AstId *A_PrefixExpression::prefixOperator() const {
  return prefixOperator_;
}

const AstExpression *A_PrefixExpression::expression() const {
  return expression_;
}

// A_PrefixExpression }

// expression }

// type {

// AstType {

AstType::AstType(const std::string &name, const Position &position)
    : AstNamely(name), AstPositional(position) {}

// AstType}

// A_FunctionType {

A_FunctionType::A_FunctionType(const std::string &literal,
                               const Position &position)
    : AstType(nameGen.generateWith("A_FunctionType", literal), position) {}

// A_FunctionType }

// A_PrimitiveType {

A_PrimitiveType::A_PrimitiveType(const std::string &literal,
                                 const Position &position)
    : AstType(nameGen.generateWith("A_PrimitiveType", literal), position),
      literal_(literal) {}

AstCategory A_PrimitiveType::category() const {
  return AstCategory::PrimitiveType;
}

std::string A_PrimitiveType::toString() const {
  return fmt::format("[@{} position:{}, name:{}, literal:{}]", name(),
                     position().toString(), name(), literal_);
}

const std::string &A_PrimitiveType::literal() const { return literal_; }

const A_PrimitiveType *A_PrimitiveType::get(int token,
                                            const Position &position) {
  switch (token) {
  case T_INT8:
  case T_UINT8:
  case T_INT16:
  case T_UINT16:
  case T_INT32:
  case T_UINT32:
  case T_INT64:
  case T_UINT64:
  case T_FLOAT32:
  case T_FLOAT64:
  case T_BOOLEAN:
    return new A_PrimitiveType(tokenName(token), position);
  default:
    EX_ASSERT(false, "invalid token:{} {}", tokenName(token), token);
    break;
  }
  return nullptr;
}

// A_PrimitiveType }

// type }

// expression {

// AstExpression {

AstExpression::AstExpression(const std::string &name)
    : AstNamely(name), AstPositional(Position()) {}

AstExpression::AstExpression(const std::string &name, const Position &position)
    : AstNamely(name), AstPositional(position) {}

// AstExpression }

A_CallExpression::A_CallExpression(const AstId *id,
                                   const A_ExpressionList *argumentList,
                                   const Position &idPosition)
    : AstExpression(nameGen.generate("A_CallExpression"), idPosition), id_(id),
      argumentList_(argumentList) {
  EX_ASSERT(id_, "id_ must not null");
  EX_ASSERT(argumentList_, "argumentList_ must not null");
  locate(id_->position());
  locate(argumentList_->position());
}

A_CallExpression::~A_CallExpression() {
  delete id_;
  delete argumentList_;
  id_ = nullptr;
  argumentList_ = nullptr;
}

AstCategory A_CallExpression::category() const {
  return AstCategory::CallExpression;
}

std::string A_CallExpression::toString() const {
  return fmt::format("[@{} position:{}, id:{}, argumentList:{}]", name(),
                     position().toString(), id_->toString(),
                     argumentList_->toString());
}

const AstId *A_CallExpression::id() const { return id_; }

const A_ExpressionList *A_CallExpression::argumentList() const {
  return argumentList_;
}

// A_UnaryExpression {

A_UnaryExpression::A_UnaryExpression(const AstToken *token,
                                     const AstExpression *expression)
    : AstExpression(nameGen.generate("A_UnaryExpression")), token_(token),
      expression_(expression) {
  EX_ASSERT(token_, "token_ must not null");
  EX_ASSERT(expression_, "expression_ must not null");
  locate(token_->position());
  locate(expression_->position());
}

A_UnaryExpression::~A_UnaryExpression() {
  delete token_;
  token_ = nullptr;
  delete expression_;
  expression_ = nullptr;
}

AstCategory A_UnaryExpression::category() const {
  return AstCategory::UnaryExpression;
}

std::string A_UnaryExpression::toString() const {
  return fmt::format("[@{} position:{}, token:{}, expression:{}]", name(),
                     position().toString(), token_->name(),
                     expression_->toString());
}

const AstToken *A_UnaryExpression::token() const { return token_; }

const AstExpression *A_UnaryExpression::expression() const {
  return expression_;
}

// A_UnaryExpression }

// A_BinaryExpression {

A_BinaryExpression::A_BinaryExpression(const AstExpression *left,
                                       const AstToken *token,
                                       const AstExpression *right)
    : AstExpression(nameGen.generate("A_BinaryExpression")), left_(left),
      token_(token), right_(right) {
  EX_ASSERT(left_, "left_ must not null");
  EX_ASSERT(token_, "token_ must not null");
  EX_ASSERT(right_, "right_ must not null");
  locate(left_->position());
  locate(token_->position());
  locate(right_->position());
}

A_BinaryExpression::~A_BinaryExpression() {
  delete left_;
  left_ = nullptr;
  delete token_;
  token_ = nullptr;
  delete right_;
  right_ = nullptr;
}

AstCategory A_BinaryExpression::category() const {
  return AstCategory::BinaryExpression;
}

std::string A_BinaryExpression::toString() const {
  return fmt::format("[@{} position:{}, token:{}, left:{}, right:{}]", name(),
                     position().toString(), token_->name(), left_->toString(),
                     right_->toString());
}

const AstExpression *A_BinaryExpression::left() const { return left_; }

const AstToken *A_BinaryExpression::token() const { return token_; }

const AstExpression *A_BinaryExpression::right() const { return right_; }

// A_BinaryExpression }

// A_ConditionalExpression {

A_ConditionalExpression::A_ConditionalExpression(const AstExpression *condition,
                                                 const AstExpression *thens,
                                                 const AstExpression *elses)
    : AstExpression(nameGen.generate("A_ConditionalExpression")),
      condition_(condition), thens_(thens), elses_(elses) {
  EX_ASSERT(condition_, "condition_ must not null");
  EX_ASSERT(thens_, "thens_ must not null");
  EX_ASSERT(elses_, "elses_ must not null");
}

A_ConditionalExpression::~A_ConditionalExpression() {
  delete condition_;
  condition_ = nullptr;
  delete thens_;
  thens_ = nullptr;
  delete elses_;
  elses_ = nullptr;
}

AstCategory A_ConditionalExpression::category() const {
  return AstCategory::ConditionalExpression;
}

std::string A_ConditionalExpression::toString() const {
  return fmt::format("[@{} position:{}, condition:{}, thens:{}, elses:{}]",
                     name(), position().toString(), condition_->toString(),
                     thens_->toString(), elses_->toString());
}

const AstExpression *A_ConditionalExpression::condition() const {
  return condition_;
}

const AstExpression *A_ConditionalExpression::thens() const { return thens_; }

const AstExpression *A_ConditionalExpression::elses() const { return elses_; }

// A_ConditionalExpression }

// A_AssigmentExpression {

A_AssignmentExpression::A_AssignmentExpression(const AstExpression *left,
                                               int token,
                                               const AstExpression *right,
                                               const Position &tokenPosition)
    : AstExpression(nameGen.generate("A_AssignmentExpression"), tokenPosition),
      left_(left), token_(token), right_(right) {
  EX_ASSERT(left_, "left_ must not null");
  EX_ASSERT(right_, "right_ must not null");
  EX_ASSERT(tokenNameExist(token_), "invalid token:{}", token_);
  locate(left_->position());
  locate(right_->position());
}

A_AssignmentExpression::~A_AssignmentExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

AstCategory A_AssignmentExpression::category() const {
  return AstCategory::AssignmentExpression;
}

std::string A_AssignmentExpression::toString() const {
  return fmt::format("[@{} position:{} left:{}, token:{}, right:{}]", name(),
                     position().toString(), left_->toString(),
                     tokenName(token_), right_->toString());
}

const AstExpression *A_AssignmentExpression::left() const { return left_; }

int A_AssignmentExpression::token() const { return token_; }

const AstExpression *A_AssignmentExpression::right() const { return right_; }

// A_AssigmentExpression }

// A_VoidExpression {

A_VoidExpression::A_VoidExpression()
    : AstExpression(nameGen.generate("A_VoidExpression")) {}

AstCategory A_VoidExpression::category() const {
  return AstCategory::VoidExpression;
}

std::string A_VoidExpression::toString() const {
  return fmt::format("[@{}]", name());
}

// A_VoidExpression }

// expression }

// statement {

AstStatement::AstStatement(const std::string &name)
    : AstNamely(name), AstPositional(Position()) {}

AstStatement::AstStatement(const std::string &name, const Position &position)
    : AstNamely(name), AstPositional(position) {}

// statement }

AstDefinition::AstDefinition(const std::string &name) : AstStatement(name) {}

AstDefinition::AstDefinition(const std::string &name, const Position &position)
    : AstStatement(name, position) {}

AstStatementList::AstStatementList()
    : detail::AstList<AstStatement>(nameGen.generate("A_StmList")) {}

std::string AstStatementList::stringify() const { return "AstStatementList"; }

AstCategory AstStatementList::category() const {
  return AstCategory::StatementList;
}

AstDefinitionList::AstDefinitionList()
    : detail::AstList<AstDefinition>(nameGen.generate("A_DeclList")) {}

std::string AstDefinitionList::stringify() const { return "AstDefinitionList"; }

AstCategory AstDefinitionList::category() const {
  return AstCategory::DefinitionList;
}

AstTranslateUnit::AstTranslateUnit()
    : detail::AstList<AstDefinition>(nameGen.generate("A_TUnit")) {}

AstCategory AstTranslateUnit::category() const {
  return AstCategory::TranslateUnit;
}

std::string AstTranslateUnit::stringify() const { return "AstTranslateUnit"; }

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

AstCategory AstExpressionStatement::category() const {
  return AstCategory::ExpressionStatement;
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

AstCategory AstCompoundStatement::category() const {
  return AstCategory::CompoundStatement;
}

std::string AstCompoundStatement::toString() const {
  return fmt::format("[@AstCompoundStatement {} statementList_:{}]",
                     position_.toString(), statementList_->toString());
}

AstStatementList *AstCompoundStatement::statementList() const {
  return statementList_;
}

A_IfStatement::A_IfStatement(AstExpression *condition, AstStatement *thens,
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

A_IfStatement::~A_IfStatement() {
  delete condition_;
  condition_ = nullptr;
  delete thens_;
  thens_ = nullptr;
  delete elses_;
  elses_ = nullptr;
}

AstCategory A_IfStatement::category() const { return AstCategory::IfStatement; }

std::string A_IfStatement::toString() const {
  return fmt::format("[@A_IfStatement {} condition_:{}, thens_:{}, elses_:{}]",
                     position_.toString(), condition_->toString(),
                     thens_->toString(), elses_->toString());
}

AstExpression *A_IfStatement::condition() const { return condition_; }

AstStatement *A_IfStatement::thens() const { return thens_; }

AstStatement *A_IfStatement::elses() const { return elses_; }

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

AstCategory AstWhileStatement::category() const {
  return AstCategory::WhileStatement;
}

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

AstCategory AstForStatement::category() const {
  return AstCategory::ForStatement;
}

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

AstCategory AstContinueStatement::category() const {
  return AstCategory::ContinueStatement;
}

std::string AstContinueStatement::toString() const {
  return fmt::format("[@AstContinueStatement {}]", position_.toString());
}

AstBreakStatement::AstBreakStatement(const Position &breakTokenPosition,
                                     const Position &semiTokenPosition)
    : AstStatement(nameGen.generate("A_break"), breakTokenPosition) {
  position_.updatePosition(semiTokenPosition);
}

AstCategory AstBreakStatement::category() const {
  return AstCategory::BreakStatement;
}

std::string AstBreakStatement::toString() const {
  return fmt::format("[@AstBreakStatement {}]", position_.toString());
}

AstEmptyStatement::AstEmptyStatement()
    : AstStatement(nameGen.generate("A_EmpStm")) {}

AstEmptyStatement::AstEmptyStatement(const Position &semiTokenPosition)
    : AstStatement(nameGen.generate("A_EmpStm"), semiTokenPosition) {}

AstCategory AstEmptyStatement::category() const {
  return AstCategory::EmptyStatement;
}

std::string AstEmptyStatement::toString() const {
  return fmt::format("[@AstEmptyStatement {}]", position_.toString());
}

AstVariableDefinition::AstVariableDefinition(const char *varId,
                                             const char *typeId,
                                             AstExpression *expression,
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

AstCategory AstVariableDefinition::category() const {
  return AstCategory::VariableDefinition;
}

std::string AstVariableDefinition::toString() const {
  return fmt::format("[@AstVariableDefinition {} definitionList_:{}]",
                     position_.toString(), definitionList_->toString());
}

AstDefinitionList *AstVariableDefinition::definitionList() const {
  return definitionList_;
}

AstVariableInitialDefinition::AstVariableInitialDefinition(
    const char *identifier, AstIdentifier *typeId, AstExpression *expression,
    const Position &identifierPosition, const Position &typePosition)
    : AstDefinition(nameGen.generate("A_VarAssDecl"), identifierPosition),
      identifier_(identifier), typeId_(typeId), expression_(expression) {
  EX_ASSERT(identifier, "identifier must not null");
  EX_ASSERT(typeId_, "typeId_ must not null");
  EX_ASSERT(expression_, "expression_ must not null");
  position_.updatePosition(typePosition);
  position_.updatePosition(expression_->position());
}

AstVariableInitialDefinition::~AstVariableInitialDefinition() {
  delete expression_;
  expression_ = nullptr;
}

AstCategory AstVariableInitialDefinition::category() const {
  return AstCategory::VariableInitialDefinition;
}

std::string AstVariableInitialDefinition::toString() const {
  return fmt::format(
      "[@AstVariableInitialDefinition {} identifier_:{}, expression_:{}]",
      position_.toString(), identifier_, expression_->toString());
}

const std::string &AstVariableInitialDefinition::identifier() const {
  return identifier_;
}

AstIdentifier *AstVariableInitialDefinition::typeId() const { return typeId_; }

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

AstCategory AstFunctionDefinition::category() const {
  return AstCategory::FunctionDefinition;
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

AstCategory AstFunctionSignatureDefinition::category() const {
  return AstCategory::FunctionSignatureDefinition;
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

AstCategory AstFunctionArgumentDefinition::category() const {
  return AstCategory::FunctionArgumentDefinition;
}

std::string AstFunctionArgumentDefinition::toString() const {
  return fmt::format("[@AstFunctionArgumentDefinition {} identifier_:{}]",
                     position_.toString(), identifier_);
}

const std::string &AstFunctionArgumentDefinition::identifier() const {
  return identifier_;
}
