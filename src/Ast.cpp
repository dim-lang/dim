// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Strings.h"
#include "Token.h"
#include "parser.tab.hh"
#include <algorithm>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#define DLT(x)                                                                 \
  do {                                                                         \
    if (x) {                                                                   \
      delete x;                                                                \
      x = nullptr;                                                             \
    }                                                                          \
  } while (0)

// Ast {

Ast::Ast(const std::string &name, const Location &location)
    : Nameable(name), Locationable(location) {}

std::string Ast::toString() const { return name().toString(); }

bool Ast::isLiteral(Ast *e) {
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

bool Ast::isId(Ast *e) { return e && e->category() == (+AstCategory::VarId); }

bool Ast::isExpr(Ast *e) {
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

bool Ast::isDef(Ast *e) {
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

bool Ast::isDecl(Ast *e) {
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

bool Ast::isType(Ast *e) {
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

A_Integer::A_Integer(const std::string &literal, const Location &location)
    : Ast("integerLiteral", location), literal_(literal) {
  LOG_ASSERT(literal_.length() > 0, "literal_.length {} > 0",
             literal_.length());

  int startPosition = 0;
  std::vector<std::string> decimalPrefix = {"0x", "0X", "0o", "0O", "0b", "0B"};
  if (Strings::startWith(literal_, decimalPrefix.begin(),
                         decimalPrefix.end())) {
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
      break;
    }
  } else {
    decimalCategory_ = A_Integer::DecimalCategory::DEC;
    base_ = 10;
    startPosition = 0;
  }

  int endPosition = (int)literal_.length();
  std::vector<std::string> bitPostfix = {"ul", "UL", "uL", "Ul"};
  std::vector<char> longPostfix = {'l', 'L'};
  std::vector<char> unsignedPostfix = {'u', 'U'};
  if (Strings::endWith(literal_, bitPostfix.begin(), bitPostfix.end())) {
    bitCategory_ = A_Integer::BitCategory::ULONG;
    bits_ = 64;
    endPosition = (int)literal_.length() - 2;
  } else if (Strings::endWith(literal_, longPostfix.begin(),
                              longPostfix.end())) {
    bitCategory_ = A_Integer::BitCategory::LONG;
    bits_ = 64;
    endPosition = (int)literal_.length() - 1;
  } else if (Strings::endWith(literal_, unsignedPostfix.begin(),
                              unsignedPostfix.end())) {
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

const std::string &A_Integer::literal() const { return literal_; }

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

A_Float::A_Float(const std::string &literal, const Location &location)
    : Ast("floatLiteral", location), literal_(literal) {
  LOG_ASSERT(literal_.length() > 0, "literal_.length {} > 0",
             literal_.length());

  int startPosition = 0;
  int endPosition = (int)literal_.length();
  std::vector<char> doublePostfix = {'d', 'D'};
  if (Strings::endWith(literal_, doublePostfix.begin(), doublePostfix.end())) {
    bitCategory_ = A_Float::BitCategory::DBL;
    bits_ = 64;
    endPosition = (int)literal_.length();
  } else {
    bitCategory_ = A_Float::BitCategory::FLT;
    bits_ = 32;
    endPosition = (int)literal_.length() - 1;
  }

  parsed_ = literal_.substr(startPosition, endPosition - startPosition);
}

AstCategory A_Float::category() const { return AstCategory::Float; }

const std::string &A_Float::literal() const { return literal_; }

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

A_String::A_String(const std::string &literal, const Location &location)
    : Ast("stringLiteral", location), literal_(literal) {
  std::vector<std::string> multiplePrefix = {"\"\"\""};
  parsed_ = literal_.length() >= 3 &&
                    Strings::startWith(literal_, multiplePrefix.begin(),
                                       multiplePrefix.end())
                ? literal_.substr(3, literal_.length() - 6)
                : literal_.substr(1, literal_.length() - 2);
  quoteCategory_ = literal_.length() >= 3 &&
                           Strings::startWith(literal_, multiplePrefix.begin(),
                                              multiplePrefix.end())
                       ? A_String::QuoteCategory::TRIPLE
                       : A_String::QuoteCategory::SINGLE;
}

AstCategory A_String::category() const { return AstCategory::String; }

const std::string &A_String::literal() const { return literal_; }

A_String::QuoteCategory A_String::quoteCategory() const {
  return quoteCategory_;
}

const std::string &A_String::asString() const { return parsed_; }

// A_String }

// A_Character {

A_Character::A_Character(const std::string &literal, const Location &location)
    : Ast("characterLiteral", location), literal_(literal),
      parsed_(literal[1]) {}

AstCategory A_Character::category() const { return AstCategory::Character; }

const std::string &A_Character::literal() const { return literal_; }

char A_Character::asChar() const { return parsed_; }

// A_Character }

// A_Boolean {

A_Boolean::A_Boolean(const std::string &literal, const Location &location)
    : Ast("booleanLiteral", location), literal_(literal),
      parsed_(literal == "true") {}

AstCategory A_Boolean::category() const { return AstCategory::Boolean; }

const std::string &A_Boolean::literal() const { return literal_; }

bool A_Boolean::asBoolean() const { return parsed_; }

// A_Boolean }

// A_Nil {

A_Nil::A_Nil(const Location &location) : Ast("nil", location) {}

AstCategory A_Nil::category() const { return AstCategory::Nil; }

// A_Nil }

// A_Void {

A_Void::A_Void(const Location &location) : Ast("void", location) {}

AstCategory A_Void::category() const { return AstCategory::Void; }

// A_Void }

// literal }

// id {

// A_VarId {

A_VarId::A_VarId(const std::string &literal, const Location &location)
    : Ast("varId", location), literal_(literal) {}

AstCategory A_VarId::category() const { return AstCategory::VarId; }

const std::string &A_VarId::literal() const { return literal_; }

// A_VarId }

// id }

// expr {

// A_Throw {

A_Throw::A_Throw(Ast *a_expr, const Location &location)
    : Ast("throw", location), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
}

A_Throw::~A_Throw() { DLT(expr); }

AstCategory A_Throw::category() const { return AstCategory::Throw; }

// A_Throw }

// A_Return {

A_Return::A_Return(Ast *a_expr, const Location &location)
    : Ast("return", location), expr(a_expr) {}

A_Return::~A_Return() { DLT(expr); }

AstCategory A_Return::category() const { return AstCategory::Return; }

// A_Return }

// A_Break {

A_Break::A_Break(const Location &location) : Ast("break", location) {}

AstCategory A_Break::category() const { return AstCategory::Break; }

// A_Break }

// A_Continue {

A_Continue::A_Continue(const Location &location) : Ast("continue", location) {}

AstCategory A_Continue::category() const { return AstCategory::Continue; }

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
  DLT(assignee);
  DLT(assignor);
}

AstCategory A_Assign::category() const { return AstCategory::Assign; }

// A_Assign }

// A_PostfixExpr {

A_PostfixExpr::A_PostfixExpr(Ast *a_expr, int a_postfixOp,
                             const Location &location)
    : Ast("postfixExpr", location), expr(a_expr), postfixOp(a_postfixOp) {
  LOG_ASSERT(expr, "expr must not null");
}

A_PostfixExpr::~A_PostfixExpr() { DLT(expr); }

AstCategory A_PostfixExpr::category() const { return AstCategory::PostfixExpr; }

// A_PostfixExpr }

// A_InfixExpr {

A_InfixExpr::A_InfixExpr(Ast *a_left, int a_infixOp, Ast *a_right,
                         const Location &location)
    : Ast("infixExpr", location), left(a_left), infixOp(a_infixOp),
      right(a_right) {
  LOG_ASSERT(left, "left must not null");
  LOG_ASSERT(right, "right must not null");
}

A_InfixExpr::~A_InfixExpr() {
  DLT(left);
  DLT(right);
}

AstCategory A_InfixExpr::category() const { return AstCategory::InfixExpr; }

// A_InfixExpr }

// A_PrefixExpr {

A_PrefixExpr::A_PrefixExpr(int a_prefixOp, Ast *a_expr,
                           const Location &location)
    : Ast("prefixExpr", location), prefixOp(a_prefixOp), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
}

A_PrefixExpr::~A_PrefixExpr() { DLT(expr); }

AstCategory A_PrefixExpr::category() const { return AstCategory::PrefixExpr; }

// A_PrefixExpr }

// A_Call {

A_Call::A_Call(Ast *a_id, A_Exprs *a_args, const Location &location)
    : Ast("call", location), id(a_id), args(a_args) {
  LOG_ASSERT(id, "id must not null");
}

A_Call::~A_Call() {
  DLT(id);
  DLT(args);
}

AstCategory A_Call::category() const { return AstCategory::Call; }

// A_Call }

// A_Exprs {

A_Exprs::A_Exprs(Ast *a_expr, A_Exprs *a_next, const Location &location)
    : Ast("exprs", location), expr(a_expr), next(a_next) {
  LOG_ASSERT(expr, "id must not null");
}

A_Exprs::~A_Exprs() {
  DLT(expr);
  DLT(next);
}

AstCategory A_Exprs::category() const { return AstCategory::Exprs; }

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
  DLT(condition);
  DLT(thenp);
  DLT(elsep);
}

AstCategory A_If::category() const { return AstCategory::If; }

// A_If }

// A_Loop {

A_Loop::A_Loop(Ast *a_condition, Ast *a_body, const Location &location)
    : Ast("loop", location), condition(a_condition), body(a_body) {
  LOG_ASSERT(condition, "condition must not null");
  LOG_ASSERT(body, "body must not null");
}

A_Loop::~A_Loop() {
  DLT(condition);
  DLT(body);
}

AstCategory A_Loop::category() const { return AstCategory::Loop; }

// A_Loop }

// A_Yield {

A_Yield::A_Yield(Ast *a_expr, const Location &location)
    : Ast("yield", location), expr(a_expr) {
  LOG_ASSERT(expr, "expr must not null");
}

A_Yield::~A_Yield() { DLT(expr); }

AstCategory A_Yield::category() const { return AstCategory::Yield; }

// A_Yield }

// A_LoopCondition {

A_LoopCondition::A_LoopCondition(Ast *a_init, Ast *a_condition, Ast *a_update,
                                 const Location &location)
    : Ast("loopCondition", location), init(a_init), condition(a_condition),
      update(a_update) {}

AstCategory A_LoopCondition::category() const {
  return AstCategory::LoopCondition;
}
A_LoopCondition::~A_LoopCondition() {
  DLT(init);
  DLT(condition);
  DLT(update);
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
  DLT(id);
  DLT(expr);
}

AstCategory A_LoopEnumerator::category() const {
  return AstCategory::LoopEnumerator;
}

// A_LoopEnumerator }

// A_DoWhile {

A_DoWhile::A_DoWhile(Ast *a_body, Ast *a_condition, const Location &location)
    : Ast("dowhile", location), body(a_body), condition(a_condition) {
  LOG_ASSERT(body, "body must not null");
  LOG_ASSERT(condition, "condition must not null");
}

A_DoWhile::~A_DoWhile() {
  DLT(body);
  DLT(condition);
}

AstCategory A_DoWhile::category() const { return AstCategory::DoWhile; }

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
  DLT(tryp);
  DLT(catchp);
  DLT(finallyp);
}

AstCategory A_Try::category() const { return AstCategory::Try; }

// A_Try }

// A_Block {

A_Block::A_Block(A_BlockStats *a_blockStats, const Location &location)
    : Ast("block", location), blockStats(a_blockStats) {}

A_Block::~A_Block() { DLT(blockStats); }

AstCategory A_Block::category() const { return AstCategory::Block; }

// A_Block }

// A_BlockStats {

A_BlockStats::A_BlockStats(Ast *a_blockStat, A_BlockStats *a_next,
                           const Location &location)
    : Ast("blockStats", location), blockStat(a_blockStat), next(a_next) {}

A_BlockStats::~A_BlockStats() {
  DLT(blockStat);
  DLT(next);
}

AstCategory A_BlockStats::category() const { return AstCategory::BlockStats; }

// A_BlockStats }

// expr }

// type {

// A_PlainType {

A_PlainType::A_PlainType(int a_token, const Location &location)
    : Ast("plainType", location), token(a_token) {}

AstCategory A_PlainType::category() const { return AstCategory::PlainType; }

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
  DLT(funcSign);
  DLT(resultType);
  DLT(body);
}

AstCategory A_FuncDef::category() const { return AstCategory::FuncDef; }

A_FuncSign::A_FuncSign(Ast *a_id, A_Params *a_params, const Location &location)
    : Ast("funcSign", location), id(a_id), params(a_params) {
  LOG_ASSERT(id, "id must not null");
}

A_FuncSign::~A_FuncSign() {
  DLT(id);
  DLT(params);
}

AstCategory A_FuncSign::category() const { return AstCategory::FuncSign; }

A_Params::A_Params(A_Param *a_param, A_Params *a_next, const Location &location)
    : Ast("params", location), param(a_param), next(a_next) {
  LOG_ASSERT(param, "param must not null");
}

A_Params::~A_Params() {
  DLT(param);
  DLT(next);
}

AstCategory A_Params::category() const { return AstCategory::Params; }

A_Param::A_Param(Ast *a_id, Ast *a_type, const Location &location)
    : Ast("param", location), id(a_id), type(a_type) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
}

A_Param::~A_Param() {
  DLT(id);
  DLT(type);
}

AstCategory A_Param::category() const { return AstCategory::Param; }

A_VarDef::A_VarDef(Ast *a_id, Ast *a_type, Ast *a_expr,
                   const Location &location)
    : Ast("varDef", location), id(a_id), type(a_type), expr(a_expr) {
  LOG_ASSERT(id, "id must not null");
  LOG_ASSERT(type, "type must not null");
  LOG_ASSERT(expr, "expr must not null");
}

A_VarDef::~A_VarDef() {
  DLT(id);
  DLT(type);
  DLT(expr);
}

AstCategory A_VarDef::category() const { return AstCategory::VarDef; }

// definition and declaration }

// compile unit {

A_TopStats::A_TopStats(Ast *a_topStat, A_TopStats *a_next,
                       const Location &location)
    : Ast("topStats", location), topStat(a_topStat), next(a_next) {}

A_TopStats::~A_TopStats() {
  DLT(topStat);
  DLT(next);
}

AstCategory A_TopStats::category() const { return AstCategory::TopStats; }

A_CompileUnit::A_CompileUnit(A_TopStats *a_topStats, const Location &location)
    : Ast("compileUnit", location), topStats(a_topStats) {}

A_CompileUnit::~A_CompileUnit() { DLT(topStats); }

AstCategory A_CompileUnit::category() const { return AstCategory::CompileUnit; }

// compile unit }
