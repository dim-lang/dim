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

bool Ast::isLiteral(const Ast *e) { return true; }

bool Ast::isId(const Ast *e) { return true; }

bool Ast::isExpr(const Ast *e) { return true; }

bool Ast::isExprWithBlock(const Ast *e) { return true; }

bool Ast::isExprWithoutBlock(const Ast *e) { return true; }

bool Ast::isDef(const Ast *e) { return true; }

bool Ast::isDecl(const Ast *e) { return true; }

bool Ast::isType(const Ast *e) { return true; }

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
    : Ast(literal, position) {
  EX_ASSERT(literal.length() > 0, "literal.length {} > 0", literal.length());

  int startPosition = 0;
  if (stringStartsWith(literal, std::vector<std::string>{"0x", "0X", "0o", "0O",
                                                         "0b", "0B"})) {
    switch (literal[1]) {
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

  int endPosition = (int)literal.length();
  if (stringEndsWith(literal, std::vector<std::string>{"ul", "UL"})) {
    bitCategory_ = A_Integer::BitCategory::ULONG;
    bits_ = 64;
    endPosition = (int)literal.length() - 2;
  } else if (stringEndsWith(literal, std::vector<char>{'l', 'L'})) {
    bitCategory_ = A_Integer::BitCategory::LONG;
    bits_ = 64;
    endPosition = (int)literal.length() - 1;
  } else if (stringEndsWith(literal, std::vector<char>{'u', 'U'})) {
    bitCategory_ = A_Integer::BitCategory::UNSIGNED;
    bits_ = 32;
    endPosition = (int)literal.length() - 1;
  } else {
    bitCategory_ = A_Integer::BitCategory::SIGNED;
    bits_ = 32;
    endPosition = (int)literal.length();
  }

  parsed_ = literal.substr(startPosition, endPosition - startPosition);
}

AstCategory A_Integer::category() const { return AstCategory::Integer; }

std::string A_Integer::toString() const {
  return fmt::format("[{} position:{}, parsed:{}, "
                     "decimalCategory:{}, bitCategory:{}]",
                     name().toSymbolName(), position().toString(), parsed_,
                     AIL_DC_Map.find(decimalCategory_)->second,
                     AIL_BC_Map.find(bitCategory_)->second);
}

int A_Integer::bits() const { return bits_; }

int A_Integer::base() const { return base_; }

A_Integer::DecimalCategory A_Integer::decimalCategory() const {
  return decimalCategory_;
}

A_Integer::BitCategory A_Integer::bitCategory() const { return bitCategory_; }

int32_t A_Integer::asInt32() const {
  return static_cast<int32_t>(std::stol(parsed_, nullptr, base_));
}

uint32_t A_Integer::asUInt32() const {
  return static_cast<uint32_t>(std::stoul(parsed_, nullptr, base_));
}

int64_t A_Integer::asInt64() const {
  return static_cast<int64_t>(std::stoll(parsed_, nullptr, base_));
}

uint64_t A_Integer::asUInt64() const {
  return static_cast<uint64_t>(std::stoull(parsed_, nullptr, base_));
}

// A_Integer }

// A_Float {

const static std::unordered_map<A_Float::BitCategory, std::string> AFL_BC_Map =
    {
        {A_Float::BitCategory::FLT, "FLT"},
        {A_Float::BitCategory::DBL, "DBL"},
};

A_Float::A_Float(const std::string &literal, const Position &position)
    : Ast(literal, position) {
  EX_ASSERT(literal.length() > 0, "literal.length {} > 0", literal.length());

  int startPosition = 0;
  int endPosition = (int)literal.length();
  if (stringEndsWith(literal, std::vector<std::string>{"d", "D"})) {
    bitCategory_ = A_Float::BitCategory::DBL;
    bits_ = 64;
    endPosition = (int)literal.length();
  } else {
    bitCategory_ = A_Float::BitCategory::FLT;
    bits_ = 32;
    endPosition = (int)literal.length() - 1;
  }

  parsed_ = literal.substr(startPosition, endPosition - startPosition);
}

AstCategory A_Float::category() const { return AstCategory::Float; }

std::string A_Float::toString() const {
  return fmt::format("[{} position:{}, parsed:{}, bitCategory:{}]",
                     name().toSymbolName(), position().toString(), parsed_,
                     AFL_BC_Map.find(bitCategory_)->second);
}

int A_Float::bits() const { return bits_; }

A_Float::BitCategory A_Float::bitCategory() const { return bitCategory_; }

float A_Float::asFloat() const { return std::stof(parsed_); }

double A_Float::asDouble() const { return std::stod(parsed_); }

// A_Float }

// A_String {

const static std::unordered_map<A_String::QuoteCategory, std::string>
    ASL_QC_Map = {
        {A_String::QuoteCategory::SINGLE, "SINGLE"},
        {A_String::QuoteCategory::TRIPLE, "TRIPLE"},
};

A_String::A_String(const std::string &literal, const Position &position)
    : Ast(literal, position),
      parsed_(
          literal.length() >= 3 &&
                  stringStartsWith(literal, std::vector<std::string>{"\"\"\""})
              ? literal.substr(3, literal.length() - 6)
              : literal.substr(1, literal.length() - 2)),
      quoteCategory_(
          literal.length() >= 3 &&
                  stringStartsWith(literal, std::vector<std::string>{"\"\"\""})
              ? A_String::QuoteCategory::TRIPLE
              : A_String::QuoteCategory::SINGLE) {}

AstCategory A_String::category() const { return AstCategory::String; }

std::string A_String::toString() const {
  return fmt::format("[{} position:{}, parsed:{}]", name().toSymbolName(),
                     position().toString(), parsed_);
}

A_String::QuoteCategory A_String::quoteCategory() const {
  return quoteCategory_;
}

const std::string &A_String::asString() const { return parsed_; }

// A_String }

// A_Character {

A_Character::A_Character(const std::string &literal, const Position &position)
    : Ast(literal, position), parsed_(literal[1]) {}

AstCategory A_Character::category() const { return AstCategory::Character; }

std::string A_Character::toString() const {
  return fmt::format("[{} position:{}, parsed:{}]", name().toSymbolName(),
                     position().toString(), parsed_);
}

char A_Character::asChar() const { return parsed_; }

// A_Character }

// A_Boolean {

A_Boolean::A_Boolean(const std::string &literal, const Position &position)
    : Ast(literal, position), parsed_(literal == tokenName(T_TRUE)) {}

AstCategory A_Boolean::category() const { return AstCategory::Boolean; }

std::string A_Boolean::toString() const {
  return fmt::format("[{} position:{}, parsed:{}]", name().toSymbolName(),
                     position().toString(), parsed_);
}

bool A_Boolean::asBool() const { return parsed_; }

// A_Boolean }

// A_Nil {

A_Nil::A_Nil(const Position &position) : Ast(tokenName(T_NIL), position) {}

AstCategory A_Nil::category() const { return AstCategory::Nil; }

std::string A_Nil::toString() const {
  return fmt::format("[{} position:{}]", name().toSymbolName(),
                     position().toString());
}

// A_Nil }

// A_Void {

A_Void::A_Void(const Position &position) : Ast(tokenName(T_VOID), position) {}

AstCategory A_Void::category() const { return AstCategory::Void; }

std::string A_Void::toString() const {
  return fmt::format("[{} position:{}]", name().toSymbolName(),
                     position().toString());
}

// A_Void }

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

// expr {

Ast::Ast(const std::string &name) : Ast(name) {}

Ast::Ast(const std::string &name, const Position &position)
    : Ast(name, position) {}

// A_Throw {

A_Throw::A_Throw(const Ast *expr, const Position &position)
    : Ast(tokenName(T_THROW), extra), expr_(expr) {
  EX_ASSERT(expr_, "expr_ must not null");
  locate(expr_->position());
}

A_Throw::~A_Throw() {
  delete expr_;
  expr_ = nullptr;
}

AstCategory A_Throw::category() const { return AstCategory::Throw; }

std::string A_Throw::toString() const {
  return fmt::format("[{} position:{} expr:{}]", name().toSymbolName(),
                     position().toString(), expr_->toString());
}

const Ast *A_Throw::expr() const { return expr_; }

// A_Throw }

// A_Return {

A_Return::A_Return(const Ast *expr, const Position &position)
    : Ast(tokenName(T_RETURN), position), expr_(expr) {
  if (expr_) {
    locate(expr_->position());
  }
}

A_Return::~A_Return() {
  if (expr_) {
    delete expr_;
    expr_ = nullptr;
  }
}

AstCategory A_Return::category() const { return AstCategory::Return; }

std::string A_Return::toString() const {
  return fmt::format("[{} position:{} expr:{}]", name().toSymbolName(),
                     position().toString(), expr_ ? expr_->toString() : "nil");
}

const Ast *A_Return::expr() const { return expr_; }

// A_Return }

// A_Break {

A_Break::A_Break(const Position &position)
    : Ast(tokenName(T_BREAK), position) {}

AstCategory A_Break::category() const { return AstCategory::Break; }

std::string A_Break::toString() const {
  return fmt::format("[{} position:{}]", name().toSymbolName(),
                     position().toString());
}

// A_Break }

// A_Continue {

A_Continue::A_Continue(const Position &position)
    : Ast(tokenName(T_CONTINUE), position) {}

AstCategory A_Continue::category() const { return AstCategory::Continue; }

std::string toString() const {
  return fmt::format("[{} position:{}]", name().toSymbolName(),
                     position().toString());
}

// A_Continue }

// A_Assign {

A_Assign::A_Assign(const Ast *assignee, int assignOp, const Ast *assignor,
                   const Position &position)
    : Ast(tokenName(assignOp), position), assignee_(assignee),
      assignOp_(assignOp), assignor_(assignor) {
  EX_ASSERT(assignee_, "assignee_ must not null");
  EX_ASSERT(tokenExist(assignOp), "assignOp_ {} must exist");
  EX_ASSERT(assignor_, "assignor_ must not null");
  locate(assignee_->position());
  locate(assignor_->position());
}

A_Assign::~A_Assign() {
  delete assignee_;
  assignee_ = nullptr;
  delete assignor_;
  assignor_ = nullptr;
}

AstCategory A_Assign::category() const { return AstCategory::Assign; }

std::string A_Assign::toString() const {
  return fmt::format("[{} position:{} assignee:{} assignOp:{} assignor:{}]",
                     name().toSymbolName(), position().toString(),
                     assignee_->toString(), tokenName(assignOp_),
                     assignor_->toString());
}

const Ast *A_Assign::assignee() const { return assignee_; }

int A_Assign::assignOp() const { return assignOp_; }

const Ast *A_Assign::assignor() const { return assignor_; }

// A_Assign }

// A_PostfixExpr {

A_PostfixExpr::A_PostfixExpr(const Ast *expr, int postfixOp,
                             const Position &position)
    : Ast("PostfixExpr", position), expr_(expr), postfixOp_(postfixOp) {
  EX_ASSERT(expr_, "expr_ must not null");
  EX_ASSERT(tokenExist(postfixOp_), "postfixOp_ {} must exist",
            tokenName(postfixOp_));
}

A_PostfixExpr::~A_PostfixExpr() {
  delete expr_;
  expr_ = nullptr;
}

AstCategory A_PostfixExpr::category() const { return AstCategory::PostfixExpr; }

std::string A_PostfixExpr::toString() const {
  return fmt::format("[{} position:{} expr:{} postfixOp:{}]",
                     name().toSymbolName(), position().toString(),
                     expr_->toString(), tokenName(postfixOp_));
}

const Ast *A_PostfixExpr::expr() const { return expr_; }

int A_PostfixExpr::postfixOp() const { return postfixOp_; }

// A_PostfixExpr }

// A_InfixExpr {

A_InfixExpr::A_InfixExpr(const Ast *left, int infixOp, const Ast *right,
                         const Position &position)
    : Ast("InfixExpr", position), left_(left), infixOp_(infixOp),
      right_(right) {
  EX_ASSERT(left_, "left_ must not null");
  EX_ASSERT(tokenExist(infixOp_), "infixOp_ {} must exist",
            tokenName(infixOp_));
  EX_ASSERT(right_, "right_ must not null");
  locate(left_->position());
  locate(right_->position());
}

A_InfixExpr::~A_InfixExpr() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

AstCategory A_InfixExpr::category() const { return AstCategory::InfixExpr; }

std::string A_InfixExpr::toString() const {
  return fmt::format("[{} position:{} left:{} infixOp:{} right:{}]",
                     name().toSymbolName(), position().toString(),
                     left_->toString(), tokenName(infixOp_),
                     right_->toString());
}

const Ast *A_InfixExpr::left() const { return left_; }

int A_InfixExpr::infixOp() const { return infixOp_; }

const Ast *A_InfixExpr::right() const { return right_; }

// A_InfixExpr }

// A_PrefixExpr {

A_PrefixExpr::A_PrefixExpr(int prefixOp, const Ast *expr,
                           const Position &position)
    : Ast("PrefixExpr", position), prefixOp_(prefixOp), expr_(expr) {
  EX_ASSERT(tokenExist(prefixOp_), "prefixOp_ {} must exist",
            tokenName(prefixOp_));
  EX_ASSERT(expr_, "expr_ must not null");
  locate(expr_->position());
}

A_PrefixExpr::~A_PrefixExpr() {
  delete expr_;
  expr_ = nullptr;
}

AstCategory A_PrefixExpr::category() const { return AstCategory::PrefixExpr; }

std::string A_PrefixExpr::toString() const {
  return fmt::format("[{} position:{} prefixOp:{} expr:{}]",
                     name().toSymbolName(), position().toString(),
                     tokenName(prefixOp_), expr_->toString());
}

int A_PrefixExpr::prefixOp() const { return prefixOp_; }

const Ast *A_PrefixExpr::expr() const { return expr_; }

// A_PrefixExpr }

// expr }

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

// expr {

// Ast {

Ast::Ast(const std::string &name)
    : AstNamely(name), AstPositional(Position()) {}

Ast::Ast(const std::string &name, const Position &position)
    : AstNamely(name), AstPositional(position) {}

// Ast }

A_CallExpr::A_CallExpr(const AstId *id, const A_ExprList *argumentList,
                       const Position &idPosition)
    : Ast(nameGen.generate("A_CallExpr"), idPosition), id_(id),
      argumentList_(argumentList) {
  EX_ASSERT(id_, "id_ must not null");
  EX_ASSERT(argumentList_, "argumentList_ must not null");
  locate(id_->position());
  locate(argumentList_->position());
}

A_CallExpr::~A_CallExpr() {
  delete id_;
  delete argumentList_;
  id_ = nullptr;
  argumentList_ = nullptr;
}

AstCategory A_CallExpr::category() const { return AstCategory::CallExpr; }

std::string A_CallExpr::toString() const {
  return fmt::format("[@{} position:{}, id:{}, argumentList:{}]", name(),
                     position().toString(), id_->toString(),
                     argumentList_->toString());
}

const AstId *A_CallExpr::id() const { return id_; }

const A_ExprList *A_CallExpr::argumentList() const { return argumentList_; }

// A_UnaryExpr {

A_UnaryExpr::A_UnaryExpr(const AstToken *token, const Ast *expr)
    : Ast(nameGen.generate("A_UnaryExpr")), token_(token), expr_(expr) {
  EX_ASSERT(token_, "token_ must not null");
  EX_ASSERT(expr_, "expr_ must not null");
  locate(token_->position());
  locate(expr_->position());
}

A_UnaryExpr::~A_UnaryExpr() {
  delete token_;
  token_ = nullptr;
  delete expr_;
  expr_ = nullptr;
}

AstCategory A_UnaryExpr::category() const { return AstCategory::UnaryExpr; }

std::string A_UnaryExpr::toString() const {
  return fmt::format("[@{} position:{}, token:{}, expr:{}]", name(),
                     position().toString(), token_->name(), expr_->toString());
}

const AstToken *A_UnaryExpr::token() const { return token_; }

const Ast *A_UnaryExpr::expr() const { return expr_; }

// A_UnaryExpr }

// A_BinaryExpr {

A_BinaryExpr::A_BinaryExpr(const Ast *left, const AstToken *token,
                           const Ast *right)
    : Ast(nameGen.generate("A_BinaryExpr")), left_(left), token_(token),
      right_(right) {
  EX_ASSERT(left_, "left_ must not null");
  EX_ASSERT(token_, "token_ must not null");
  EX_ASSERT(right_, "right_ must not null");
  locate(left_->position());
  locate(token_->position());
  locate(right_->position());
}

A_BinaryExpr::~A_BinaryExpr() {
  delete left_;
  left_ = nullptr;
  delete token_;
  token_ = nullptr;
  delete right_;
  right_ = nullptr;
}

AstCategory A_BinaryExpr::category() const { return AstCategory::BinaryExpr; }

std::string A_BinaryExpr::toString() const {
  return fmt::format("[@{} position:{}, token:{}, left:{}, right:{}]", name(),
                     position().toString(), token_->name(), left_->toString(),
                     right_->toString());
}

const Ast *A_BinaryExpr::left() const { return left_; }

const AstToken *A_BinaryExpr::token() const { return token_; }

const Ast *A_BinaryExpr::right() const { return right_; }

// A_BinaryExpr }

// A_ConditionalExpr {

A_ConditionalExpr::A_ConditionalExpr(const Ast *condition, const Ast *thens,
                                     const Ast *elses)
    : Ast(nameGen.generate("A_ConditionalExpr")), condition_(condition),
      thens_(thens), elses_(elses) {
  EX_ASSERT(condition_, "condition_ must not null");
  EX_ASSERT(thens_, "thens_ must not null");
  EX_ASSERT(elses_, "elses_ must not null");
}

A_ConditionalExpr::~A_ConditionalExpr() {
  delete condition_;
  condition_ = nullptr;
  delete thens_;
  thens_ = nullptr;
  delete elses_;
  elses_ = nullptr;
}

AstCategory A_ConditionalExpr::category() const {
  return AstCategory::ConditionalExpr;
}

std::string A_ConditionalExpr::toString() const {
  return fmt::format("[@{} position:{}, condition:{}, thens:{}, elses:{}]",
                     name(), position().toString(), condition_->toString(),
                     thens_->toString(), elses_->toString());
}

const Ast *A_ConditionalExpr::condition() const { return condition_; }

const Ast *A_ConditionalExpr::thens() const { return thens_; }

const Ast *A_ConditionalExpr::elses() const { return elses_; }

// A_ConditionalExpr }

// A_AssigmentExpr {

A_AssignmentExpr::A_AssignmentExpr(const Ast *left, int token, const Ast *right,
                                   const Position &tokenPosition)
    : Ast(nameGen.generate("A_AssignmentExpr"), tokenPosition), left_(left),
      token_(token), right_(right) {
  EX_ASSERT(left_, "left_ must not null");
  EX_ASSERT(right_, "right_ must not null");
  EX_ASSERT(tokenNameExist(token_), "invalid token:{}", token_);
  locate(left_->position());
  locate(right_->position());
}

A_AssignmentExpr::~A_AssignmentExpr() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

AstCategory A_AssignmentExpr::category() const {
  return AstCategory::AssignmentExpr;
}

std::string A_AssignmentExpr::toString() const {
  return fmt::format("[@{} position:{} left:{}, token:{}, right:{}]", name(),
                     position().toString(), left_->toString(),
                     tokenName(token_), right_->toString());
}

const Ast *A_AssignmentExpr::left() const { return left_; }

int A_AssignmentExpr::token() const { return token_; }

const Ast *A_AssignmentExpr::right() const { return right_; }

// A_AssigmentExpr }

// A_VoidExpr {

A_VoidExpr::A_VoidExpr() : Ast(nameGen.generate("A_VoidExpr")) {}

AstCategory A_VoidExpr::category() const { return AstCategory::VoidExpr; }

std::string A_VoidExpr::toString() const {
  return fmt::format("[@{}]", name());
}

// A_VoidExpr }

// expr }

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

AstStatement::AstStatement(Ast *expr, const Position &semiTokenPosition)
    : AstStatement(nameGen.generate("A_ExpStm"), semiTokenPosition),
      expr_(expr) {
  EX_ASSERT(expr_, "expr_ is null");
  position_.updatePosition(expr_->position());
}

AstStatement::~AstStatement() {
  delete expr_;
  expr_ = nullptr;
}

AstCategory AstStatement::category() const {
  return AstCategory::ExprStatement;
}

std::string AstStatement::toString() const {
  return fmt::format("[@AstStatement {} expr_:{}]", position_.toString(),
                     expr_->toString());
}

Ast *AstStatement::expr() const { return expr_; }

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

A_IfStatement::A_IfStatement(Ast *condition, AstStatement *thens,
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

Ast *A_IfStatement::condition() const { return condition_; }

AstStatement *A_IfStatement::thens() const { return thens_; }

AstStatement *A_IfStatement::elses() const { return elses_; }

AstWhileStatement::AstWhileStatement(Ast *condition, AstStatement *statement,
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

Ast *AstWhileStatement::condition() const { return condition_; }

AstStatement *AstWhileStatement::statement() const { return statement_; }

AstForStatement::AstForStatement(AstStatement *start, AstStatement *step,
                                 Ast *end, AstStatement *statement,
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

Ast *AstForStatement::end() const { return end_; }

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
                                             const char *typeId, Ast *expr,
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
    const char *identifier, AstIdentifier *typeId, Ast *expr,
    const Position &identifierPosition, const Position &typePosition)
    : AstDefinition(nameGen.generate("A_VarAssDecl"), identifierPosition),
      identifier_(identifier), typeId_(typeId), expr_(expr) {
  EX_ASSERT(identifier, "identifier must not null");
  EX_ASSERT(typeId_, "typeId_ must not null");
  EX_ASSERT(expr_, "expr_ must not null");
  position_.updatePosition(typePosition);
  position_.updatePosition(expr_->position());
}

AstVariableInitialDefinition::~AstVariableInitialDefinition() {
  delete expr_;
  expr_ = nullptr;
}

AstCategory AstVariableInitialDefinition::category() const {
  return AstCategory::VariableInitialDefinition;
}

std::string AstVariableInitialDefinition::toString() const {
  return fmt::format(
      "[@AstVariableInitialDefinition {} identifier_:{}, expr_:{}]",
      position_.toString(), identifier_, expr_->toString());
}

const std::string &AstVariableInitialDefinition::identifier() const {
  return identifier_;
}

AstIdentifier *AstVariableInitialDefinition::typeId() const { return typeId_; }

Ast *AstVariableInitialDefinition::expr() const { return expr_; }

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
    const char *identifier, AstDefinitionList *argumentList, Ast *result,
    const Position &funcTokenPosition, const Position &identifierPosition)
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

Ast *AstFunctionSignatureDefinition::result() const { return result_; }

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
