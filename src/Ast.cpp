// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Token.h"
#include "parser.tab.hh"
#include <algorithm>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

static Counter IdentifierCounter;

#define DEL(x)                                                                 \
  do {                                                                         \
    if (x) {                                                                   \
      delete x;                                                                \
      x = nullptr;                                                             \
    }                                                                          \
  } while (0)

namespace detail {

ParentableImpl::ParentableImpl(Ast *parentableImpl)
    : parentableImpl_(parentableImpl) {}

Ast *&ParentableImpl::parent() { return parentableImpl_; }

Ast *ParentableImpl::parent() const { return parentableImpl_; }

} // namespace detail

// Ast {

Ast::Ast(const Cowstr &name, const Location &location)
    : NameableImpl(name), LocationableImpl(location),
      CountableImpl(IdentifierCounter.next()) {}

bool Ast::isLiteral(Ast *e) {
  if (!e)
    return false;
  switch (e->kind()) {
  case AstKind::Integer:
  case AstKind::Float:
  case AstKind::Boolean:
  case AstKind::Character:
  case AstKind::String:
  case AstKind::Nil:
  case AstKind::Void:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isId(Ast *e) { return e && e->kind() == (+AstKind::VarId); }

bool Ast::isExpr(Ast *e) {
  if (!e)
    return false;
  switch (e->kind()) {
  case AstKind::If:
  case AstKind::Loop:
  case AstKind::LoopCondition:
  case AstKind::Block:
  case AstKind::BlockStats:
  case AstKind::Throw:
  case AstKind::Return:
  case AstKind::Break:
  case AstKind::Continue:
  case AstKind::Assign:
  case AstKind::PostfixExpr:
  case AstKind::PrefixExpr:
  case AstKind::InfixExpr:
  case AstKind::Call:
  case AstKind::Exprs:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isDef(Ast *e) {
  if (!e)
    return false;
  switch (e->kind()) {
  case AstKind::FuncDef:
  case AstKind::FuncSign:
  case AstKind::Params:
  case AstKind::Param:
  case AstKind::VarDef:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isDecl(Ast *e) {
  if (!e)
    return false;
  switch (e->kind()) {
  case AstKind::FuncSign:
  case AstKind::Params:
  case AstKind::Param:
    return true;
  default:
    return false;
  }
  return false;
}

bool Ast::isType(Ast *e) { return e && e->kind() == (+AstKind::PlainType); }

// Ast }

// literal {

// A_Integer {

static const std::unordered_map<A_Integer::DecimalKind, Cowstr> AIL_DC_Map = {
    {A_Integer::DecimalKind::DEC, "DEC"},
    {A_Integer::DecimalKind::HEX, "HEX"},
    {A_Integer::DecimalKind::BIN, "BIN"},
    {A_Integer::DecimalKind::OCT, "OCT"},
};

static const std::unordered_map<A_Integer::BitKind, Cowstr> AIL_BC_Map = {
    {A_Integer::BitKind::SIGNED, "SIGNED"},
    {A_Integer::BitKind::UNSIGNED, "UNSIGNED"},
    {A_Integer::BitKind::LONG, "LONG"},
    {A_Integer::BitKind::ULONG, "ULONG"},
};

A_Integer::A_Integer(const Cowstr &literal, const Location &location)
    : Ast(literal, location) {
  LOG_ASSERT(literal.length() > 0, "literal.length {} > 0", literal.length());

  int startPosition = 0;
  std::vector<Cowstr> decimalPrefix = {"0x", "0X", "0o", "0O", "0b", "0B"};
  if (literal.startWithAnyOf(decimalPrefix.begin(), decimalPrefix.end())) {
    switch (literal[1]) {
    case 'x':
    case 'X':
      decimalKind_ = A_Integer::DecimalKind::HEX;
      base_ = 16;
      startPosition = 2;
      break;
    case 'o':
    case 'O':
      decimalKind_ = A_Integer::DecimalKind::OCT;
      base_ = 8;
      startPosition = 2;
      break;
    case 'b':
    case 'B':
      decimalKind_ = A_Integer::DecimalKind::BIN;
      base_ = 2;
      startPosition = 2;
      break;
    default:
      break;
    }
  } else {
    decimalKind_ = A_Integer::DecimalKind::DEC;
    base_ = 10;
    startPosition = 0;
  }

  int endPosition = (int)literal.length();
  std::vector<Cowstr> bitPostfix = {"ul", "UL", "uL", "Ul"};
  std::vector<Cowstr> longPostfix = {"l", "L"};
  std::vector<Cowstr> unsignedPostfix = {"u", "U"};
  if (literal.endWithAnyOf(bitPostfix.begin(), bitPostfix.end())) {
    bitKind_ = A_Integer::BitKind::ULONG;
    bits_ = 64;
    endPosition = (int)literal.length() - 2;
  } else if (literal.endWithAnyOf(longPostfix.begin(), longPostfix.end())) {
    bitKind_ = A_Integer::BitKind::LONG;
    bits_ = 64;
    endPosition = (int)literal.length() - 1;
  } else if (literal.endWithAnyOf(unsignedPostfix.begin(),
                                  unsignedPostfix.end())) {
    bitKind_ = A_Integer::BitKind::UNSIGNED;
    bits_ = 32;
    endPosition = (int)literal.length() - 1;
  } else {
    bitKind_ = A_Integer::BitKind::SIGNED;
    bits_ = 32;
    endPosition = (int)literal.length();
  }

  parsed_ = literal.subString(startPosition, endPosition - startPosition);
}

AstKind A_Integer::kind() const { return AstKind::Integer; }

int A_Integer::bits() const { return bits_; }

int A_Integer::base() const { return base_; }

A_Integer::DecimalKind A_Integer::decimalKind() const { return decimalKind_; }

A_Integer::BitKind A_Integer::bitKind() const { return bitKind_; }

int32_t A_Integer::asInt32() const {
  return static_cast<int32_t>(std::stol(parsed_.str(), nullptr, base_));
}

uint32_t A_Integer::asUInt32() const {
  return static_cast<uint32_t>(std::stoul(parsed_.str(), nullptr, base_));
}

int64_t A_Integer::asInt64() const {
  return static_cast<int64_t>(std::stoll(parsed_.str(), nullptr, base_));
}

uint64_t A_Integer::asUInt64() const {
  return static_cast<uint64_t>(std::stoull(parsed_.str(), nullptr, base_));
}

// A_Integer }

// A_Float {

const static std::unordered_map<A_Float::BitKind, Cowstr> AFL_BC_Map = {
    {A_Float::BitKind::FLT, "FLT"},
    {A_Float::BitKind::DBL, "DBL"},
};

A_Float::A_Float(const Cowstr &literal, const Location &location)
    : Ast(literal, location) {
  LOG_ASSERT(literal.length() > 0, "literal.length {} > 0", literal.length());

  int startPosition = 0;
  int endPosition = (int)literal.length();
  std::vector<Cowstr> doublePostfix = {"d", "D"};
  if (literal.endWithAnyOf(doublePostfix.begin(), doublePostfix.end())) {
    bitKind_ = A_Float::BitKind::DBL;
    bits_ = 64;
    endPosition = (int)literal.length();
  } else {
    bitKind_ = A_Float::BitKind::FLT;
    bits_ = 32;
    endPosition = (int)literal.length() - 1;
  }

  parsed_ = literal.subString(startPosition, endPosition - startPosition);
}

AstKind A_Float::kind() const { return AstKind::Float; }

int A_Float::bits() const { return bits_; }

A_Float::BitKind A_Float::bitKind() const { return bitKind_; }

float A_Float::asFloat() const { return std::stof(parsed_.str()); }

double A_Float::asDouble() const { return std::stod(parsed_.str()); }

// A_Float }

// A_String {

const static std::unordered_map<A_String::QuoteKind, Cowstr> ASL_QC_Map = {
    {A_String::QuoteKind::SINGLE, "SINGLE"},
    {A_String::QuoteKind::TRIPLE, "TRIPLE"},
};

A_String::A_String(const Cowstr &literal, const Location &location)
    : Ast(literal, location) {
  std::vector<Cowstr> multiplePrefix = {"\"\"\""};
  if (literal.length() >= 3 &&
      literal.startWithAnyOf(multiplePrefix.begin(), multiplePrefix.end())) {
    parsed_ = literal.subString(3, literal.length() - 6);
    quoteKind_ = A_String::QuoteKind::TRIPLE;
  } else {
    parsed_ = literal.subString(1, literal.length() - 2);
    quoteKind_ = A_String::QuoteKind::SINGLE;
  }
}

AstKind A_String::kind() const { return AstKind::String; }

A_String::QuoteKind A_String::quoteKind() const { return quoteKind_; }

const Cowstr &A_String::asString() const { return parsed_; }

// A_String }

// A_Character {

A_Character::A_Character(const Cowstr &literal, const Location &location)
    : Ast(literal, location), parsed_(literal[1]) {}

AstKind A_Character::kind() const { return AstKind::Character; }

char A_Character::asChar() const { return parsed_; }

// A_Character }

// A_Boolean {

A_Boolean::A_Boolean(const Cowstr &literal, const Location &location)
    : Ast(literal, location), parsed_(literal == "true") {}

AstKind A_Boolean::kind() const { return AstKind::Boolean; }

bool A_Boolean::asBoolean() const { return parsed_; }

// A_Boolean }

// A_Nil {

A_Nil::A_Nil(const Location &location) : Ast("nil", location) {}

AstKind A_Nil::kind() const { return AstKind::Nil; }

// A_Nil }

// A_Void {

A_Void::A_Void(const Location &location) : Ast("void", location) {}

AstKind A_Void::kind() const { return AstKind::Void; }

// A_Void }

// literal }

// id {

// A_VarId {

A_VarId::A_VarId(const Cowstr &literal, const Location &location, Ast *parent)
    : Ast(literal, location), ParentableImpl(parent) {}

AstKind A_VarId::kind() const { return AstKind::VarId; }

// A_VarId }

// id }

// expr {

// A_Throw {

A_Throw::A_Throw(Ast *a_expr, const Location &location)
    : Ast("throw", location), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
}

A_Throw::~A_Throw() { DEL(expr); }

AstKind A_Throw::kind() const { return AstKind::Throw; }

// A_Throw }

// A_Return {

A_Return::A_Return(Ast *a_expr, const Location &location)
    : Ast("return", location), expr(a_expr) {}

A_Return::~A_Return() { DEL(expr); }

AstKind A_Return::kind() const { return AstKind::Return; }

// A_Return }

// A_Break {

A_Break::A_Break(const Location &location) : Ast("break", location) {}

AstKind A_Break::kind() const { return AstKind::Break; }

// A_Break }

// A_Continue {

A_Continue::A_Continue(const Location &location) : Ast("continue", location) {}

AstKind A_Continue::kind() const { return AstKind::Continue; }

// A_Continue }

// A_Assign {

A_Assign::A_Assign(Ast *a_assignee, int a_assignOp, Ast *a_assignor,
                   const Location &location)
    : Ast(tokenName(a_assignOp), location), assignee(a_assignee),
      assignOp(a_assignOp), assignor(a_assignor) {
  LOG_ASSERT(assignee, "assignee must not null");
  LOG_ASSERT(assignor, "assignor must not null");
}

A_Assign::~A_Assign() {
  DEL(assignee);
  DEL(assignor);
}

AstKind A_Assign::kind() const { return AstKind::Assign; }

// A_Assign }

// A_PostfixExpr {

A_PostfixExpr::A_PostfixExpr(Ast *a_expr, int a_postfixOp,
                             const Location &location)
    : Ast(Cowstr("postfix") + tokenName(a_postfixOp), location), expr(a_expr),
      postfixOp(a_postfixOp) {
  LOG_ASSERT(expr, "expr must not null");
}

A_PostfixExpr::~A_PostfixExpr() { DEL(expr); }

AstKind A_PostfixExpr::kind() const { return AstKind::PostfixExpr; }

// A_PostfixExpr }

// A_InfixExpr {

A_InfixExpr::A_InfixExpr(Ast *a_left, int a_infixOp, Ast *a_right,
                         const Location &location)
    : Ast(Cowstr("infix") + tokenName(a_infixOp), location), left(a_left),
      infixOp(a_infixOp), right(a_right) {
  LOG_ASSERT(left, "left must not null");
  LOG_ASSERT(right, "right must not null");
}

A_InfixExpr::~A_InfixExpr() {
  DEL(left);
  DEL(right);
}

AstKind A_InfixExpr::kind() const { return AstKind::InfixExpr; }

// A_InfixExpr }

// A_PrefixExpr {

A_PrefixExpr::A_PrefixExpr(int a_prefixOp, Ast *a_expr,
                           const Location &location)
    : Ast(Cowstr("prefix") + tokenName(a_prefixOp), location),
      prefixOp(a_prefixOp), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
}

A_PrefixExpr::~A_PrefixExpr() { DEL(expr); }

AstKind A_PrefixExpr::kind() const { return AstKind::PrefixExpr; }

// A_PrefixExpr }

// A_Call {

A_Call::A_Call(Ast *a_id, A_Exprs *a_args, const Location &location)
    : Ast("call", location), id(a_id), args(a_args) {
  LOG_ASSERT(id, "id must not null");
}

A_Call::~A_Call() {
  DEL(id);
  DEL(args);
}

AstKind A_Call::kind() const { return AstKind::Call; }

// A_Call }

// A_Exprs {

A_Exprs::A_Exprs(Ast *a_expr, A_Exprs *a_next, const Location &location)
    : Ast("exprs", location), expr(a_expr), next(a_next) {
  LOG_ASSERT(expr, "id must not null");
}

A_Exprs::~A_Exprs() {
  DEL(expr);
  DEL(next);
}

AstKind A_Exprs::kind() const { return AstKind::Exprs; }

// A_Exprs }

// A_If {

A_If::A_If(Ast *a_condition, Ast *a_thenp, Ast *a_elsep,
           const Location &location)
    : Ast("if", location), condition(a_condition), thenp(a_thenp),
      elsep(a_elsep) {
  LOG_ASSERT(condition, "condition must not null");
  LOG_ASSERT(thenp, "thenp must not null");
}

A_If::~A_If() {
  DEL(condition);
  DEL(thenp);
  DEL(elsep);
}

AstKind A_If::kind() const { return AstKind::If; }

// A_If }

// A_Loop {

A_Loop::A_Loop(Ast *a_condition, Ast *a_body, const Location &location)
    : Ast("loop", location), condition(a_condition), body(a_body) {
  LOG_ASSERT(condition, "condition must not null");
  LOG_ASSERT(body, "body must not null");
}

A_Loop::~A_Loop() {
  DEL(condition);
  DEL(body);
}

AstKind A_Loop::kind() const { return AstKind::Loop; }

// A_Loop }

// A_Yield {

A_Yield::A_Yield(Ast *a_expr, const Location &location)
    : Ast("yield", location), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
}

A_Yield::~A_Yield() { DEL(expr); }

AstKind A_Yield::kind() const { return AstKind::Yield; }

// A_Yield }

// A_LoopCondition {

A_LoopCondition::A_LoopCondition(Ast *a_init, Ast *a_condition, Ast *a_update,
                                 const Location &location)
    : Ast("loopCondition", location), init(a_init), condition(a_condition),
      update(a_update) {}

AstKind A_LoopCondition::kind() const { return AstKind::LoopCondition; }
A_LoopCondition::~A_LoopCondition() {
  DEL(init);
  DEL(condition);
  DEL(update);
}

// A_LoopCondition }

// A_LoopEnumerator {

A_LoopEnumerator::A_LoopEnumerator(Ast *a_id, Ast *a_expr,
                                   const Location &location)
    : Ast("loopEnumerator", location), id(a_id), expr(a_expr) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(expr, "expr must not null");
}

A_LoopEnumerator::~A_LoopEnumerator() {
  DEL(id);
  DEL(expr);
}

AstKind A_LoopEnumerator::kind() const { return AstKind::LoopEnumerator; }

// A_LoopEnumerator }

// A_DoWhile {

A_DoWhile::A_DoWhile(Ast *a_body, Ast *a_condition, const Location &location)
    : Ast("doWhile", location), body(a_body), condition(a_condition) {
  LOG_ASSERT(body, "body must not null");
  LOG_ASSERT(condition, "condition must not null");
}

A_DoWhile::~A_DoWhile() {
  DEL(body);
  DEL(condition);
}

AstKind A_DoWhile::kind() const { return AstKind::DoWhile; }

// A_DoWhile }

// A_Try {

A_Try::A_Try(Ast *a_tryp, Ast *a_catchp, Ast *a_finallyp,
             const Location &location)
    : Ast("try", location), tryp(a_tryp), catchp(a_catchp),
      finallyp(a_finallyp) {
  LOG_ASSERT(tryp, "tryp must not null");
  LOG_ASSERT(catchp, "catchp must not null");
}

A_Try::~A_Try() {
  DEL(tryp);
  DEL(catchp);
  DEL(finallyp);
}

AstKind A_Try::kind() const { return AstKind::Try; }

// A_Try }

// A_Block {

A_Block::A_Block(A_BlockStats *a_blockStats, const Location &location)
    : Ast("block", location), blockStats(a_blockStats) {}

A_Block::~A_Block() { DEL(blockStats); }

AstKind A_Block::kind() const { return AstKind::Block; }

// A_Block }

// A_BlockStats {

A_BlockStats::A_BlockStats(Ast *a_blockStat, A_BlockStats *a_next,
                           const Location &location)
    : Ast("blockStats", location), blockStat(a_blockStat), next(a_next) {}

A_BlockStats::~A_BlockStats() {
  DEL(blockStat);
  DEL(next);
}

AstKind A_BlockStats::kind() const { return AstKind::BlockStats; }

// A_BlockStats }

// expr }

// type {

// A_PlainType {

A_PlainType::A_PlainType(int a_token, const Location &location)
    : Ast("plainType", location), token(a_token) {}

AstKind A_PlainType::kind() const { return AstKind::PlainType; }

// A_PlainType }

// type }

// definition and declaration {

A_FuncDef::A_FuncDef(Ast *a_funcSign, Ast *a_resultType, Ast *a_body,
                     const Location &location)
    : Ast("funcDef", location), funcSign(a_funcSign), resultType(a_resultType),
      body(a_body) {
  LOG_ASSERT(funcSign, "funcSign must not null");
  LOG_ASSERT(resultType, "resultType must not null");
  LOG_ASSERT(body, "body must not null");
}

A_FuncDef::~A_FuncDef() {
  DEL(funcSign);
  DEL(resultType);
  DEL(body);
}

AstKind A_FuncDef::kind() const { return AstKind::FuncDef; }

A_FuncSign::A_FuncSign(Ast *a_id, A_Params *a_params, const Location &location)
    : Ast("funcSign", location), id(a_id), params(a_params) {
  LOG_ASSERT(id, "id must not null");
}

A_FuncSign::~A_FuncSign() {
  DEL(id);
  DEL(params);
}

AstKind A_FuncSign::kind() const { return AstKind::FuncSign; }

A_Params::A_Params(A_Param *a_param, A_Params *a_next, const Location &location)
    : Ast("params", location), param(a_param), next(a_next) {
  LOG_ASSERT(param, "param must not null");
}

A_Params::~A_Params() {
  DEL(param);
  DEL(next);
}

AstKind A_Params::kind() const { return AstKind::Params; }

A_Param::A_Param(Ast *a_id, Ast *a_type, const Location &location)
    : Ast("param", location), id(a_id), type(a_type) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
}

A_Param::~A_Param() {
  DEL(id);
  DEL(type);
}

AstKind A_Param::kind() const { return AstKind::Param; }

A_VarDef::A_VarDef(Ast *a_id, Ast *a_type, Ast *a_expr,
                   const Location &location)
    : Ast("varDef", location), id(a_id), type(a_type), expr(a_expr) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
  LOG_ASSERT(expr, "expr must not null");
}

A_VarDef::~A_VarDef() {
  DEL(id);
  DEL(type);
  DEL(expr);
}

AstKind A_VarDef::kind() const { return AstKind::VarDef; }

// definition and declaration }

// compile unit {

A_TopStats::A_TopStats(Ast *a_topStat, A_TopStats *a_next,
                       const Location &location)
    : Ast("topStats", location), topStat(a_topStat), next(a_next) {}

A_TopStats::~A_TopStats() {
  DEL(topStat);
  DEL(next);
}

AstKind A_TopStats::kind() const { return AstKind::TopStats; }

A_CompileUnit::A_CompileUnit(const &Cowstr name, A_TopStats *a_topStats,
                             const Location &location)
    : Ast(name, location), topStats(a_topStats) {}

A_CompileUnit::~A_CompileUnit() { DEL(topStats); }

AstKind A_CompileUnit::kind() const { return AstKind::CompileUnit; }

// compile unit }
