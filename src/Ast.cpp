// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Token.h"
#include "fmt/format.h"
#include "iface/Visitor.h"
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

#define PARENT(x)                                                              \
  do {                                                                         \
    if (x) {                                                                   \
      (x)->parent() = this;                                                    \
    }                                                                          \
  } while (0)

// detail::Parentable {

namespace detail {

Parentable::Parentable(Ast *parent) : parentable_(parent) {}

Ast *&Parentable::parent() { return parentable_; }

Ast *Parentable::parent() const { return parentable_; }

} // namespace detail

// detail::Parentable }

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
  case AstKind::Postfix:
  case AstKind::Prefix:
  case AstKind::Infix:
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

void A_Integer::accept(Visitor *visitor) { visitor->visitInteger(this); }

int A_Integer::bit() const { return bit_; }

bool A_Integer::isSigned() const { return isSigned_; }

int A_Integer::base() const { return base_; }

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

void A_Float::accept(Visitor *visitor) { visitor->visitFloat(this); }

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

void A_String::accept(Visitor *visitor) { visitor->visitString(this); }

bool A_String::isMultipleLine() const { return isMultipleLine_; }

const Cowstr &A_String::asString() const { return parsed_; }

// A_String }

// A_Character {

A_Character::A_Character(const Cowstr &literal, const Location &location)
    : Ast(literal, location), parsed_(literal[1]) {}

AstKind A_Character::kind() const { return AstKind::Character; }

void A_Character::accept(Visitor *visitor) { visitor->visitCharacter(this); }

char A_Character::asChar() const { return parsed_; }

// A_Character }

// A_Boolean {

A_Boolean::A_Boolean(const Cowstr &literal, const Location &location)
    : Ast(literal, location), parsed_(literal == "true") {}

AstKind A_Boolean::kind() const { return AstKind::Boolean; }

void A_Boolean::accept(Visitor *visitor) { visitor->visitBoolean(this); }

bool A_Boolean::asBoolean() const { return parsed_; }

// A_Boolean }

// A_Nil {

A_Nil::A_Nil(const Location &location) : Ast("nil", location) {}

AstKind A_Nil::kind() const { return AstKind::Nil; }

void A_Nil::accept(Visitor *visitor) { visitor->visitNil(this); }

// A_Nil }

// A_Void {

A_Void::A_Void(const Location &location) : Ast("void", location) {}

AstKind A_Void::kind() const { return AstKind::Void; }

void A_Void::accept(Visitor *visitor) { visitor->visitVoid(this); }

// A_Void }

// literal }

// id {

// AstId::AstId(const Cowstr &literal, const Location &location, Ast *parent)
//     : Ast(literal, location), detail::ParentableImpl(parent) {}

// A_VarId {

A_VarId::A_VarId(const Cowstr &literal, const Location &location)
    : Ast(literal, location) {}

AstKind A_VarId::kind() const { return AstKind::VarId; }

void A_VarId::accept(Visitor *visitor) { visitor->visitVarId(this); }

// A_VarId }

// id }

// expr {

// A_Throw {

A_Throw::A_Throw(Ast *a_expr, const Location &location)
    : Ast("throw", location), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
  PARENT(expr);
}

A_Throw::~A_Throw() { DESTROY(expr); }

AstKind A_Throw::kind() const { return AstKind::Throw; }

void A_Throw::accept(Visitor *visitor) { visitor->visitThrow(this); }

// A_Throw }

// A_Return {

A_Return::A_Return(Ast *a_expr, const Location &location)
    : Ast("return", location), expr(a_expr) {
  PARENT(expr);
}

A_Return::~A_Return() { DESTROY(expr); }

AstKind A_Return::kind() const { return AstKind::Return; }

void A_Return::accept(Visitor *visitor) { visitor->visitReturn(this); }

// A_Return }

// A_Break {

A_Break::A_Break(const Location &location) : Ast("break", location) {}

AstKind A_Break::kind() const { return AstKind::Break; }

void A_Break::accept(Visitor *visitor) { visitor->visitBreak(this); }

// A_Break }

// A_Continue {

A_Continue::A_Continue(const Location &location) : Ast("continue", location) {}

AstKind A_Continue::kind() const { return AstKind::Continue; }

void A_Continue::accept(Visitor *visitor) { visitor->visitContinue(this); }

// A_Continue }

// A_Assign {

A_Assign::A_Assign(Ast *a_assignee, int a_assignOp, Ast *a_assignor,
                   const Location &location)
    : Ast(tokenName(a_assignOp), location), assignee(a_assignee),
      assignOp(a_assignOp), assignor(a_assignor) {
  LOG_ASSERT(assignee, "assignee must not null");
  LOG_ASSERT(assignor, "assignor must not null");
  PARENT(assignee);
  PARENT(assignor);
}

A_Assign::~A_Assign() {
  DESTROY(assignee);
  DESTROY(assignor);
}

AstKind A_Assign::kind() const { return AstKind::Assign; }

void A_Assign::accept(Visitor *visitor) { visitor->visitAssign(this); }

// A_Assign }

// A_Postfix {

A_Postfix::A_Postfix(Ast *a_expr, int a_postfixOp, const Location &location)
    : Ast(tokenName(a_postfixOp), location), expr(a_expr),
      postfixOp(a_postfixOp) {
  LOG_ASSERT(expr, "expr must not null");
  PARENT(expr);
}

A_Postfix::~A_Postfix() { DESTROY(expr); }

AstKind A_Postfix::kind() const { return AstKind::Postfix; }

void A_Postfix::accept(Visitor *visitor) { visitor->visitPostfix(this); }

// A_Postfix }

// A_Infix {

A_Infix::A_Infix(Ast *a_left, int a_infixOp, Ast *a_right,
                 const Location &location)
    : Ast(tokenName(a_infixOp), location), left(a_left), infixOp(a_infixOp),
      right(a_right) {
  LOG_ASSERT(left, "left must not null");
  LOG_ASSERT(right, "right must not null");
  PARENT(left);
  PARENT(right);
}

A_Infix::~A_Infix() {
  DESTROY(left);
  DESTROY(right);
}

AstKind A_Infix::kind() const { return AstKind::Infix; }

void A_Infix::accept(Visitor *visitor) { visitor->visitInfix(this); }

// A_Infix }

// A_Prefix {

A_Prefix::A_Prefix(int a_prefixOp, Ast *a_expr, const Location &location)
    : Ast(tokenName(a_prefixOp), location), prefixOp(a_prefixOp), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
  PARENT(expr);
}

A_Prefix::~A_Prefix() { DESTROY(expr); }

AstKind A_Prefix::kind() const { return AstKind::Prefix; }

void A_Prefix::accept(Visitor *visitor) { visitor->visitPrefix(this); }

// A_Prefix }

// A_Call {

A_Call::A_Call(Ast *a_id, A_Exprs *a_args, const Location &location)
    : Ast("call", location), id(a_id), args(a_args) {
  LOG_ASSERT(id, "id must not null");
  PARENT(id);
  PARENT(args);
}

A_Call::~A_Call() {
  DESTROY(id);
  DESTROY(args);
}

AstKind A_Call::kind() const { return AstKind::Call; }

void A_Call::accept(Visitor *visitor) { visitor->visitCall(this); }

// A_Call }

// A_Exprs {

A_Exprs::A_Exprs(Ast *a_expr, A_Exprs *a_next, const Location &location)
    : Ast("exprs", location), expr(a_expr), next(a_next) {
  LOG_ASSERT(expr, "id must not null");
  PARENT(expr);
  PARENT(next);
}

A_Exprs::~A_Exprs() {
  DESTROY(expr);
  DESTROY(next);
}

AstKind A_Exprs::kind() const { return AstKind::Exprs; }

void A_Exprs::accept(Visitor *visitor) { visitor->visitExprs(this); }

// A_Exprs }

// A_If {

A_If::A_If(Ast *a_condition, Ast *a_thenp, Ast *a_elsep,
           const Location &location)
    : Ast("if", location), condition(a_condition), thenp(a_thenp),
      elsep(a_elsep) {
  LOG_ASSERT(condition, "condition must not null");
  LOG_ASSERT(thenp, "thenp must not null");
  PARENT(condition);
  PARENT(thenp);
  PARENT(elsep);
}

A_If::~A_If() {
  DESTROY(condition);
  DESTROY(thenp);
  DESTROY(elsep);
}

AstKind A_If::kind() const { return AstKind::If; }

void A_If::accept(Visitor *visitor) { visitor->visitIf(this); }

// A_If }

// A_Loop {

A_Loop::A_Loop(Ast *a_condition, Ast *a_body, const Location &location)
    : Ast("loop", location), condition(a_condition), body(a_body) {
  LOG_ASSERT(condition, "condition must not null");
  LOG_ASSERT(body, "body must not null");
  PARENT(condition);
  PARENT(body);
}

A_Loop::~A_Loop() {
  DESTROY(condition);
  DESTROY(body);
}

AstKind A_Loop::kind() const { return AstKind::Loop; }

void A_Loop::accept(Visitor *visitor) { visitor->visitLoop(this); }

// A_Loop }

// A_Yield {

A_Yield::A_Yield(Ast *a_expr, const Location &location)
    : Ast("yield", location), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
  PARENT(expr);
}

A_Yield::~A_Yield() { DESTROY(expr); }

AstKind A_Yield::kind() const { return AstKind::Yield; }

void A_Yield::accept(Visitor *visitor) { visitor->visitYield(this); }

// A_Yield }

// A_LoopCondition {

A_LoopCondition::A_LoopCondition(Ast *a_init, Ast *a_condition, Ast *a_update,
                                 const Location &location)
    : Ast("loopCondition", location), init(a_init), condition(a_condition),
      update(a_update) {
  PARENT(init);
  PARENT(condition);
  PARENT(update);
}

AstKind A_LoopCondition::kind() const { return AstKind::LoopCondition; }

A_LoopCondition::~A_LoopCondition() {
  DESTROY(init);
  DESTROY(condition);
  DESTROY(update);
}

void A_LoopCondition::accept(Visitor *visitor) {
  visitor->visitLoopCondition(this);
}

// A_LoopCondition }

// A_LoopEnumerator {

A_LoopEnumerator::A_LoopEnumerator(Ast *a_id, Ast *a_type, Ast *a_expr,
                                   const Location &location)
    : Ast("loopEnumerator", location), id(a_id), type(a_type), expr(a_expr) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
  LOG_ASSERT(expr, "expr must not null");
  PARENT(id);
  PARENT(type);
  PARENT(expr);
}

A_LoopEnumerator::~A_LoopEnumerator() {
  DESTROY(id);
  DESTROY(type);
  DESTROY(expr);
}

AstKind A_LoopEnumerator::kind() const { return AstKind::LoopEnumerator; }

void A_LoopEnumerator::accept(Visitor *visitor) {
  visitor->visitLoopEnumerator(this);
}

// A_LoopEnumerator }

// A_DoWhile {

A_DoWhile::A_DoWhile(Ast *a_body, Ast *a_condition, const Location &location)
    : Ast("doWhile", location), body(a_body), condition(a_condition) {
  LOG_ASSERT(body, "body must not null");
  LOG_ASSERT(condition, "condition must not null");
  PARENT(body);
  PARENT(condition);
}

A_DoWhile::~A_DoWhile() {
  DESTROY(body);
  DESTROY(condition);
}

AstKind A_DoWhile::kind() const { return AstKind::DoWhile; }

void A_DoWhile::accept(Visitor *visitor) { visitor->visitDoWhile(this); }

// A_DoWhile }

// A_Try {

A_Try::A_Try(Ast *a_tryp, Ast *a_catchp, Ast *a_finallyp,
             const Location &location)
    : Ast("try", location), tryp(a_tryp), catchp(a_catchp),
      finallyp(a_finallyp) {
  LOG_ASSERT(tryp, "tryp must not null");
  LOG_ASSERT(catchp, "catchp must not null");
  PARENT(tryp);
  PARENT(catchp);
  PARENT(finallyp);
}

A_Try::~A_Try() {
  DESTROY(tryp);
  DESTROY(catchp);
  DESTROY(finallyp);
}

AstKind A_Try::kind() const { return AstKind::Try; }

void A_Try::accept(Visitor *visitor) { visitor->visitTry(this); }

// A_Try }

// A_Block {

A_Block::A_Block(A_BlockStats *a_blockStats, const Location &location)
    : Ast("block", location), blockStats(a_blockStats) {
  PARENT(blockStats);
}

A_Block::~A_Block() { DESTROY(blockStats); }

AstKind A_Block::kind() const { return AstKind::Block; }

void A_Block::accept(Visitor *visitor) { visitor->visitBlock(this); }

// A_Block }

// A_BlockStats {

A_BlockStats::A_BlockStats(Ast *a_blockStat, A_BlockStats *a_next,
                           const Location &location)
    : Ast("blockStats", location), blockStat(a_blockStat), next(a_next) {
  LOG_ASSERT(blockStat, "blockStat must not null");
  PARENT(blockStat);
  PARENT(next);
}

A_BlockStats::~A_BlockStats() {
  DESTROY(blockStat);
  DESTROY(next);
}

AstKind A_BlockStats::kind() const { return AstKind::BlockStats; }

void A_BlockStats::accept(Visitor *visitor) { visitor->visitBlockStats(this); }

// A_BlockStats }

// expr }

// type {

// AstType::AstType(const Cowstr &literal, const Location &location, Ast
// *parent)
//     : Ast(literal, location), detail::ParentableImpl(parent) {}

// A_PlainType {

A_PlainType::A_PlainType(int a_token, const Location &location)
    : Ast(tokenName(a_token), location), token(a_token) {}

AstKind A_PlainType::kind() const { return AstKind::PlainType; }

void A_PlainType::accept(Visitor *visitor) { visitor->visitPlainType(this); }

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
  PARENT(funcSign);
  PARENT(resultType);
  PARENT(body);
}

A_FuncDef::~A_FuncDef() {
  DESTROY(funcSign);
  DESTROY(resultType);
  DESTROY(body);
}

AstKind A_FuncDef::kind() const { return AstKind::FuncDef; }

void A_FuncDef::accept(Visitor *visitor) { visitor->visitFuncDef(this); }

Ast *A_FuncDef::getId() const {
  LOG_ASSERT(funcSign->kind() == +AstKind::FuncSign,
             "funcSign kind {} != AstKind::FuncSign",
             funcSign->kind()._to_string());
  return static_cast<A_FuncSign *>(funcSign)->id;
}

std::vector<std::pair<Ast *, Ast *>> A_FuncDef::getArguments() const {
  LOG_ASSERT(funcSign->kind() == +AstKind::FuncSign,
             "funcSign kind {} != AstKind::FuncSign",
             funcSign->kind()._to_string());
  A_FuncSign *fs = static_cast<A_FuncSign *>(funcSign);
  std::vector<std::pair<Ast *, Ast *>> argList;
  for (A_Params *params = fs->params; params; params = params->next) {
    A_Param *param = params->param;
    argList.push_back(std::make_pair(param->id, param->type));
  }
  return argList;
}

A_FuncSign::A_FuncSign(Ast *a_id, A_Params *a_params, const Location &location)
    : Ast("funcSign", location), id(a_id), params(a_params) {
  LOG_ASSERT(id, "id must not null");
  PARENT(id);
  PARENT(params);
}

A_FuncSign::~A_FuncSign() {
  DESTROY(id);
  DESTROY(params);
}

AstKind A_FuncSign::kind() const { return AstKind::FuncSign; }

void A_FuncSign::accept(Visitor *visitor) { visitor->visitFuncSign(this); }

A_Params::A_Params(A_Param *a_param, A_Params *a_next, const Location &location)
    : Ast("params", location), param(a_param), next(a_next) {
  LOG_ASSERT(param, "param must not null");
  PARENT(param);
  PARENT(next);
}

A_Params::~A_Params() {
  DESTROY(param);
  DESTROY(next);
}

AstKind A_Params::kind() const { return AstKind::Params; }

void A_Params::accept(Visitor *visitor) { visitor->visitParams(this); }

A_Param::A_Param(Ast *a_id, Ast *a_type, const Location &location)
    : Ast("param", location), id(a_id), type(a_type) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
  PARENT(id);
  PARENT(type);
}

A_Param::~A_Param() {
  DESTROY(id);
  DESTROY(type);
}

AstKind A_Param::kind() const { return AstKind::Param; }

void A_Param::accept(Visitor *visitor) { visitor->visitParam(this); }

A_VarDef::A_VarDef(Ast *a_id, Ast *a_type, Ast *a_expr,
                   const Location &location)
    : Ast("varDef", location), id(a_id), type(a_type), expr(a_expr) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
  LOG_ASSERT(expr, "expr must not null");
  PARENT(id);
  PARENT(type);
  PARENT(expr);
}

A_VarDef::~A_VarDef() {
  DESTROY(id);
  DESTROY(type);
  DESTROY(expr);
}

AstKind A_VarDef::kind() const { return AstKind::VarDef; }

void A_VarDef::accept(Visitor *visitor) { visitor->visitVarDef(this); }

// definition and declaration }

// compile unit {

A_TopStats::A_TopStats(Ast *a_topStat, A_TopStats *a_next,
                       const Location &location)
    : Ast("topStats", location), topStat(a_topStat), next(a_next) {
  LOG_ASSERT(topStat, "topStat must not null");
  PARENT(topStat);
  PARENT(next);
}

A_TopStats::~A_TopStats() {
  DESTROY(topStat);
  DESTROY(next);
}

AstKind A_TopStats::kind() const { return AstKind::TopStats; }

void A_TopStats::accept(Visitor *visitor) { visitor->visitTopStats(this); }

A_CompileUnit::A_CompileUnit(const Cowstr &name, A_TopStats *a_topStats,
                             const Location &location)
    : Ast(name, location), topStats(a_topStats) {
  PARENT(topStats);
}

A_CompileUnit::~A_CompileUnit() { DESTROY(topStats); }

AstKind A_CompileUnit::kind() const { return AstKind::CompileUnit; }

void A_CompileUnit::accept(Visitor *visitor) {
  visitor->visitCompileUnit(this);
}

// compile unit }
