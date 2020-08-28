// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Strings.h"
#include "TokenName.h"
#include "parser.tab.hh"
#include <algorithm>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

// Ast {

Ast::Ast(const std::string &name, const yy::location &location)
    : Namely(name), Locationly(location) {}

bool Ast::isLiteral(std::shared_ptr<Ast> e) {
  if (!e)
    return false;
  switch (e->category()) {
  case AstCategory::Integer:
  case AstCategory::Float:
  case AstCategory::Boolean:
  case AstCategory::Character:
  case AstCategory::String:
  case AstCategory::Nil:
  case AstCategory::Void:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isId(std::shared_ptr<Ast> e) {
  return e && e->category() == (+AstCategory::VarId);
}

bool Ast::isExpr(std::shared_ptr<Ast> e) {
  if (!e)
    return false;
  switch (e->category()) {
  case AstCategory::If:
  case AstCategory::Loop:
  case AstCategory::LoopCondition:
  case AstCategory::Block:
  case AstCategory::BlockStats:
  case AstCategory::Throw:
  case AstCategory::Return:
  case AstCategory::Break:
  case AstCategory::Continue:
  case AstCategory::Assign:
  case AstCategory::PostfixExpr:
  case AstCategory::PrefixExpr:
  case AstCategory::InfixExpr:
  case AstCategory::Call:
  case AstCategory::Exprs:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isDef(std::shared_ptr<Ast> e) {
  if (!e)
    return false;
  switch (e->category()) {
  case AstCategory::FuncDef:
  case AstCategory::FuncSign:
  case AstCategory::Params:
  case AstCategory::Param:
  case AstCategory::VarDef:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isDecl(std::shared_ptr<Ast> e) {
  if (!e)
    return false;
  switch (e->category()) {
  case AstCategory::FuncSign:
  case AstCategory::Params:
  case AstCategory::Param:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isType(std::shared_ptr<Ast> e) {
  return e && e->category() == (+AstCategory::PlainType);
}

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

A_Integer::A_Integer(const std::string &literal, const yy::location &location)
    : Ast(literal, location) {
  LOG_ASSERT(literal.length() > 0, "literal.length {} > 0", literal.length());

  int startPosition = 0;
  if (Strings::startWith(literal, std::vector<std::string>{"0x", "0X", "0o",
                                                           "0O", "0b", "0B"})) {
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
  if (Strings::endWith(literal, std::vector<std::string>{"ul", "UL"})) {
    bitCategory_ = A_Integer::BitCategory::ULONG;
    bits_ = 64;
    endPosition = (int)literal.length() - 2;
  } else if (Strings::endWith(literal, std::vector<char>{'l', 'L'})) {
    bitCategory_ = A_Integer::BitCategory::LONG;
    bits_ = 64;
    endPosition = (int)literal.length() - 1;
  } else if (Strings::endWith(literal, std::vector<char>{'u', 'U'})) {
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
  return fmt::format("[{} location:{}, parsed:{}, "
                     "decimalCategory:{}, bitCategory:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(), parsed_,
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

A_Float::A_Float(const std::string &literal, const yy::location &location)
    : Ast(literal, location) {
  LOG_ASSERT(literal.length() > 0, "literal.length {} > 0", literal.length());

  int startPosition = 0;
  int endPosition = (int)literal.length();
  if (Strings::endWith(literal, std::vector<std::string>{"d", "D"})) {
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
  return fmt::format("[{} location:{}, parsed:{}, bitCategory:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(), parsed_,
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

A_String::A_String(const std::string &literal, const yy::location &location)
    : Ast(literal, location),
      parsed_(literal.length() >= 3 &&
                      Strings::startWith(literal,
                                         std::vector<std::string>{"\"\"\""})
                  ? literal.substr(3, literal.length() - 6)
                  : literal.substr(1, literal.length() - 2)),
      quoteCategory_(literal.length() >= 3 &&
                             Strings::startWith(
                                 literal, std::vector<std::string>{"\"\"\""})
                         ? A_String::QuoteCategory::TRIPLE
                         : A_String::QuoteCategory::SINGLE) {}

AstCategory A_String::category() const { return AstCategory::String; }

std::string A_String::toString() const {
  return fmt::format("[{} location:{}, parsed:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str(), parsed_);
}

A_String::QuoteCategory A_String::quoteCategory() const {
  return quoteCategory_;
}

const std::string &A_String::asString() const { return parsed_; }

// A_String }

// A_Character {

A_Character::A_Character(const std::string &literal,
                         const yy::location &location)
    : Ast(literal, location), parsed_(literal[1]) {}

AstCategory A_Character::category() const { return AstCategory::Character; }

std::string A_Character::toString() const {
  return fmt::format("[{} location:{}, parsed:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str(), parsed_);
}

char A_Character::asChar() const { return parsed_; }

// A_Character }

// A_Boolean {

A_Boolean::A_Boolean(bool a_value, const yy::location &location)
    : Ast(a_value ? "true" : "false", location), value(a_value) {}

AstCategory A_Boolean::category() const { return AstCategory::Boolean; }

std::string A_Boolean::toString() const {
  return fmt::format("[{} location:{}, value:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str(), value);
}

// A_Boolean }

// A_Nil {

A_Nil::A_Nil(const yy::location &location) : Ast("nil", location) {}

AstCategory A_Nil::category() const { return AstCategory::Nil; }

std::string A_Nil::toString() const {
  return fmt::format("[{} location:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str());
}

// A_Nil }

// A_Void {

A_Void::A_Void(const yy::location &location) : Ast("void", location) {}

AstCategory A_Void::category() const { return AstCategory::Void; }

std::string A_Void::toString() const {
  return fmt::format("[{} location:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str());
}

// A_Void }

// literal }

// id {

// A_VarId {

A_VarId::A_VarId(const std::string &literal, const yy::location &location)
    : Ast(literal, location) {}

AstCategory A_VarId::category() const { return AstCategory::VarId; }

std::string A_VarId::toString() const {
  return fmt::format("[{} location:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str());
}

// A_VarId }

// id }

// expr {

// A_Throw {

A_Throw::A_Throw(std::shared_ptr<Ast> a_expr, const yy::location &location)
    : Ast("throw", location), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
}

AstCategory A_Throw::category() const { return AstCategory::Throw; }

std::string A_Throw::toString() const {
  return fmt::format("[{} location:{} expr:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     expr->toString());
}

// A_Throw }

// A_Return {

A_Return::A_Return(std::shared_ptr<Ast> a_expr, const yy::location &location)
    : Ast("return", location), expr(a_expr) {}

AstCategory A_Return::category() const { return AstCategory::Return; }

std::string A_Return::toString() const {
  return fmt::format("[{} location:{} expr:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     expr ? expr->toString() : "nil");
}

// A_Return }

// A_Break {

A_Break::A_Break(const yy::location &location) : Ast("break", location) {}

AstCategory A_Break::category() const { return AstCategory::Break; }

std::string A_Break::toString() const {
  return fmt::format("[{} location:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str());
}

// A_Break }

// A_Continue {

A_Continue::A_Continue(const yy::location &location)
    : Ast("continue", location) {}

AstCategory A_Continue::category() const { return AstCategory::Continue; }

std::string A_Continue::toString() const {
  return fmt::format("[{} location:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str());
}

// A_Continue }

// A_Assign {

A_Assign::A_Assign(std::shared_ptr<Ast> a_assignee, int a_assignOp,
                   std::shared_ptr<Ast> a_assignor,
                   const yy::location &location)
    : Ast(tokenName(a_assignOp), location), assignee(a_assignee),
      assignOp(a_assignOp), assignor(a_assignor) {
  LOG_ASSERT(assignee, "assignee must not null");
  LOG_ASSERT(assignor, "assignor must not null");
}

AstCategory A_Assign::category() const { return AstCategory::Assign; }

std::string A_Assign::toString() const {
  return fmt::format(
      "[{} location:{} assignee:{} assignOp:{} assignor:{}]",
      name().toSymbolName(), (std::stringstream() << location()).str(),
      assignee->toString(), tokenName(assignOp), assignor->toString());
}

// A_Assign }

// A_PostfixExpr {

A_PostfixExpr::A_PostfixExpr(std::shared_ptr<Ast> a_expr, int a_postfixOp,
                             const yy::location &location)
    : Ast("postfixExpr", location), expr(a_expr), postfixOp(a_postfixOp) {
  LOG_ASSERT(expr, "expr must not null");
}

AstCategory A_PostfixExpr::category() const { return AstCategory::PostfixExpr; }

std::string A_PostfixExpr::toString() const {
  return fmt::format("[{} location:{} expr:{} postfixOp:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     expr->toString(), tokenName(postfixOp));
}

// A_PostfixExpr }

// A_InfixExpr {

A_InfixExpr::A_InfixExpr(std::shared_ptr<Ast> a_left, int a_infixOp,
                         std::shared_ptr<Ast> a_right,
                         const yy::location &location)
    : Ast("infixExpr", location), left(a_left), infixOp(a_infixOp),
      right(a_right) {
  LOG_ASSERT(left, "left must not null");
  LOG_ASSERT(right, "right must not null");
}

AstCategory A_InfixExpr::category() const { return AstCategory::InfixExpr; }

std::string A_InfixExpr::toString() const {
  return fmt::format("[{} location:{} left:{} infixOp:{} right:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     left->toString(), tokenName(infixOp), right->toString());
}

// A_InfixExpr }

// A_PrefixExpr {

A_PrefixExpr::A_PrefixExpr(int a_prefixOp, std::shared_ptr<Ast> a_expr,
                           const yy::location &location)
    : Ast("prefixExpr", location), prefixOp(a_prefixOp), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
}

AstCategory A_PrefixExpr::category() const { return AstCategory::PrefixExpr; }

std::string A_PrefixExpr::toString() const {
  return fmt::format("[{} location:{} prefixOp:{} expr:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     tokenName(prefixOp), expr->toString());
}

// A_PrefixExpr }

// A_Call {

A_Call::A_Call(std::shared_ptr<Ast> a_id, std::shared_ptr<A_Exprs> a_args,
               const yy::location &location)
    : Ast("call", location), id(a_id), args(a_args) {
  LOG_ASSERT(id, "id must not null");
}

AstCategory A_Call::category() const { return AstCategory::Call; }

std::string A_Call::toString() const {
  return fmt::format("[{} location:{} id:{} args:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str(), id->toString(),
                     args ? args->toString() : "nil");
}

// A_Call }

// A_Exprs {

A_Exprs::A_Exprs(std::shared_ptr<Ast> a_expr, std::shared_ptr<A_Exprs> a_next,
                 const yy::location &location)
    : Ast("exprs", location), expr(a_expr), next(a_next) {
  LOG_ASSERT(expr, "id must not null");
}

AstCategory A_Exprs::category() const { return AstCategory::Exprs; }

std::string A_Exprs::toString() const {
  return fmt::format("[{} location:{} expr:{} next:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     expr->toString(), next ? next->toString() : "nil");
}

// A_Exprs }

// A_If {

A_If::A_If(std::shared_ptr<Ast> a_condition, std::shared_ptr<Ast> a_thenp,
           std::shared_ptr<Ast> a_elsep, const yy::location &location)
    : Ast("if", location), condition(a_condition), thenp(a_thenp),
      elsep(a_elsep) {
  LOG_ASSERT(condition, "condition must not null");
  LOG_ASSERT(thenp, "thenp must not null");
}

AstCategory A_If::category() const { return AstCategory::If; }

std::string A_If::toString() const {
  return fmt::format(
      "[{} location:{} condition:{} thenp:{} elsep:{}]", name().toSymbolName(),
      (std::stringstream() << location()).str(), condition->toString(),
      thenp->toString(), elsep ? elsep->toString() : "nil");
}

// A_If }

// A_Loop {

A_Loop::A_Loop(std::shared_ptr<Ast> a_condition, std::shared_ptr<Ast> a_body,
               const yy::location &location)
    : Ast("loop", location), condition(a_condition), body(a_body) {
  LOG_ASSERT(condition, "condition must not null");
  LOG_ASSERT(body, "body must not null");
}

AstCategory A_Loop::category() const { return AstCategory::Loop; }

std::string A_Loop::toString() const {
  return fmt::format("[{} location:{} condition:{} body:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     condition->toString(), body->toString());
}

// A_Loop }

// A_LoopCondition {

A_LoopCondition::A_LoopCondition(std::shared_ptr<Ast> a_init,
                                 std::shared_ptr<Ast> a_condition,
                                 std::shared_ptr<Ast> a_update, bool a_do_once,
                                 const yy::location &location)
    : Ast("loopCondition", location), init(a_init), condition(a_condition),
      update(a_update), do_once(a_do_once) {}

AstCategory A_LoopCondition::category() const {
  return AstCategory::LoopCondition;
}

std::string A_LoopCondition::toString() const {
  return fmt::format(
      "[{} location:{} init:{} condition:{} update:{} do_once:{}]",
      name().toSymbolName(), (std::stringstream() << location()).str(),
      init ? init->toString() : "nil",
      condition ? condition->toString() : "nil",
      update ? update->toString() : "nil", do_once);
}

// A_LoopCondition }

// A_LoopEnumerator {

A_LoopEnumerator::A_LoopEnumerator(std::shared_ptr<Ast> a_id,
                                   std::shared_ptr<Ast> a_expr,
                                   const yy::location &location)
    : Ast("loopEnumerator", location), id(a_id), expr(a_expr) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(expr, "expr must not null");
}

AstCategory A_LoopEnumerator::category() const {
  return AstCategory::LoopEnumerator;
}

std::string A_LoopEnumerator::toString() const {
  return fmt::format("[{} location:{} id:{} expr:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str(), id->toString(),
                     expr->toString());
}

// A_LoopEnumerator }

// A_Try {

A_Try::A_Try(std::shared_ptr<Ast> a_tryp, std::shared_ptr<Ast> a_catchp,
             std::shared_ptr<Ast> a_finallyp, const yy::location &location)
    : Ast("try", location), tryp(a_tryp), catchp(a_catchp),
      finallyp(a_finallyp) {
  LOG_ASSERT(tryp, "tryp must not null");
  LOG_ASSERT(catchp, "catchp must not null");
}

AstCategory A_Try::category() const { return AstCategory::Try; }

std::string A_Try::toString() const {
  return fmt::format(
      "[{} location:{} init:{} condition:{} update:{}]", name().toSymbolName(),
      (std::stringstream() << location()).str(), tryp->toString(),
      catchp->toString(), finallyp ? finallyp->toString() : "nil");
}

// A_Try }

// A_Block {

A_Block::A_Block(std::shared_ptr<Ast> a_blockStat,
                 std::shared_ptr<A_BlockStats> a_blockStats,
                 const yy::location &location)
    : Ast("block", location), blockStat(a_blockStat), blockStats(a_blockStats) {
}

AstCategory A_Block::category() const { return AstCategory::Block; }

std::string A_Block::toString() const {
  return fmt::format("[{} location:{} blockStat:{} blockStats:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     blockStat ? blockStat->toString() : "nil",
                     blockStats ? blockStats->toString() : "nil");
}

// A_Block }

// A_BlockStats {

A_BlockStats::A_BlockStats(std::shared_ptr<Ast> a_blockStat,
                           std::shared_ptr<A_BlockStats> a_next,
                           const yy::location &location)
    : Ast("block", location), blockStat(a_blockStat), next(a_next) {}

AstCategory A_BlockStats::category() const { return AstCategory::BlockStats; }

std::string A_BlockStats::toString() const {
  return fmt::format("[{} location:{} blockStat:{} next:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     blockStat ? blockStat->toString() : "nil",
                     next ? next->toString() : "nil");
}

// A_BlockStats }

// expr }

// type {

// A_PlainType {

A_PlainType::A_PlainType(int a_token, const yy::location &location)
    : Ast(tokenName(a_token), location), token(a_token) {}

AstCategory A_PlainType::category() const { return AstCategory::PlainType; }

std::string A_PlainType::toString() const {
  return fmt::format("[@{} location:{}, token:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     tokenName(token));
}

// A_PlainType }

// type }

// definition and declaration {

A_FuncDef::A_FuncDef(std::shared_ptr<Ast> a_funcSign,
                     std::shared_ptr<Ast> a_resultType,
                     std::shared_ptr<Ast> a_body, const yy::location &location)
    : Ast("funcDef", location), funcSign(a_funcSign), resultType(a_resultType),
      body(a_body) {
  LOG_ASSERT(funcSign, "funcSign must not null");
  LOG_ASSERT(resultType, "resultType must not null");
  LOG_ASSERT(body, "body must not null");
}

AstCategory A_FuncDef::category() const { return AstCategory::FuncDef; }

std::string A_FuncDef::toString() const {
  return fmt::format(
      "[@{} location:{}, funcSign:{} resultType:{} body:{}]",
      name().toSymbolName(), (std::stringstream() << location()).str(),
      funcSign->toString(), resultType->toString(), body->toString());
}

A_FuncSign::A_FuncSign(std::shared_ptr<Ast> a_id,
                       std::shared_ptr<A_Params> a_params,
                       const yy::location &location)
    : Ast("funcSign", location), id(a_id), params(a_params) {
  LOG_ASSERT(id, "id must not null");
}

AstCategory A_FuncSign::category() const { return AstCategory::FuncSign; }

std::string A_FuncSign::toString() const {
  return fmt::format("[@{} location:{}, id:{} params:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(), id->toString(),
                     params ? params->toString() : "nil");
}

A_Params::A_Params(std::shared_ptr<A_Param> a_param,
                   std::shared_ptr<A_Params> a_next,
                   const yy::location &location)
    : Ast("params", location), param(a_param), next(a_next) {
  LOG_ASSERT(param, "param must not null");
}

AstCategory A_Params::category() const { return AstCategory::Params; }

std::string A_Params::toString() const {
  return fmt::format("[@{} location:{}, param:{} next:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     param->toString(), next ? next->toString() : "nil");
}

A_Param::A_Param(std::shared_ptr<Ast> a_id, std::shared_ptr<Ast> a_type,
                 const yy::location &location)
    : Ast("param", location), id(a_id), type(a_type) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
}

AstCategory A_Param::category() const { return AstCategory::Param; }

std::string A_Param::toString() const {
  return fmt::format("[@{} location:{}, id:{} type:{}]", name().toSymbolName(),
                     (std::stringstream() << location()).str(), id->toString(),
                     type->toString());
}

A_VarDef::A_VarDef(std::shared_ptr<Ast> a_id, std::shared_ptr<Ast> a_type,
                   std::shared_ptr<Ast> a_expr, const yy::location &location)
    : Ast("varDef", location), id(a_id), type(a_type), expr(a_expr) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
  LOG_ASSERT(expr, "expr must not null");
}

AstCategory A_VarDef::category() const { return AstCategory::VarDef; }

std::string A_VarDef::toString() const {
  return fmt::format("[@{} location:{}, id:{} type:{} expr:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(), id->toString(),
                     type->toString(), expr->toString());
}

// definition and declaration }

// compile unit {

A_TopStats::A_TopStats(std::shared_ptr<Ast> a_topStat,
                       std::shared_ptr<A_TopStats> a_next,
                       const yy::location &location)
    : Ast("topStats", location), topStat(a_topStat), next(a_next) {}

AstCategory A_TopStats::category() const { return AstCategory::TopStats; }

std::string A_TopStats::toString() const {
  return fmt::format(
      "[@{} location:{}, topStat:{} next:{}]", name().toSymbolName(),
      (std::stringstream() << location()).str(),
      topStat ? topStat->toString() : "nil", next ? next->toString() : "nil");
}

A_CompileUnit::A_CompileUnit(std::shared_ptr<Ast> a_topStat,
                             std::shared_ptr<A_TopStats> a_topStats,
                             const yy::location &location)
    : Ast("compileUnit", location), topStat(a_topStat), topStats(a_topStats) {}

AstCategory A_CompileUnit::category() const { return AstCategory::CompileUnit; }

std::string A_CompileUnit::toString() const {
  return fmt::format("[@{} location:{}, topStat:{} topStats:{}]",
                     name().toSymbolName(),
                     (std::stringstream() << location()).str(),
                     topStat ? topStat->toString() : "nil",
                     topStats ? topStats->toString() : "nil");
}

// compile unit }
