// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Token.h"
#include "infra/Log.h"
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#define DESTROY(x)                                                             \
  do {                                                                         \
    delete x;                                                                  \
    x = nullptr;                                                               \
  } while (0)

// Ast {

Ast::Ast(const Cowstr &name, const Location &location)
    : Nameable(name), Locationable(location) {}

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

A_Integer::A_Integer(const Cowstr &literal, const Location &location)
    : Ast(literal, location) {
  LOG_ASSERT(literal.length() > 0, "literal.length {} > 0", literal.length());

  int startPosition = 0;
  std::vector<Cowstr> decimalPrefix = {"0x", "0X", "0o", "0O", "0b", "0B"};
  if (literal.startWithAnyOf(decimalPrefix.begin(), decimalPrefix.end())) {
    switch (literal[1]) {
    case 'x':
    case 'X':
      base_ = 16;
      startPosition = 2;
      break;
    case 'o':
    case 'O':
      base_ = 8;
      startPosition = 2;
      break;
    case 'b':
    case 'B':
      base_ = 2;
      startPosition = 2;
      break;
    default:
      break;
    }
  } else {
    base_ = 10;
    startPosition = 0;
  }

  int endPosition = (int)literal.length();
  std::vector<Cowstr> ulongPostfix = {"ul", "UL", "uL", "Ul"};
  std::vector<Cowstr> longPostfix = {"l", "L"};
  std::vector<Cowstr> unsignedPostfix = {"u", "U"};
  if (literal.endWithAnyOf(ulongPostfix.begin(), ulongPostfix.end())) {
    bit_ = 64;
    isSigned_ = false;
    endPosition = (int)literal.length() - 2;
  } else if (literal.endWithAnyOf(longPostfix.begin(), longPostfix.end())) {
    bit_ = 64;
    isSigned_ = true;
    endPosition = (int)literal.length() - 1;
  } else if (literal.endWithAnyOf(unsignedPostfix.begin(),
                                  unsignedPostfix.end())) {
    bit_ = 32;
    isSigned_ = false;
    endPosition = (int)literal.length() - 1;
  } else {
    bit_ = 32;
    isSigned_ = true;
    endPosition = (int)literal.length();
  }

  parsed_ = literal.subString(startPosition, endPosition - startPosition);
}

AstKind A_Integer::kind() const { return AstKind::Integer; }

int A_Integer::bit() const { return bit_; }

bool A_Integer::isSigned() const { return isSigned_; }

int A_Integer::base() const { return base_; }

int32_t A_Integer::asInt32() const {
  LOG_INFO("before parsed: {}", parsed_);
  int32_t r = static_cast<int32_t>(std::stol(parsed_.str(), nullptr, base_));
  LOG_INFO("after parsed: {}, result: {}", parsed_, r);
  return r;
}

uint32_t A_Integer::asUInt32() const {
  LOG_INFO("before parsed: {}", parsed_);
  uint32_t r = static_cast<uint32_t>(std::stoul(parsed_.str(), nullptr, base_));
  LOG_INFO("after parsed: {}, result: {}", parsed_, r);
  return r;
}

int64_t A_Integer::asInt64() const {
  LOG_INFO("before parsed: {}", parsed_);
  int64_t r = static_cast<int64_t>(std::stoll(parsed_.str(), nullptr, base_));
  LOG_INFO("after parsed: {}, result: {}", parsed_, r);
  return r;
}

uint64_t A_Integer::asUInt64() const {
  LOG_INFO("before parsed: {}", parsed_);
  uint64_t r =
      static_cast<uint64_t>(std::stoull(parsed_.str(), nullptr, base_));
  LOG_INFO("after parsed: {}, result: {}", parsed_, r);
  return r;
}

// A_Integer }

// A_Float {

A_Float::A_Float(const Cowstr &literal, const Location &location)
    : Ast(literal, location) {
  LOG_ASSERT(literal.length() > 0, "literal.length {} > 0", literal.length());

  int startPosition = 0;
  int endPosition = (int)literal.length();
  std::vector<Cowstr> doublePostfix = {"d", "D"};
  if (literal.endWithAnyOf(doublePostfix.begin(), doublePostfix.end())) {
    bit_ = 64;
    endPosition = (int)literal.length();
  } else {
    bit_ = 32;
    endPosition = (int)literal.length() - 1;
  }

  parsed_ = literal.subString(startPosition, endPosition - startPosition);
}

AstKind A_Float::kind() const { return AstKind::Float; }

int A_Float::bit() const { return bit_; }

float A_Float::asFloat() const { return std::stof(parsed_.str()); }

double A_Float::asDouble() const { return std::stod(parsed_.str()); }

// A_Float }

// A_String {

A_String::A_String(const Cowstr &literal, const Location &location)
    : Ast(literal, location) {
  std::vector<Cowstr> multiplePrefix = {"\"\"\""};
  if (literal.length() >= 3 &&
      literal.startWithAnyOf(multiplePrefix.begin(), multiplePrefix.end())) {
    parsed_ = literal.subString(3, literal.length() - 6);
    isMultipleLine_ = true;
  } else {
    parsed_ = literal.subString(1, literal.length() - 2);
    isMultipleLine_ = false;
  }
}

AstKind A_String::kind() const { return AstKind::String; }

bool A_String::isMultipleLine() const { return isMultipleLine_; }

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

// AstId::AstId(const Cowstr &literal, const Location &location, Ast *parent)
//     : Ast(literal, location), detail::ParentableImpl(parent) {}

// A_VarId {

A_VarId::A_VarId(const Cowstr &literal, const Location &location)
    : Ast(literal, location) {}

AstKind A_VarId::kind() const { return AstKind::VarId; }

// A_VarId }

// id }

// expr {

// A_Throw {

A_Throw::A_Throw(Ast *a_expr, const Location &location)
    : Ast("throw", location), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
}

A_Throw::~A_Throw() { DESTROY(expr); }

AstKind A_Throw::kind() const { return AstKind::Throw; }

// A_Throw }

// A_Return {

A_Return::A_Return(Ast *a_expr, const Location &location)
    : Ast("return", location), expr(a_expr) {}

A_Return::~A_Return() { DESTROY(expr); }

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
  DESTROY(assignee);
  DESTROY(assignor);
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

A_PostfixExpr::~A_PostfixExpr() { DESTROY(expr); }

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
  DESTROY(left);
  DESTROY(right);
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

A_PrefixExpr::~A_PrefixExpr() { DESTROY(expr); }

AstKind A_PrefixExpr::kind() const { return AstKind::PrefixExpr; }

// A_PrefixExpr }

// A_Call {

A_Call::A_Call(Ast *a_id, A_Exprs *a_args, const Location &location)
    : Ast("call", location), id(a_id), args(a_args) {
  LOG_ASSERT(id, "id must not null");
}

A_Call::~A_Call() {
  DESTROY(id);
  DESTROY(args);
}

AstKind A_Call::kind() const { return AstKind::Call; }

// A_Call }

// A_Exprs {

A_Exprs::A_Exprs(Ast *a_expr, A_Exprs *a_next, const Location &location)
    : Ast("exprs", location), expr(a_expr), next(a_next) {
  LOG_ASSERT(expr, "id must not null");
}

A_Exprs::~A_Exprs() {
  DESTROY(expr);
  DESTROY(next);
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
  DESTROY(condition);
  DESTROY(thenp);
  DESTROY(elsep);
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
  DESTROY(condition);
  DESTROY(body);
}

AstKind A_Loop::kind() const { return AstKind::Loop; }

// A_Loop }

// A_Yield {

A_Yield::A_Yield(Ast *a_expr, const Location &location)
    : Ast("yield", location), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
}

A_Yield::~A_Yield() { DESTROY(expr); }

AstKind A_Yield::kind() const { return AstKind::Yield; }

// A_Yield }

// A_LoopCondition {

A_LoopCondition::A_LoopCondition(Ast *a_init, Ast *a_condition, Ast *a_update,
                                 const Location &location)
    : Ast("loopCondition", location), init(a_init), condition(a_condition),
      update(a_update) {}

AstKind A_LoopCondition::kind() const { return AstKind::LoopCondition; }
A_LoopCondition::~A_LoopCondition() {
  DESTROY(init);
  DESTROY(condition);
  DESTROY(update);
}

// A_LoopCondition }

// A_LoopEnumerator {

A_LoopEnumerator::A_LoopEnumerator(Ast *a_id, Ast *a_type, Ast *a_expr,
                                   const Location &location)
    : Ast("loopEnumerator", location), id(a_id), type(a_type), expr(a_expr) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
  LOG_ASSERT(expr, "expr must not null");
}

A_LoopEnumerator::~A_LoopEnumerator() {
  DESTROY(id);
  DESTROY(type);
  DESTROY(expr);
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
  DESTROY(body);
  DESTROY(condition);
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
  DESTROY(tryp);
  DESTROY(catchp);
  DESTROY(finallyp);
}

AstKind A_Try::kind() const { return AstKind::Try; }

// A_Try }

// A_Block {

A_Block::A_Block(A_BlockStats *a_blockStats, const Location &location)
    : Ast("block", location), blockStats(a_blockStats) {}

A_Block::~A_Block() { DESTROY(blockStats); }

AstKind A_Block::kind() const { return AstKind::Block; }

// A_Block }

// A_BlockStats {

A_BlockStats::A_BlockStats(Ast *a_blockStat, A_BlockStats *a_next,
                           const Location &location)
    : Ast("blockStats", location), blockStat(a_blockStat), next(a_next) {}

A_BlockStats::~A_BlockStats() {
  DESTROY(blockStat);
  DESTROY(next);
}

AstKind A_BlockStats::kind() const { return AstKind::BlockStats; }

// A_BlockStats }

// expr }

// type {

// AstType::AstType(const Cowstr &literal, const Location &location, Ast
// *parent)
//     : Ast(literal, location), detail::ParentableImpl(parent) {}

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
  DESTROY(funcSign);
  DESTROY(resultType);
  DESTROY(body);
}

AstKind A_FuncDef::kind() const { return AstKind::FuncDef; }

A_FuncSign::A_FuncSign(Ast *a_id, A_Params *a_params, const Location &location)
    : Ast("funcSign", location), id(a_id), params(a_params) {
  LOG_ASSERT(id, "id must not null");
}

A_FuncSign::~A_FuncSign() {
  DESTROY(id);
  DESTROY(params);
}

AstKind A_FuncSign::kind() const { return AstKind::FuncSign; }

A_Params::A_Params(A_Param *a_param, A_Params *a_next, const Location &location)
    : Ast("params", location), param(a_param), next(a_next) {
  LOG_ASSERT(param, "param must not null");
}

A_Params::~A_Params() {
  DESTROY(param);
  DESTROY(next);
}

AstKind A_Params::kind() const { return AstKind::Params; }

A_Param::A_Param(Ast *a_id, Ast *a_type, const Location &location)
    : Ast("param", location), id(a_id), type(a_type) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
}

A_Param::~A_Param() {
  DESTROY(id);
  DESTROY(type);
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
  DESTROY(id);
  DESTROY(type);
  DESTROY(expr);
}

AstKind A_VarDef::kind() const { return AstKind::VarDef; }

// definition and declaration }

// compile unit {

A_TopStats::A_TopStats(Ast *a_topStat, A_TopStats *a_next,
                       const Location &location)
    : Ast("topStats", location), topStat(a_topStat), next(a_next) {}

A_TopStats::~A_TopStats() {
  DESTROY(topStat);
  DESTROY(next);
}

AstKind A_TopStats::kind() const { return AstKind::TopStats; }

A_CompileUnit::A_CompileUnit(const Cowstr &name, A_TopStats *a_topStats,
                             const Location &location)
    : Ast(name, location), topStats(a_topStats) {}

A_CompileUnit::~A_CompileUnit() { DESTROY(topStats); }

AstKind A_CompileUnit::kind() const { return AstKind::CompileUnit; }

// compile unit }
