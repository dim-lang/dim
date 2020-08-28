// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Locationly.h"
#include "Namely.h"
#include "boost/core/noncopyable.hpp"
#include "enum.h"
#include "location.hh"
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
            If, Loop, LoopCondition, LoopEnumerator, Try, Block, BlockStats,
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
class A_If;
class A_Loop;
class A_LoopCondition;
class A_LoopEnumerator;
class A_Try;
class A_Call;
class A_Exprs;
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
class A_CompileUnit;
class A_TopStats;

// Ast {

class Ast : public Namely, public Locationly, private boost::noncopyable {
public:
  Ast(const std::string &name, const yy::location &location = yy::location());
  virtual ~Ast() = default;
  virtual AstCategory category() const = 0;
  virtual std::string toString() const = 0;

  static bool isLiteral(std::shared_ptr<Ast> e);
  static bool isId(std::shared_ptr<Ast> e);
  static bool isExpr(std::shared_ptr<Ast> e);
  static bool isDef(std::shared_ptr<Ast> e);
  static bool isDecl(std::shared_ptr<Ast> e);
  static bool isType(std::shared_ptr<Ast> e);
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

  A_Integer(const std::string &literal, const yy::location &location);
  virtual ~A_Integer() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual int bits() const;
  virtual int base() const;
  virtual DecimalCategory decimalCategory() const;
  virtual BitCategory bitCategory() const;
  virtual int32_t asInt32() const;
  virtual uint32_t asUInt32() const;
  virtual int64_t asInt64() const;
  virtual uint64_t asUInt64() const;

private:
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

  A_Float(const std::string &literal, const yy::location &location);
  virtual ~A_Float() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual int bits() const;
  virtual BitCategory bitCategory() const;
  virtual float asFloat() const;
  virtual double asDouble() const;

private:
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

  A_String(const std::string &literal, const yy::location &location);
  virtual ~A_String() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual QuoteCategory quoteCategory() const;
  virtual const std::string &asString() const;

private:
  std::string parsed_;
  QuoteCategory quoteCategory_;
};

class A_Character : public Ast {
public:
  A_Character(const std::string &literal, const yy::location &location);
  virtual ~A_Character() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual char asChar() const;

private:
  char parsed_;
};

class A_Boolean : public Ast {
public:
  A_Boolean(bool a_value, const yy::location &location);
  virtual ~A_Boolean() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  bool value;
};

class A_Nil : public Ast {
public:
  A_Nil(const yy::location &location);
  virtual ~A_Nil() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_Void : public Ast {
public:
  A_Void(const yy::location &location);
  virtual ~A_Void() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

// literal }

// id {

class A_VarId : public Ast {
public:
  A_VarId(const std::string &literal, const yy::location &location);
  virtual ~A_VarId() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

// id }

// simple expression without block {

class A_Throw : public Ast {
public:
  A_Throw(std::shared_ptr<Ast> a_expr, const yy::location &location);
  virtual ~A_Throw() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> expr;
};

class A_Return : public Ast {
public:
  A_Return(std::shared_ptr<Ast> a_expr, const yy::location &location);
  virtual ~A_Return() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> expr;
};

class A_Break : public Ast {
public:
  A_Break(const yy::location &location);
  virtual ~A_Break() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_Continue : public Ast {
public:
  A_Continue(const yy::location &location);
  virtual ~A_Continue() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_Assign : public Ast {
public:
  A_Assign(std::shared_ptr<Ast> a_assignee, int a_assignOp,
           std::shared_ptr<Ast> a_assignor, const yy::location &location);
  virtual ~A_Assign() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> assignee; // left
  int assignOp;
  std::shared_ptr<Ast> assignor; // right
};

class A_PostfixExpr : public Ast {
public:
  A_PostfixExpr(std::shared_ptr<Ast> a_expr, int a_postfixOp,
                const yy::location &location);
  virtual ~A_PostfixExpr() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> expr;
  int postfixOp;
};

class A_InfixExpr : public Ast {
public:
  A_InfixExpr(std::shared_ptr<Ast> a_left, int a_infixOp,
              std::shared_ptr<Ast> a_right, const yy::location &location);
  virtual ~A_InfixExpr() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> left;
  int infixOp;
  std::shared_ptr<Ast> right;
};

class A_PrefixExpr : public Ast {
public:
  A_PrefixExpr(int a_prefixOp, std::shared_ptr<Ast> a_expr,
               const yy::location &location);
  virtual ~A_PrefixExpr() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  int prefixOp;
  std::shared_ptr<Ast> expr;
};

class A_Call : public Ast {
public:
  A_Call(std::shared_ptr<Ast> a_id, std::shared_ptr<A_Exprs> a_args,
         const yy::location &location);
  virtual ~A_Call() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> id;
  std::shared_ptr<A_Exprs> args;
};

class A_Exprs : public Ast {
public:
  A_Exprs(std::shared_ptr<Ast> a_expr, std::shared_ptr<A_Exprs> a_next,
          const yy::location &location);
  virtual ~A_Exprs() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> expr;
  std::shared_ptr<A_Exprs> next;
};

// simple expression without block }

// statement like expression with block {

class A_If : public Ast {
public:
  A_If(std::shared_ptr<Ast> a_condition, std::shared_ptr<Ast> a_thenp,
       std::shared_ptr<Ast> a_elsep, const yy::location &location);
  virtual ~A_If() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> condition;
  std::shared_ptr<Ast> thenp;
  std::shared_ptr<Ast> elsep;
};

// for and while
class A_Loop : public Ast {
public:
  A_Loop(std::shared_ptr<Ast> a_condition, std::shared_ptr<Ast> a_body,
         const yy::location &location);
  virtual ~A_Loop() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> condition;
  std::shared_ptr<Ast> body;
};

class A_LoopCondition : public Ast {
public:
  A_LoopCondition(std::shared_ptr<Ast> a_init, std::shared_ptr<Ast> a_condition,
                  std::shared_ptr<Ast> a_update, bool a_do_once,
                  const yy::location &location);
  virtual ~A_LoopCondition() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> init;
  std::shared_ptr<Ast> condition;
  std::shared_ptr<Ast> update;
  bool do_once;
};

class A_LoopEnumerator : public Ast {
public:
  A_LoopEnumerator(std::shared_ptr<Ast> a_id, std::shared_ptr<Ast> a_expr,
                   const yy::location &location);
  virtual ~A_LoopEnumerator() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> id;
  std::shared_ptr<Ast> expr;
};

class A_Try : public Ast {
public:
  A_Try(std::shared_ptr<Ast> a_tryp, std::shared_ptr<Ast> a_catchp,
        std::shared_ptr<Ast> a_finallyp, const yy::location &location);
  virtual ~A_Try() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> tryp;
  std::shared_ptr<Ast> catchp;
  std::shared_ptr<Ast> finallyp;
};

class A_Block : public Ast {
public:
  A_Block(std::shared_ptr<Ast> blockStat,
          std::shared_ptr<A_BlockStats> a_blockStats,
          const yy::location &location);
  virtual ~A_Block() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> blockStat;
  std::shared_ptr<A_BlockStats> blockStats;
};

class A_BlockStats : public Ast {
public:
  A_BlockStats(std::shared_ptr<Ast> a_blockStat,
               std::shared_ptr<A_BlockStats> a_next,
               const yy::location &location);
  virtual ~A_BlockStats() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> blockStat;
  std::shared_ptr<A_BlockStats> next;
};

// statement like expression with block }

// type {

class A_PlainType : public Ast {
public:
  A_PlainType(int a_token, const yy::location &location);
  virtual ~A_PlainType() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  int token;
};

// type }

// definition and declaration {

class A_FuncDef : public Ast {
public:
  A_FuncDef(std::shared_ptr<Ast> a_funcSign, std::shared_ptr<Ast> a_resultType,
            std::shared_ptr<Ast> a_body, const yy::location &location);
  virtual ~A_FuncDef() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> funcSign;
  std::shared_ptr<Ast> resultType;
  std::shared_ptr<Ast> body;
};

class A_FuncSign : public Ast {
public:
  A_FuncSign(std::shared_ptr<Ast> a_id, std::shared_ptr<A_Params> a_params,
             const yy::location &location);
  virtual ~A_FuncSign() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> id;
  std::shared_ptr<A_Params> params;
};

class A_Params : public Ast {
public:
  A_Params(std::shared_ptr<A_Param> a_param, std::shared_ptr<A_Params> a_next,
           const yy::location &location);
  virtual ~A_Params() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<A_Param> param;
  std::shared_ptr<A_Params> next;
};

class A_Param : public Ast {
public:
  A_Param(std::shared_ptr<Ast> a_id, std::shared_ptr<Ast> a_type,
          const yy::location &location);
  virtual ~A_Param() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> id;
  std::shared_ptr<Ast> type;
};

class A_VarDef : public Ast {
public:
  A_VarDef(std::shared_ptr<Ast> a_id, std::shared_ptr<Ast> a_type,
           std::shared_ptr<Ast> a_expr, const yy::location &location);
  virtual ~A_VarDef() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> id;
  std::shared_ptr<Ast> type;
  std::shared_ptr<Ast> expr;
};

// definition and declaration }

// compile unit {

class A_TopStats : public Ast {
public:
  A_TopStats(std::shared_ptr<Ast> a_topStat, std::shared_ptr<A_TopStats> a_next,
             const yy::location &location);
  virtual ~A_TopStats() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> topStat;
  std::shared_ptr<A_TopStats> next;
};

class A_CompileUnit : public Ast {
public:
  A_CompileUnit(std::shared_ptr<Ast> a_topStat,
                std::shared_ptr<A_TopStats> a_topStats,
                const yy::location &location);
  virtual ~A_CompileUnit();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  std::shared_ptr<Ast> topStat;
  std::shared_ptr<A_TopStats> topStats;
};

// compile unit }
