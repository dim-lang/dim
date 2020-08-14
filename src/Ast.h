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
            // expr
            Throw, Return, Break, Continue, Assign, PostfixExpr, PrefixExpr,
            InfixExpr, Call, Exprs, If, Loop, LoopCondition, Block, BlockStats,
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
class AstExpr;
class AstDecl;
class AstDef;
class AstType;

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
class A_Call;
class A_Exprs;

/* expression with block */
class A_If;
class A_Loop;
class A_LoopCondition;
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
};

class AstExpr : public Ast {
public:
  AstExpr(const std::string &name);
  AstExpr(const std::string &name, const Position &position);
  virtual ~AstExpr() = default;
};

class AstDecl : public Ast {
public:
  AstDecl(const std::string &name);
  AstDecl(const std::string &name, const Position &position);
  virtual ~AstDecl() = default;
};

class AstDef : public AstDecl {
public:
  AstDef(const std::string &name);
  AstDef(const std::string &name, const Position &position);
  virtual ~AstDef() = default;
};

class AstType : public Ast {
public:
  AstType(const std::string &name);
  AstType(const std::string &name, const Position &position);
  virtual ~AstType() = default;
};

// Ast }

// literal {

class A_Integer : public AstExpr {
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

class A_Float : public AstExpr {
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
class A_String : public AstExpr {
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

class A_Character : public AstExpr {
  A_Character(const std::string &literal, const Position &position);
  virtual ~A_Character() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual char asChar() const;

private:
  char parsed_;
};

class A_Boolean : public AstExpr {
public:
  A_Boolean(const std::string &literal, const Position &position);
  virtual ~A_Boolean() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual bool asBool() const;

private:
  bool parsed_;
};

class A_Nil : public AstExpr {
public:
  A_Nil(const Position &position);
  virtual ~A_Nil() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_Void : public AstExpr {
public:
  A_Void(const Position &position);
  virtual ~A_Void() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

// literal }

// id {

class A_VarId : public AstExpr {
public:
  A_VarId(const std::string &literal, const Position &position);
  virtual ~A_VarId() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

// id }

// simple expression without block {

class A_Throw : public AstExpr {
public:
  A_Throw(const AstExpr *expr, const Position &position);
  virtual ~A_Throw();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *expr() const;

private:
  const AstExpr *expr_;
};

class A_Return : public AstExpr {
public:
  A_Return(const AstExpr *expr, const Position &position);
  virtual ~A_Return();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *expr() const;

private:
  const AstExpr *expr_;
};

class A_Break : public AstExpr {
public:
  A_Break(const Position &position);
  virtual ~A_Break() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_Continue : public AstExpr {
public:
  A_Continue(const Position &position);
  virtual ~A_Continue() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_Assign : public AstExpr {
public:
  A_Assign(const AstExpr *assignee, int assignOp, const AstExpr *assignor,
           const Position &position);
  virtual ~A_Assign();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *assignee() const;
  virtual int assignOp() const;
  virtual const AstExpr *assignor() const;

private:
  const AstExpr *assignee_; // left
  int assignOp_;
  const AstExpr *assignor_; // right
};

class A_PostfixExpr : public AstExpr {
public:
  A_PostfixExpr(const AstExpr *expr, int postfixOp, const Position &position);
  virtual ~A_PostfixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *expr() const;
  virtual int postfixOp() const;

private:
  const AstExpr *expr_;
  int postfixOp_;
};

class A_InfixExpr : public AstExpr {
public:
  A_InfixExpr(const AstExpr *left, int infixOp, const AstExpr *right,
              const Position &position);
  virtual ~A_InfixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *left() const;
  virtual int infixOp() const;
  virtual const AstExpr *right() const;

private:
  const AstExpr *left_;
  int infixOp_;
  const AstExpr *right_;
};

class A_PrefixExpr : public AstExpr {
public:
  A_PrefixExpr(int prefixOp, const AstExpr *expr, const Position &position);
  virtual ~A_PrefixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual int prefixOp() const;
  virtual const AstExpr *expr() const;

private:
  int prefixOp_;
  const AstExpr *expr_;
};

class A_Call : public AstExpr {
public:
  A_Call(const AstExpr *id, const A_Exprs *args);
  virtual ~A_Call();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *id() const;
  virtual const A_Exprs *args() const;

private:
  const AstExpr *id_;
  const A_Exprs *args_;
};

class A_Exprs : public AstExpr {
public:
  A_Exprs(const AstExpr *expr, const A_Exprs *next);
  A_Exprs(const AstExpr *expr, const A_Exprs *next, const Position &position);
  virtual ~A_Exprs();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *expr() const;
  virtual const A_Exprs *next() const;

private:
  const AstExpr *expr_;
  const A_Exprs *next_;
};

// simple expression without block }

// statement like expression with block {

class A_If : public AstExpr {
public:
  A_If(const AstExpr *condition, const AstExpr *thenp, const AstExpr *elsep);
  virtual ~A_If();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *condition() const;
  virtual const AstExpr *thenp() const;
  virtual const AstExpr *elsep() const;

private:
  const AstExpr *condition_;
  const AstExpr *thenp_;
  const AstExpr *elsep_;
};

// for and while
class A_Loop : public AstExpr {
public:
  A_Loop(const AstExpr *condition, const AstExpr *body);
  virtual ~A_Loop();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const Ast *condition() const;
  virtual const AstExpr *body() const;

private:
  const AstExpr *condition_;
  const AstExpr *body_;
};

class A_LoopCondition : public Ast {
public:
  A_LoopCondition();
  virtual ~A_LoopCondition() = default;
};

class A_Block : public AstExpr {
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

class A_PlainType : public AstType {
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

class A_FuncDef : public AstDef {
public:
  A_FuncDef(const AstDecl *funcSign, const AstType *resultType,
            const AstExpr *block);
  virtual ~A_FuncDef();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstDecl *funcSign() const;
  virtual const AstType *resultType() const;
  virtual const AstExpr *body() const;

private:
  const AstDecl *funcSign_;
  const AstType *resultType_;
  const AstExpr *block_;
};

class A_FuncSign : public AstDecl {
public:
  A_FuncSign(const AstExpr *id, const A_Params *params,
             const Position &lparenPosition, const Position &rparenPosition);
  virtual ~A_FuncSign();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *id() const;
  virtual const A_Params *params() const;

private:
  const AstExpr *id_;
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
  A_Param(const AstExpr *id, const AstType *type);
  virtual ~A_Param();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *id() const;
  virtual const AstType *type() const;

private:
  const AstExpr *id_;
  const AstType *type_;
};

class A_VarDef : public AstDef {
public:
  A_VarDef(const AstExpr *id, const AstType *type, const AstExpr *expr);
  virtual ~A_VarDef();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *id() const;
  virtual const AstType *type() const;
  virtual const AstExpr *expr() const;

public:
  const AstExpr *id_;
  const AstType *type_;
  const AstExpr *expr_;
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
