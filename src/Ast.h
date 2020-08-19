// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Exception.h"
#include "Name.h"
#include "Position.h"
#include "boost/core/noncopyable.hpp"
#include "enum.h"
#include "interface/Namely.h"
#include "interface/Stringify.h"
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
            If, Loop, LoopCondition, Try, Block, BlockStats,
            // type
            PlainType,
            // declaration and definition
            FuncDef, FuncSign, Params, Param, VarDef,
            // compile unit
            CompileUnit, TopStats)

/*================ class ================*/

/* ast */
class AstNamely;
class AstPositional;
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

class AstNamely {
public:
  AstNamely(const std::string &name);
  virtual ~AstNamely() = default;
  virtual const Name &name() const;

protected:
  Name name_;
};

class AstPositional {
public:
  AstPositional();
  AstPositional(const Position &position);
  virtual ~AstPositional() = default;
  virtual const Position &position() const;
  virtual void locate(const Position &position);

protected:
  Position position_;
};

class Ast : public AstNamely, public AstPositional, private boost::noncopyable {
public:
  Ast(const std::string &name);
  Ast(const std::string &name, const Position &position);
  virtual ~Ast() = default;
  virtual AstCategory category() const = 0;
  virtual std::string toString() const = 0;

  static bool isLiteral(const Ast *e);
  static bool isId(const Ast *e);
  static bool isExpr(const Ast *e);
  static bool isExprWithBlock(const Ast *e);
  static bool isExprWithoutBlock(const Ast *e);
  static bool isDef(const Ast *e);
  static bool isDecl(const Ast *e);
  static bool isType(const Ast *e);
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

  A_Integer(const std::string &literal, const Position &position);
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

  A_Float(const std::string &literal, const Position &position);
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

  A_String(const std::string &literal, const Position &position);
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
  A_Character(const std::string &literal, const Position &position);
  virtual ~A_Character() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual char asChar() const;

private:
  char parsed_;
};

class A_Boolean : public Ast {
public:
  A_Boolean(const std::string &literal, const Position &position);
  virtual ~A_Boolean() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual bool asBool() const;

private:
  bool parsed_;
};

class A_Nil : public Ast {
public:
  A_Nil(const Position &position);
  virtual ~A_Nil() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_Void : public Ast {
public:
  A_Void(const Position &position);
  virtual ~A_Void() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

// literal }

// id {

class A_VarId : public Ast {
public:
  A_VarId(const std::string &literal, const Position &position);
  virtual ~A_VarId() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

// id }

// simple expression without block {

class A_Throw : public Ast {
public:
  A_Throw(const Ast *expr, const Position &position);
  virtual ~A_Throw();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *expr() const;

private:
  const Ast *expr_;
};

class A_Return : public Ast {
public:
  A_Return(const Ast *expr, const Position &position);
  virtual ~A_Return();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *expr() const;

private:
  const Ast *expr_;
};

class A_Break : public Ast {
public:
  A_Break(const Position &position);
  virtual ~A_Break() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_Continue : public Ast {
public:
  A_Continue(const Position &position);
  virtual ~A_Continue() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_Assign : public Ast {
public:
  A_Assign(const Ast *assignee, int assignOp, const Ast *assignor,
           const Position &position);
  virtual ~A_Assign();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *assignee() const;
  virtual int assignOp() const;
  virtual const Ast *assignor() const;

private:
  const Ast *assignee_; // left
  int assignOp_;
  const Ast *assignor_; // right
};

class A_PostfixExpr : public Ast {
public:
  A_PostfixExpr(const Ast *expr, int postfixOp, const Position &position);
  virtual ~A_PostfixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *expr() const;
  virtual int postfixOp() const;

private:
  const Ast *expr_;
  int postfixOp_;
};

class A_InfixExpr : public Ast {
public:
  A_InfixExpr(const Ast *left, int infixOp, const Ast *right,
              const Position &position);
  virtual ~A_InfixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *left() const;
  virtual int infixOp() const;
  virtual const Ast *right() const;

private:
  const Ast *left_;
  int infixOp_;
  const Ast *right_;
};

class A_PrefixExpr : public Ast {
public:
  A_PrefixExpr(int prefixOp, const Ast *expr, const Position &position);
  virtual ~A_PrefixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual int prefixOp() const;
  virtual const Ast *expr() const;

private:
  int prefixOp_;
  const Ast *expr_;
};

class A_Call : public Ast {
public:
  A_Call(const Ast *id, const A_Exprs *args);
  virtual ~A_Call();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *id() const;
  virtual const A_Exprs *args() const;

private:
  const Ast *id_;
  const A_Exprs *args_;
};

class A_Exprs : public Ast {
public:
  A_Exprs(const Ast *expr, const A_Exprs *next);
  A_Exprs(const Ast *expr, const A_Exprs *next, const Position &position);
  virtual ~A_Exprs();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *expr() const;
  virtual const A_Exprs *next() const;

private:
  const Ast *expr_;
  const A_Exprs *next_;
};

// simple expression without block }

// statement like expression with block {

class A_If : public Ast {
public:
  A_If(const Ast *condition, const Ast *thenp, const Ast *elsep);
  virtual ~A_If();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *condition() const;
  virtual const Ast *thenp() const;
  virtual const Ast *elsep() const;

private:
  const Ast *condition_;
  const Ast *thenp_;
  const Ast *elsep_;
};

// for and while
class A_Loop : public Ast {
public:
  A_Loop(const Ast *condition, const Ast *body);
  virtual ~A_Loop();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *condition() const;
  virtual const Ast *body() const;

private:
  const Ast *condition_;
  const Ast *body_;
};

class A_LoopCondition : public Ast {
public:
  A_LoopCondition(const Ast *init, const Ast *condition, const Ast *update);
  virtual ~A_LoopCondition() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

private:
  const Ast *init_;
  const Ast *condition_;
  const Ast *update_;
};

class A_LoopEnumerator : public Ast {
public:
  A_LoopEnumerator(const Ast *id, const Ast *expr);
  virtual ~A_LoopEnumerator() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

private:
  const Ast *id_;
  const Ast *expr_;
};

class A_Try : public Ast {
public:
  A_Try(const Ast *tryp, const Ast *catchp, const Ast *finallyp);
  virtual ~A_Try();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *tryp() const;
  virtual const Ast *catchp() const;
  virtual const Ast *finallyp() const;

private:
  const Ast *tryp_;
  const Ast *catchp_;
  const Ast *finallyp_;
};

class A_Block : public Ast {
public:
  A_Block(const A_BlockStats *blockStats, const Position &lbracePosition,
          const Position &rbracePosition);
  virtual ~A_Block();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const A_BlockStats *blockStats() const;

private:
  const A_BlockStats *blockStats_;
};

class A_BlockStats : public Ast {
public:
  A_BlockStats(const Ast *stat, const A_BlockStats *next);
  A_BlockStats(const Ast *stat, const A_BlockStats *next,
               const Position &position);
  virtual ~A_BlockStats();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *stat() const;
  virtual const A_BlockStats *next() const;

private:
  const Ast *stat_;
  const A_BlockStats *next_;
};

// statement like expression with block }

// type {

class A_PlainType : public Ast {
public:
  A_PlainType(int token, const Position &position);
  virtual ~A_PlainType() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual int token() const;

private:
  int token_;
};

// type }

// definition and declaration {

class A_FuncDef : public Ast {
public:
  A_FuncDef(const Ast *funcSign, const Ast *resultType, const Ast *body);
  virtual ~A_FuncDef();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *funcSign() const;
  virtual const Ast *resultType() const;
  virtual const Ast *body() const;

private:
  const Ast *funcSign_;
  const Ast *resultType_;
  const Ast *body_;
};

class A_FuncSign : public Ast {
public:
  A_FuncSign(const Ast *id, const A_Params *params,
             const Position &lparenPosition, const Position &rparenPosition);
  virtual ~A_FuncSign();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *id() const;
  virtual const A_Params *params() const;

private:
  const Ast *id_;
  const A_Params *params_;
};

class A_Params : public Ast {
public:
  A_Params(const A_Param *param, const A_Params *next);
  A_Params(const A_Param *param, const A_Params *next,
           const Position &position);
  virtual ~A_Params();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const A_Param *param() const;
  virtual const A_Params *next() const;

private:
  const A_Param *param_;
  const A_Params *next_;
};

class A_Param : public Ast {
public:
  A_Param(const Ast *id, const Ast *type);
  virtual ~A_Param();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *id() const;
  virtual const Ast *type() const;

private:
  const Ast *id_;
  const Ast *type_;
};

class A_VarDef : public Ast {
public:
  A_VarDef(const Ast *id, const Ast *type, const Ast *expr);
  virtual ~A_VarDef();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *id() const;
  virtual const Ast *type() const;
  virtual const Ast *expr() const;

public:
  const Ast *id_;
  const Ast *type_;
  const Ast *expr_;
};

// definition and declaration }

// compile unit {

class A_TopStats : public Ast {
public:
  A_TopStats(const Ast *topStat, const A_TopStats *next);
  A_TopStats(const Ast *topStat, const A_TopStats *next,
             const Position &position);
  virtual ~A_TopStats();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *topStat() const;
  virtual const A_TopStats *next() const;

private:
  const Ast *topStat_;
  const A_TopStats *next_;
};

class A_CompileUnit : public Ast {
public:
  A_CompileUnit(const A_TopStats *topStats);
  virtual ~A_CompileUnit();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const A_TopStats *topStats() const;

private:
  const A_TopStats *topStats_;
};

// compile unit }
