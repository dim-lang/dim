// Copyright 2019- <shepherd-lang>
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

// { Ast

Ast::Ast(const std::string &name) : name_(name) {}

const std::string &Ast::name() const { return name_; }

const Position &Ast::position() const { return position_; }

void Ast::collect(const Position &position) {
  position_.updatePosition(position);
}

void Ast::collect(const Ast *node) { collect(node->position()); }

// }

// id {

// AstId {

AstId::AstId(const std::string &name) : Ast(name) {}

// AstId }

// A_PlainId {

A_PlainId::A_PlainId(const std::string &literal, const Position &position)
    : AstId(nameGen.generateWith(literal, "A_PlainId")), literal_(literal) {
  collect(position);
}

AstCategory A_PlainId::category() const { return AstCategory::PlainId; }

std::string A_PlainId::toString() const {
  return fmt::format("[@A_PlainId position:{}, name:{}, literal:{}]",
                     position().toString(), name(), literal_);
}

const std::string &A_PlainId::literal() const { return literal_; }

// A_PlainId }

// A_StableId {

/* A_StableId::A_StableId(const std::string &literal, const Position &position)
 */
/*     : AstId(nameGen.generateWith(literal, "A_StableId")), literal_(literal) {
 */
/*   collect(position); */
/* } */

/* AstCategory A_StableId::category() const { return AstCategory::StableId; } */

/* std::string A_StableId::toString() const { */
/*   return fmt::format("[@A_StableId position:{}, name:{}, literal:{}]", */
/*                      position().toString(), name(), literal_); */
/* } */

/* const std::string &A_StableId::literal() const { return literal_; } */

/* const A_StableId *A_StableId::get(int token, const Position &position) { */
/*   switch (token) { */
/*   case T_INT8: */
/*   case T_UINT8: */
/*   case T_INT16: */
/*   case T_UINT16: */
/*   case T_INT32: */
/*   case T_UINT32: */
/*   case T_INT64: */
/*   case T_UINT64: */
/*   case T_INT128: */
/*   case T_UINT128: */
/*   case T_FLOAT32: */
/*   case T_FLOAT64: */
/*   case T_NAN: */
/*   case T_INF: */
/*   case T_BOOLEAN: */
/*   case T_NIL: */
/*   case T_VOID: */
/*     return new A_StableId(tokenName(token), position); */
/*   default: */
/*     EX_ASSERT(false, "invalid token:{}, tokenName:{}", token,
 * tokenName(token)); */
/*     break; */
/*   } */
/*   return nullptr; */
/* } */

// A_StableId }

// id }

// type {

// AstType {

AstType::AstType(const std::string &name) : Ast(name) {}

// AstType }

// A_FunctionType {

A_FunctionType::A_FunctionType(const std::string &name) : AstType(name) {}

// A_FunctionType }

// A_PrimitiveType {

A_PrimitiveType::A_PrimitiveType(const std::string &literal,
                                 const Position &position)
    : AstType(nameGen.generateWith(literal, "A_PrimitiveType")),
      literal_(literal) {
  collect(position);
}

AstCategory A_PrimitiveType::category() const {
  return AstCategory::PrimitiveType;
}

std::string A_PrimitiveType::toString() const {
  return fmt::format("[@A_PrimitiveType position:{}, name:{}, literal:{}]",
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

AstExpression::AstExpression(const std::string &name) : Ast(name) {}

// literal {

AstLiteral::AstLiteral(const std::string &name) : AstExpression(name) {}

// A_IntegerLiteral {

static const std::unordered_map<A_IntegerLiteral::DecimalCategory, std::string>
    AIL_DC_Map = {
        {A_IntegerLiteral::DecimalCategory::DEC, "DEC"},
        {A_IntegerLiteral::DecimalCategory::HEX, "HEX"},
        {A_IntegerLiteral::DecimalCategory::BIN, "BIN"},
        {A_IntegerLiteral::DecimalCategory::OCT, "OCT"},
};

static const std::unordered_map<A_IntegerLiteral::BitCategory, std::string>
    AIL_BC_Map = {
        {A_IntegerLiteral::BitCategory::SIGNED, "SIGNED"},
        {A_IntegerLiteral::BitCategory::UNSIGNED, "UNSIGNED"},
        {A_IntegerLiteral::BitCategory::LONG, "LONG"},
        {A_IntegerLiteral::BitCategory::ULONG, "ULONG"},
};

A_IntegerLiteral::A_IntegerLiteral(const std::string &literal,
                                   const Position &position)
    : AstLiteral(nameGen.generateWith(literal, "A_IntegerLiteral")),
      literal_(literal), parsed_(literal), bits_(32), base_(10),
      decimalCategory_(A_IntegerLiteral::DecimalCategory::DEC),
      bitCategory_(A_IntegerLiteral::BitCategory::SIGNED) {
  EX_ASSERT(literal_.length() > 0, "literal.length {} > 0", literal_.length());

  // position
  collect(position);

  int startPosition = 0;
  if (stringStartsWith(literal_, std::vector<std::string>{"0x", "0X", "0o",
                                                          "0O", "0b", "0B"})) {
    switch (literal_[1]) {
    case 'x':
    case 'X':
      decimalCategory_ = A_IntegerLiteral::DecimalCategory::HEX;
      base_ = 16;
      startPosition = 2;
      break;
    case 'o':
    case 'O':
      decimalCategory_ = A_IntegerLiteral::DecimalCategory::OCT;
      base_ = 8;
      startPosition = 2;
      break;
    case 'b':
    case 'B':
      decimalCategory_ = A_IntegerLiteral::DecimalCategory::BIN;
      base_ = 2;
      startPosition = 2;
      break;
    default:
      break;
    }
  }

  int endPosition = (int)literal_.length();
  if (stringEndsWith(literal_, std::vector<std::string>{"ul", "UL"})) {
    bitCategory_ = A_IntegerLiteral::BitCategory::ULONG;
    bits_ = 64;
    endPosition = (int)literal_.length() - 2;
  } else if (stringEndsWith(literal_, std::vector<char>{'l', 'L'})) {
    bitCategory_ = A_IntegerLiteral::BitCategory::LONG;
    bits_ = 64;
    endPosition = (int)literal_.length() - 1;
  } else if (stringEndsWith(literal_, std::vector<char>{'u', 'U'})) {
    bitCategory_ = A_IntegerLiteral::BitCategory::UNSIGNED;
    bits_ = 32;
    endPosition = (int)literal_.length() - 1;
  }

  parsed_ = literal_.substr(startPosition, endPosition - startPosition);
}

AstCategory A_IntegerLiteral::category() const {
  return AstCategory::IntegerLiteral;
}

std::string A_IntegerLiteral::toString() const {
  return fmt::format("[@A_IntegerLiteral position:{}, literal:{}, parsed:{}, "
                     "decimalCategory:{}, bitCategory:{}]",
                     position().toString(), literal_, parsed_,
                     AIL_DC_Map.find(decimalCategory_)->second,
                     AIL_BC_Map.find(bitCategory_)->second);
}

const std::string &A_IntegerLiteral::parsed() const { return parsed_; }

int A_IntegerLiteral::bits() const { return bits_; }

int A_IntegerLiteral::base() const { return base_; }

A_IntegerLiteral::DecimalCategory A_IntegerLiteral::decimalCategory() const {
  return decimalCategory_;
}

A_IntegerLiteral::BitCategory A_IntegerLiteral::bitCategory() const {
  return bitCategory_;
}

const std::string &A_IntegerLiteral::literal() const { return literal_; }

int32_t A_IntegerLiteral::toInt32() const {
  return static_cast<int32_t>(std::stol(parsed_, nullptr, base_));
}

uint32_t A_IntegerLiteral::toUInt32() const {
  return static_cast<uint32_t>(std::stoul(parsed_, nullptr, base_));
}

int64_t A_IntegerLiteral::toInt64() const {
  return static_cast<int64_t>(std::stoll(parsed_, nullptr, base_));
}

uint64_t A_IntegerLiteral::toUInt64() const {
  return static_cast<uint64_t>(std::stoull(parsed_, nullptr, base_));
}

// A_IntegerLiteral }

// A_FloatLiteral {

const static std::unordered_map<A_FloatLiteral::DecimalCategory, std::string>
    AFL_DC_Map = {
        {A_FloatLiteral::DecimalCategory::DEC, "DEC"},
        {A_FloatLiteral::DecimalCategory::HEX, "HEX"},
};

const static std::unordered_map<A_FloatLiteral::BitCategory, std::string>
    AFL_BC_Map = {
        {A_FloatLiteral::BitCategory::FLT, "FLT"},
        {A_FloatLiteral::BitCategory::DBL, "DBL"},
};

A_FloatLiteral::A_FloatLiteral(const std::string &literal,
                               const Position &position)
    : AstLiteral(nameGen.generateWith(literal, "A_FloatLiteral")),
      literal_(literal), parsed_(literal), bits_(32), base_(10),
      decimalCategory_(A_FloatLiteral::DecimalCategory::DEC),
      bitCategory_(A_FloatLiteral::BitCategory::FLT) {
  EX_ASSERT(literal_.length() > 0, "literal_.length {} > 0", literal_.length());

  collect(position);

  int startPosition = 0;
  if (stringStartsWith(literal_, std::vector<std::string>{"0x", "0X"})) {
    switch (literal_[1]) {
    case 'x':
    case 'X':
      decimalCategory_ = A_FloatLiteral::DecimalCategory::HEX;
      base_ = 16;
      startPosition = 2;
      break;
    default:
      break;
    }
  }

  int endPosition = (int)literal_.length();
  if (stringEndsWith(literal_, std::vector<std::string>{"d", "D"})) {
    bitCategory_ = A_FloatLiteral::BitCategory::DBL;
    bits_ = 64;
    endPosition = (int)literal_.length() - 1;
  }

  parsed_ = literal_.substr(startPosition, endPosition - startPosition);

  size_t dot = parsed_.find('.');
  EX_ASSERT(dot >= 0 && dot < parsed_.length(),
            "0 <= dot {} < parsed_.length {}", dot, parsed_.length());
  std::string dotLeftLiteral = parsed_.substr(0, dot);
  std::string dotRightLiteral =
      parsed_.substr(dot + 1, parsed_.length() - dot - 1);
  uint64_t dotLeftDecimal = std::stoull(dotLeftLiteral, nullptr, base_);
  uint64_t dotRightDecimal = std::stoull(dotRightLiteral, nullptr, base_);
  parsed_ =
      std::to_string(dotLeftDecimal) + "." + std::to_string(dotRightDecimal);
}

AstCategory A_FloatLiteral::category() const {
  return AstCategory::FloatLiteral;
}

std::string A_FloatLiteral::toString() const {
  return fmt::format("[@A_FloatLiteral position:{}, literal:{}, parsed:{} "
                     "decimalCategory:{}, bitCategory:{}]",
                     position().toString(), literal_, parsed_,
                     AFL_DC_Map.find(decimalCategory_)->second,
                     AFL_BC_Map.find(bitCategory_)->second);
}

const std::string &A_FloatLiteral::parsed() const { return parsed_; }

int A_FloatLiteral::bits() const { return bits_; }

int A_FloatLiteral::base() const { return base_; }

A_FloatLiteral::DecimalCategory A_FloatLiteral::decimalCategory() const {
  return decimalCategory_;
}

A_FloatLiteral::BitCategory A_FloatLiteral::bitCategory() const {
  return bitCategory_;
}

const std::string &A_FloatLiteral::literal() const { return literal_; }

float A_FloatLiteral::toFloat() const { return std::stof(parsed_); }

double A_FloatLiteral::toDouble() const { return std::stod(parsed_); }

// A_FloatLiteral }

// A_StringLiteral {

const static std::unordered_map<A_StringLiteral::QuoteCategory, std::string>
    ASL_QC_Map = {
        {A_StringLiteral::QuoteCategory::SINGLE, "SINGLE"},
        {A_StringLiteral::QuoteCategory::TRIPLE, "TRIPLE"},
};

A_StringLiteral::A_StringLiteral(const std::string &literal,
                                 const Position &position)
    : AstLiteral(nameGen.generateWith(literal, "A_StringLiteral")), literals_(),
      parsed_(literal), quoteCategory_(A_StringLiteral::QuoteCategory::SINGLE) {
  EX_ASSERT(literal.length() >= 2, "literal.length {} >= 2", literal.length());

  collect(position);

  if (stringStartsWith(literal, std::vector<std::string>{"\"\"\""}) &&
      stringEndsWith(literal, std::vector<std::string>{"\"\"\""})) {
    quoteCategory_ = A_StringLiteral::QuoteCategory::TRIPLE;
    parsed_ = literal.substr(3, literal.length() - 6);
  } else {
    parsed_ = literal.substr(1, literal.length() - 2);
  }
  literals_.push_front(literal);
}

AstCategory A_StringLiteral::category() const {
  return AstCategory::StringLiteral;
}

std::string A_StringLiteral::toString() const {
  std::stringstream ss;
  ss << fmt::format(
      "[@A_StringLiteral position:{}, quoteCategory:{}, parsed:{}, size:{}, ",
      position().toString(), ASL_QC_Map.find(quoteCategory_)->second, parsed_,
      literals_.size());
  for (int i = 0; i < (int)literals_.size(); i++) {
    ss << fmt::format("{}:{}", i, literals_[i]);
    if (i < (int)literals_.size() - 1) {
      ss << ", ";
    }
  }
  ss << "]";
  return ss.str();
}

const std::string &A_StringLiteral::parsed() const { return parsed_; }

A_StringLiteral::QuoteCategory A_StringLiteral::quoteCategory() const {
  return quoteCategory_;
}

const std::deque<std::string> &A_StringLiteral::literals() const {
  return literals_;
}

void A_StringLiteral::join(const std::string &literal,
                           const Position &position) {
  EX_ASSERT(quoteCategory_ == A_StringLiteral::QuoteCategory::SINGLE,
            "quoteCategory {} == SINGLE",
            ASL_QC_Map.find(quoteCategory_)->second);
  EX_ASSERT(!stringStartsWith(literal, std::vector<std::string>{"\"\"\""}),
            "literal head {} cannot be a triple quote", literal);
  EX_ASSERT(!stringEndsWith(literal, std::vector<std::string>{"\"\"\""}),
            "literal tail {} cannot be a triple quote", literal);

  collect(position);
  literals_.push_front(literal);

  std::stringstream ss;
  std::string joiner = literal.substr(1, literal.length() - 2);
  ss << joiner << parsed_;
  parsed_ = ss.str();
}

// A_StringLiteral }

// A_CharLiteral {

A_CharLiteral::A_CharLiteral(const std::string &literal,
                             const Position &position)
    : AstLiteral(nameGen.generateWith(literal, "A_CharLiteral")),
      parsed_('\0') {
  EX_ASSERT(literal.length() == 3, "literal.length {} == 3", literal.length());

  collect(position);

  parsed_ = literal[1];
}

AstCategory A_CharLiteral::category() const { return AstCategory::CharLiteral; }

std::string A_CharLiteral::toString() const {
  return fmt::format("[@A_CharLiteral position:{}, parsed:{}]",
                     position().toString(), parsed_);
}

const char &A_CharLiteral::parsed() const { return parsed_; }

// A_CharLiteral }

// A_BooleanLiteral {

A_BooleanLiteral::A_BooleanLiteral(const std::string &literal,
                                   const Position &position)
    : AstLiteral(nameGen.generateWith(literal, "A_BooleanLiteral")),
      literal_(literal), parsed_(false) {
  EX_ASSERT(literal_.length() == 4 || literal_.length() == 5,
            "literal_.length {} == 4 or 5", literal_.length());

  collect(position);
  EX_ASSERT(literal_ == tokenName(T_TRUE) || literal_ == tokenName(T_FALSE),
            "literal_ {} must be true/false", literal_);
  parsed_ = literal_ == tokenName(T_TRUE);
}

AstCategory A_BooleanLiteral::category() const {
  return AstCategory::BooleanLiteral;
}

std::string A_BooleanLiteral::toString() const {
  return fmt::format("[@A_BooleanLiteral position:{}, literal:{}, parsed:{}]",
                     position().toString(), literal_, parsed_);
}

bool A_BooleanLiteral::parsed() const { return parsed_; }

const std::string &A_BooleanLiteral::literal() const { return literal_; }

// A_BooleanLiteral }

// A_NilLiteral {

A_NilLiteral::A_NilLiteral(const Position &position)
    : AstLiteral(nameGen.generate("A_NilLiteral")) {
  collect(position);
}

AstCategory A_NilLiteral::category() const { return AstCategory::NilLiteral; }

std::string A_NilLiteral::toString() const {
  return fmt::format("[@A_NilLiteral position:{}]", position().toString());
}

// A_NilLiteral }

// A_VoidLiteral {

A_VoidLiteral::A_VoidLiteral(const Position &position)
    : AstLiteral(nameGen.generate("A_VoidLiteral")) {
  collect(position);
}

AstCategory A_VoidLiteral::category() const { return AstCategory::VoidLiteral; }

std::string A_VoidLiteral::toString() const {
  return fmt::format("[@A_VoidLiteral position:{}]", position().toString());
}

// A_VoidLiteral }

// literal }

// expression }

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

A_ty AstExpressionList::type() const { return A_ty::ExpressionList; }

AstStatementList::AstStatementList()
    : detail::AstList<AstStatement>(nameGen.generate("A_StmList")) {}

std::string AstStatementList::stringify() const { return "AstStatementList"; }

A_ty AstStatementList::type() const { return A_ty::StatementList; }

AstDefinitionList::AstDefinitionList()
    : detail::AstList<AstDefinition>(nameGen.generate("A_DeclList")) {}

std::string AstDefinitionList::stringify() const { return "AstDefinitionList"; }

A_ty AstDefinitionList::type() const { return A_ty::DefinitionList; }

AstTranslateUnit::AstTranslateUnit()
    : detail::AstList<AstDefinition>(nameGen.generate("A_TUnit")) {}

A_ty AstTranslateUnit::type() const { return A_ty::TranslateUnit; }

std::string AstTranslateUnit::stringify() const { return "AstTranslateUnit"; }

AstIdentifier::AstIdentifier(const char *value, const Position &position)
    : AstExpression(nameGen.generateWith(value, "A_id"), position),
      value_(value) {
  EX_ASSERT(value, "value is null");
}

A_ty AstIdentifier::type() const { return A_ty::Identifier; }

std::string AstIdentifier::toString() const {
  return fmt::format("[@AstIdentifier {} value_:{}]", position_.toString(),
                     value_);
}

const std::string &AstIdentifier::value() const { return value_; }

AstInt8Literal::AstInt8Literal(const int8_t &value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_i8"), position), value_(value) {
}

A_ty AstInt8Literal::type() const { return A_ty::Int8Literal; }

std::string AstInt8Literal::toString() const {
  return fmt::format("[@AstInt8Literal {} value_:{}]", position_.toString(),
                     (int)value_);
}

const int8_t &AstInt8Literal::value() const { return value_; }

AstUInt8Literal::AstUInt8Literal(const uint8_t &value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_u8"), position), value_(value) {
}

A_ty AstUInt8Literal::type() const { return A_ty::UInt8Literal; }

std::string AstUInt8Literal::toString() const {
  return fmt::format("[@AstUInt8Literal {} value_:{}]", position_.toString(),
                     (int)value_);
}

const uint8_t &AstUInt8Literal::value() const { return value_; }

AstInt16Literal::AstInt16Literal(const int16_t &value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_i16"), position),
      value_(value) {}

A_ty AstInt16Literal::type() const { return A_ty::Int16Literal; }

std::string AstInt16Literal::toString() const {
  return fmt::format("[@AstInt16Literal {} value_:{}]", position_.toString(),
                     value_);
}

const int16_t &AstInt16Literal::value() const { return value_; }

AstUInt16Literal::AstUInt16Literal(const uint16_t &value,
                                   const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_u16"), position),
      value_(value) {}

A_ty AstUInt16Literal::type() const { return A_ty::UInt16Literal; }

std::string AstUInt16Literal::toString() const {
  return fmt::format("[@AstUInt16Literal {} value_:{}]", position_.toString(),
                     value_);
}

const uint16_t &AstUInt16Literal::value() const { return value_; }

AstInt32Literal::AstInt32Literal(const int32_t &value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_i32"), position),
      value_(value) {}

A_ty AstInt32Literal::type() const { return A_ty::Int32Literal; }

std::string AstInt32Literal::toString() const {
  return fmt::format("[@AstInt32Literal {} value_:{}]", position_.toString(),
                     value_);
}

const int32_t &AstInt32Literal::value() const { return value_; }

AstUInt32Literal::AstUInt32Literal(const uint32_t &value,
                                   const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_u32"), position),
      value_(value) {}

A_ty AstUInt32Literal::type() const { return A_ty::UInt32Literal; }

std::string AstUInt32Literal::toString() const {
  return fmt::format("[@AstUInt32Literal {} value_:{}]", position_.toString(),
                     value_);
}

const uint32_t &AstUInt32Literal::value() const { return value_; }

AstInt64Literal::AstInt64Literal(const int64_t &value, const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_i64"), position),
      value_(value) {}

A_ty AstInt64Literal::type() const { return A_ty::Int64Literal; }

std::string AstInt64Literal::toString() const {
  return fmt::format("[@AstInt64Literal {} value_:{}]", position_.toString(),
                     value_);
}

const int64_t &AstInt64Literal::value() const { return value_; }

AstUInt64Literal::AstUInt64Literal(const uint64_t &value,
                                   const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_u64"), position),
      value_(value) {}

A_ty AstUInt64Literal::type() const { return A_ty::UInt64Literal; }

std::string AstUInt64Literal::toString() const {
  return fmt::format("[@AstUInt64Literal {} value_:{}]", position_.toString(),
                     value_);
}

const uint64_t &AstUInt64Literal::value() const { return value_; }

AstFloat32Literal::AstFloat32Literal(const float &value,
                                     const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_f32"), position),
      value_(value) {}

A_ty AstFloat32Literal::type() const { return A_ty::Float32Literal; }

std::string AstFloat32Literal::toString() const {
  return fmt::format("[@AstFloat32Literal {} value_:{}]", position_.toString(),
                     value_);
}

const float &AstFloat32Literal::value() const { return value_; }

AstFloat64Literal::AstFloat64Literal(const double &value,
                                     const Position &position)
    : AstLiteral(nameGen.generateWith(value, "A_f64"), position),
      value_(value) {}

A_ty AstFloat64Literal::type() const { return A_ty::Float64Literal; }

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

A_ty AstStringLiteral::type() const { return A_ty::StringLiteral; }

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

A_ty AstBooleanLiteral::type() const { return A_ty::BooleanLiteral; }

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

A_ty AstCallExpression::type() const { return A_ty::CallExpression; }

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

A_ty AstUnaryExpression::type() const { return A_ty::UnaryExpression; }

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

A_ty AstBinaryExpression::type() const { return A_ty::BinaryExpression; }

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

A_ty AstConditionalExpression::type() const {
  return A_ty::ConditionalExpression;
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

A_ty AstAssignmentExpression::type() const {
  return A_ty::AssignmentExpression;
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

A_ty AstSequelExpression::type() const { return A_ty::SequelExpression; }

std::string AstSequelExpression::toString() const {
  return fmt::format("[@AstSequelExpression {} expressionList_:{}]",
                     position_.toString(), expressionList_->toString());
}

AstExpressionList *AstSequelExpression::expressionList() const {
  return expressionList_;
}

AstVoidExpression::AstVoidExpression()
    : AstExpression(nameGen.generate("A_VoidExp")) {}

A_ty AstVoidExpression::type() const { return A_ty::VoidExpression; }

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

A_ty AstExpressionStatement::type() const { return A_ty::ExpressionStatement; }

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

A_ty AstCompoundStatement::type() const { return A_ty::CompoundStatement; }

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

A_ty AstIfStatement::type() const { return A_ty::IfStatement; }

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

A_ty AstWhileStatement::type() const { return A_ty::WhileStatement; }

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

A_ty AstForStatement::type() const { return A_ty::ForStatement; }

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

A_ty AstContinueStatement::type() const { return A_ty::ContinueStatement; }

std::string AstContinueStatement::toString() const {
  return fmt::format("[@AstContinueStatement {}]", position_.toString());
}

AstBreakStatement::AstBreakStatement(const Position &breakTokenPosition,
                                     const Position &semiTokenPosition)
    : AstStatement(nameGen.generate("A_break"), breakTokenPosition) {
  position_.updatePosition(semiTokenPosition);
}

A_ty AstBreakStatement::type() const { return A_ty::BreakStatement; }

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

A_ty AstReturnStatement::type() const { return A_ty::ReturnStatement; }

std::string AstReturnStatement::toString() const {
  return fmt::format("[@AstReturnStatement {} expression_:{}]",
                     position_.toString(), expression_->toString());
}

AstExpression *AstReturnStatement::expression() const { return expression_; }

AstEmptyStatement::AstEmptyStatement()
    : AstStatement(nameGen.generate("A_EmpStm")) {}

AstEmptyStatement::AstEmptyStatement(const Position &semiTokenPosition)
    : AstStatement(nameGen.generate("A_EmpStm"), semiTokenPosition) {}

A_ty AstEmptyStatement::type() const { return A_ty::EmptyStatement; }

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

A_ty AstVariableDefinition::type() const { return A_ty::VariableDefinition; }

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

A_ty AstVariableInitialDefinition::type() const {
  return A_ty::VariableInitialDefinition;
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

A_ty AstFunctionDefinition::type() const { return A_ty::FunctionDefinition; }

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

A_ty AstFunctionSignatureDefinition::type() const {
  return A_ty::FunctionSignatureDefinition;
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

A_ty AstFunctionArgumentDefinition::type() const {
  return A_ty::FunctionArgumentDefinition;
}

std::string AstFunctionArgumentDefinition::toString() const {
  return fmt::format("[@AstFunctionArgumentDefinition {} identifier_:{}]",
                     position_.toString(), identifier_);
}

const std::string &AstFunctionArgumentDefinition::identifier() const {
  return identifier_;
}
