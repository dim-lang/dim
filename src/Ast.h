// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Location.h"
#include "Name.h"
#include "boost/core/noncopyable.hpp"
#include "enum.h"
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <sstream>
#include <string>

/*================ type start from 1000 ================*/
BETTER_ENUM(AstCategory, int,
            // literal
            Integer = 1000, Float, Boolean, Character, String, Nil, Void,
            // id
            VarId,
            // expr without block
            Throw, Return, Break, Continue, Assign, PostfixExpr, PrefixExpr,
            InfixExpr, Call, Exprs,
            // expr with block
            If, Loop, Yield, LoopCondition, LoopEnumerator, DoWhile, Try, Block,
            BlockStats,
            // type
            PlainType,
            // declaration and definition
            FuncDef, FuncSign, Params, Param, VarDef,
            // compile unit
            CompileUnit, TopStats)

/*================ class ================*/

/* ast */
class Ast;

/* literal */
class A_Integer;
class A_Float;
class A_Boolean;
class A_Character;
class A_String;
class A_Nil;
class A_Void;

/* id */
class A_VarId;

/* expression without block */
class A_Throw;
class A_Return;
class A_Break;
class A_Continue;
class A_Assign;
class A_PostfixExpr;
class A_InfixExpr;
class A_PrefixExpr;

/* expression with block */
class A_Call;
class A_Exprs;
class A_If;
class A_Loop;
class A_Yield;
class A_LoopCondition;
class A_LoopEnumerator;
class A_DoWhile;
class A_Try;
class A_Block;
class A_BlockStats;

/* type */
class A_PlainType;

/* declaration and definition */
class A_FuncDef;
class A_FuncSign;
class A_Params;
class A_Param;
class A_VarDef;

/* compile unit */
class A_TopStats;
class A_CompileUnit;

// Ast {

class Ast : public Nameable, public Locationable, private boost::noncopyable {
public:
  Ast(const std::string &name, const Location &location = Location());
  virtual ~Ast() = default;
  virtual AstCategory category() const = 0;
  virtual std::string toString() const;

  static bool isLiteral(Ast *e);
  static bool isId(Ast *e);
  static bool isExpr(Ast *e);
  static bool isDef(Ast *e);
  static bool isDecl(Ast *e);
  static bool isType(Ast *e);
};

// Ast }

// literal {

class A_Integer : public Ast {
public:
  // DecimalCategory:
  // DEC: base 10
  // HEX: base 16
  // BIN: base 2
  // OCT: base 8
  enum class DecimalCategory { DEC = 100, HEX, BIN, OCT };

  // BitCategory:
  // SIGNED: 32 bit signed
  // UNSIGNED: 32 bit unsigned
  // LONG: 64 bit signed long
  // ULONG: 64 bit unsigned long
  enum class BitCategory { SIGNED = 110, UNSIGNED, LONG, ULONG };

  A_Integer(const std::string &literal, const Location &location);
  virtual ~A_Integer() = default;
  virtual AstCategory category() const;

  virtual const std::string &literal() const;
  virtual int bits() const;
  virtual int base() const;
  virtual DecimalCategory decimalCategory() const;
  virtual BitCategory bitCategory() const;
  virtual int32_t asInt32() const;
  virtual uint32_t asUInt32() const;
  virtual int64_t asInt64() const;
  virtual uint64_t asUInt64() const;

private:
  std::string literal_;
  std::string parsed_;
  int bits_;
  int base_;
  DecimalCategory decimalCategory_;
  BitCategory bitCategory_;
};

class A_Float : public Ast {
public:
  // BitCategory:
  // FLT: 32 bit
  // DBL: 64 bit
  enum class BitCategory { FLT = 130, DBL };

  A_Float(const std::string &literal, const Location &location);
  virtual ~A_Float() = default;
  virtual AstCategory category() const;

  virtual const std::string &literal() const;
  virtual int bits() const;
  virtual BitCategory bitCategory() const;
  virtual float asFloat() const;
  virtual double asDouble() const;

private:
  std::string literal_;
  std::string parsed_;
  int bits_;
  BitCategory bitCategory_;
};

// string literal
class A_String : public Ast {
public:
  // QuoteCategory
  // Single: "
  // TRIPLE: """
  enum class QuoteCategory { SINGLE = 140, TRIPLE };

  A_String(const std::string &literal, const Location &location);
  virtual ~A_String() = default;
  virtual AstCategory category() const;

  virtual const std::string &literal() const;
  virtual QuoteCategory quoteCategory() const;
  virtual const std::string &asString() const;

private:
  std::string literal_;
  std::string parsed_;
  QuoteCategory quoteCategory_;
};

class A_Character : public Ast {
public:
  A_Character(const std::string &literal, const Location &location);
  virtual ~A_Character() = default;
  virtual AstCategory category() const;

  virtual const std::string &literal() const;
  virtual char asChar() const;

private:
  std::string literal_;
  char parsed_;
};

class A_Boolean : public Ast {
public:
  A_Boolean(const std::string &literal, const Location &location);
  virtual ~A_Boolean() = default;
  virtual AstCategory category() const;

  virtual const std::string &literal() const;
  virtual bool asBoolean() const;

private:
  std::string literal_;
  bool parsed_;
};

class A_Nil : public Ast {
public:
  A_Nil(const Location &location);
  virtual ~A_Nil() = default;
  virtual AstCategory category() const;
};

class A_Void : public Ast {
public:
  A_Void(const Location &location);
  virtual ~A_Void() = default;
  virtual AstCategory category() const;
};

// literal }

// id {

class A_VarId : public Ast {
public:
  A_VarId(const std::string &literal, const Location &location);
  virtual ~A_VarId() = default;
  virtual AstCategory category() const;

  virtual const std::string &literal() const;

private:
  std::string literal_;
};

// id }

// simple expression without block {

class A_Throw : public Ast {
public:
  A_Throw(Ast *a_expr, const Location &location);
  virtual ~A_Throw();
  virtual AstCategory category() const;
  Ast *expr;
};

class A_Return : public Ast {
public:
  A_Return(Ast *a_expr, const Location &location);
  virtual ~A_Return();
  virtual AstCategory category() const;
  Ast *expr;
};

class A_Break : public Ast {
public:
  A_Break(const Location &location);
  virtual ~A_Break() = default;
  virtual AstCategory category() const;
};

class A_Continue : public Ast {
public:
  A_Continue(const Location &location);
  virtual ~A_Continue() = default;
  virtual AstCategory category() const;
};

class A_Assign : public Ast {
public:
  A_Assign(Ast *a_assignee, int a_assignOp, Ast *a_assignor,
           const Location &location);
  virtual ~A_Assign();
  virtual AstCategory category() const;
  Ast *assignee; // left
  int assignOp;
  Ast *assignor; // right
};

class A_PostfixExpr : public Ast {
public:
  A_PostfixExpr(Ast *a_expr, int a_postfixOp, const Location &location);
  virtual ~A_PostfixExpr();
  virtual AstCategory category() const;
  Ast *expr;
  int postfixOp;
};

class A_InfixExpr : public Ast {
public:
  A_InfixExpr(Ast *a_left, int a_infixOp, Ast *a_right,
              const Location &location);
  virtual ~A_InfixExpr();
  virtual AstCategory category() const;
  Ast *left;
  int infixOp;
  Ast *right;
};

class A_PrefixExpr : public Ast {
public:
  A_PrefixExpr(int a_prefixOp, Ast *a_expr, const Location &location);
  virtual ~A_PrefixExpr();
  virtual AstCategory category() const;
  int prefixOp;
  Ast *expr;
};

class A_Call : public Ast {
public:
  A_Call(Ast *a_id, A_Exprs *a_args, const Location &location);
  virtual ~A_Call();
  virtual AstCategory category() const;
  Ast *id;
  A_Exprs *args;
};

class A_Exprs : public Ast {
public:
  A_Exprs(Ast *a_expr, A_Exprs *a_next, const Location &location);
  virtual ~A_Exprs();
  virtual AstCategory category() const;
  Ast *expr;
  A_Exprs *next;
};

// simple expression without block }

// statement like expression with block {

class A_If : public Ast {
public:
  A_If(Ast *a_condition, Ast *a_thenp, Ast *a_elsep, const Location &location);
  virtual ~A_If();
  virtual AstCategory category() const;
  Ast *condition;
  Ast *thenp;
  Ast *elsep;
};

// for and while
class A_Loop : public Ast {
public:
  A_Loop(Ast *a_condition, Ast *a_body, const Location &location);
  virtual ~A_Loop();
  virtual AstCategory category() const;
  Ast *condition;
  Ast *body;
};

class A_Yield : public Ast {
public:
  A_Yield(Ast *expr, const Location &location);
  virtual ~A_Yield();
  virtual AstCategory category() const;
  Ast *expr;
};

class A_LoopCondition : public Ast {
public:
  A_LoopCondition(Ast *a_init, Ast *a_condition, Ast *a_update,
                  const Location &location);
  virtual ~A_LoopCondition();
  virtual AstCategory category() const;
  Ast *init;
  Ast *condition;
  Ast *update;
};

class A_LoopEnumerator : public Ast {
public:
  A_LoopEnumerator(Ast *a_id, Ast *a_expr, const Location &location);
  virtual ~A_LoopEnumerator();
  virtual AstCategory category() const;
  Ast *id;
  Ast *expr;
};

class A_DoWhile : public Ast {
public:
  A_DoWhile(Ast *a_body, Ast *a_condition, const Location &location);
  virtual ~A_DoWhile();
  virtual AstCategory category() const;
  Ast *body;
  Ast *condition;
};

class A_Try : public Ast {
public:
  A_Try(Ast *a_tryp, Ast *a_catchp, Ast *a_finallyp, const Location &location);
  virtual ~A_Try();
  virtual AstCategory category() const;
  Ast *tryp;
  Ast *catchp;
  Ast *finallyp;
};

class A_Block : public Ast {
public:
  A_Block(A_BlockStats *a_blockStats, const Location &location);
  virtual ~A_Block();
  virtual AstCategory category() const;
  A_BlockStats *blockStats;
};

class A_BlockStats : public Ast {
public:
  A_BlockStats(Ast *a_blockStat, A_BlockStats *a_next,
               const Location &location);
  virtual ~A_BlockStats();
  virtual AstCategory category() const;
  Ast *blockStat;
  A_BlockStats *next;
};

// statement like expression with block }

// type {

class A_PlainType : public Ast {
public:
  A_PlainType(int a_token, const Location &location);
  virtual ~A_PlainType() = default;
  virtual AstCategory category() const;
  int token;
};

// type }

// definition and declaration {

class A_FuncDef : public Ast {
public:
  A_FuncDef(Ast *a_funcSign, Ast *a_resultType, Ast *a_body,
            const Location &location);
  virtual ~A_FuncDef();
  virtual AstCategory category() const;
  Ast *funcSign;
  Ast *resultType;
  Ast *body;
};

class A_FuncSign : public Ast {
public:
  A_FuncSign(Ast *a_id, A_Params *a_params, const Location &location);
  virtual ~A_FuncSign();
  virtual AstCategory category() const;
  Ast *id;
  A_Params *params;
};

class A_Params : public Ast {
public:
  A_Params(A_Param *a_param, A_Params *a_next, const Location &location);
  virtual ~A_Params();
  virtual AstCategory category() const;
  A_Param *param;
  A_Params *next;
};

class A_Param : public Ast {
public:
  A_Param(Ast *a_id, Ast *a_type, const Location &location);
  virtual ~A_Param();
  virtual AstCategory category() const;
  Ast *id;
  Ast *type;
};

class A_VarDef : public Ast {
public:
  A_VarDef(Ast *a_id, Ast *a_type, Ast *a_expr, const Location &location);
  virtual ~A_VarDef();
  virtual AstCategory category() const;
  Ast *id;
  Ast *type;
  Ast *expr;
};

// definition and declaration }

// compile unit {

class A_TopStats : public Ast {
public:
  A_TopStats(Ast *a_topStat, A_TopStats *a_next, const Location &location);
  virtual ~A_TopStats();
  virtual AstCategory category() const;
  Ast *topStat;
  A_TopStats *next;
};

class A_CompileUnit : public Ast {
public:
  A_CompileUnit(A_TopStats *a_topStats, const Location &location);
  virtual ~A_CompileUnit();
  virtual AstCategory category() const;
  A_TopStats *topStats;
};

// compile unit }
