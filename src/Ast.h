// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "boost/core/noncopyable.hpp"
#include "enum.h"
#include "iface/Identifiable.h"
#include "iface/LLVMTypable.h"
#include "iface/LLVMValuable.h"
#include "iface/Locationable.h"
#include "iface/Nameable.h"
#include "iface/Scoped.h"
#include "iface/Symbolizable.h"
#include "iface/TypeSymbolizable.h"
#include "infra/Cowstr.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <functional>
#include <sstream>

BETTER_ENUM(AstKind, int,
            // literal
            Integer = 1000, Float, Boolean, Character, String, Nil, Void,
            // id
            VarId,
            // expr without block
            Throw, Return, Break, Continue, Assign, Postfix, Prefix, Infix,
            Call, Exprs,
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
// class AstId;
class A_VarId;

/* expression without block */
class A_Throw;
class A_Return;
class A_Break;
class A_Continue;
class A_Assign;
class A_Postfix;
class A_Infix;
class A_Prefix;

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
// class AstType;
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

namespace detail {

class Parentable {
public:
  Parentable(Ast *parent = nullptr);
  virtual ~Parentable() = default;
  virtual Ast *&parent();
  virtual Ast *parent() const;

protected:
  Ast *parentable_;
};

} // namespace detail

// Ast {

class Ast : public Nameable,
            public Locationable,
            public Identifiable,
            public detail::Parentable,
            private boost::noncopyable {
public:
  Ast(const Cowstr &name, const Location &location = Location());
  virtual ~Ast() = default;
  virtual AstKind kind() const = 0;
  virtual Cowstr str() const;

  static bool isLiteral(Ast *e);
  static bool isId(Ast *e);
  static bool isExpr(Ast *e);
  static bool isDef(Ast *e);
  static bool isDecl(Ast *e);
  static bool isType(Ast *e);
};

// Ast }

// literal {

class A_Integer : public Ast, public LLVMValuable {
public:
  A_Integer(const Cowstr &literal, const Location &location);
  virtual ~A_Integer() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  // 32, 64
  virtual int bit() const;
  // signed or unsigned
  virtual bool isSigned() const;
  // 2, 8, 10, 16
  virtual int base() const;
  // integer value
  virtual int32_t asInt32() const;
  virtual uint32_t asUInt32() const;
  virtual int64_t asInt64() const;
  virtual uint64_t asUInt64() const;

private:
  Cowstr parsed_;
  int bit_;
  bool isSigned_;
  int base_;
};

class A_Float : public Ast, public LLVMValuable {
public:
  A_Float(const Cowstr &literal, const Location &location);
  virtual ~A_Float() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  // 32, 64
  virtual int bit() const;
  // floating point value
  virtual float asFloat() const;
  virtual double asDouble() const;

private:
  Cowstr parsed_;
  int bit_;
};

// string literal
class A_String : public Ast, public LLVMValuable {
public:
  A_String(const Cowstr &literal, const Location &location);
  virtual ~A_String() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  virtual bool isMultipleLine() const;
  virtual const Cowstr &asString() const;

private:
  Cowstr parsed_;
  bool isMultipleLine_;
};

class A_Character : public Ast, public LLVMValuable {
public:
  A_Character(const Cowstr &literal, const Location &location);
  virtual ~A_Character() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  virtual char asChar() const;

private:
  char parsed_;
};

class A_Boolean : public Ast, public LLVMValuable {
public:
  A_Boolean(const Cowstr &literal, const Location &location);
  virtual ~A_Boolean() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  virtual bool asBoolean() const;

private:
  bool parsed_;
};

class A_Nil : public Ast {
public:
  A_Nil(const Location &location);
  virtual ~A_Nil() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;
};

class A_Void : public Ast {
public:
  A_Void(const Location &location);
  virtual ~A_Void() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;
};

// literal }

// id {

// class AstId : public Ast {
// public:
//   AstId(const Cowstr &literal, const Location &location, Ast *parent =
//   nullptr); virtual ~AstId() = default;
// };

class A_VarId : public Ast, public Symbolizable, public TypeSymbolizable {
public:
  A_VarId(const Cowstr &literal, const Location &location);
  virtual ~A_VarId() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;
};

// id }

// simple expression without block {

class A_Throw : public Ast {
public:
  A_Throw(Ast *a_expr, const Location &location);
  virtual ~A_Throw();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *expr;
};

class A_Return : public Ast {
public:
  A_Return(Ast *a_expr, const Location &location);
  virtual ~A_Return();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *expr;
};

class A_Break : public Ast {
public:
  A_Break(const Location &location);
  virtual ~A_Break() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;
};

class A_Continue : public Ast {
public:
  A_Continue(const Location &location);
  virtual ~A_Continue() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;
};

class A_Assign : public Ast {
public:
  A_Assign(Ast *a_assignee, int a_assignOp, Ast *a_assignor,
           const Location &location);
  virtual ~A_Assign();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *assignee; // left
  int assignOp;
  Ast *assignor; // right
};

class A_Postfix : public Ast {
public:
  A_Postfix(Ast *a_expr, int a_postfixOp, const Location &location);
  virtual ~A_Postfix();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *expr;
  int postfixOp;
};

class A_Infix : public Ast {
public:
  A_Infix(Ast *a_left, int a_infixOp, Ast *a_right, const Location &location);
  virtual ~A_Infix();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *left;
  int infixOp;
  Ast *right;
};

class A_Prefix : public Ast {
public:
  A_Prefix(int a_prefixOp, Ast *a_expr, const Location &location);
  virtual ~A_Prefix();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  int prefixOp;
  Ast *expr;
};

class A_Call : public Ast {
public:
  A_Call(Ast *a_id, A_Exprs *a_args, const Location &location);
  virtual ~A_Call();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *id;
  A_Exprs *args;
};

class A_Exprs : public Ast {
public:
  A_Exprs(Ast *a_expr, A_Exprs *a_next, const Location &location);
  virtual ~A_Exprs();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *expr;
  A_Exprs *next;
};

// simple expression without block }

// statement like expression with block {

class A_If : public Ast {
public:
  A_If(Ast *a_condition, Ast *a_thenp, Ast *a_elsep, const Location &location);
  virtual ~A_If();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *condition;
  Ast *thenp;
  Ast *elsep;
};

// for and while
class A_Loop : public Ast, public Scoped {
public:
  A_Loop(Ast *a_condition, Ast *a_body, const Location &location);
  virtual ~A_Loop();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *condition;
  Ast *body;
};

class A_Yield : public Ast {
public:
  A_Yield(Ast *expr, const Location &location);
  virtual ~A_Yield();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *expr;
};

class A_LoopCondition : public Ast {
public:
  A_LoopCondition(Ast *a_init, Ast *a_condition, Ast *a_update,
                  const Location &location);
  virtual ~A_LoopCondition();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *init;
  Ast *condition;
  Ast *update;
};

class A_LoopEnumerator : public Ast {
public:
  A_LoopEnumerator(Ast *a_id, Ast *a_type, Ast *a_expr,
                   const Location &location);
  virtual ~A_LoopEnumerator();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *id;
  Ast *type;
  Ast *expr;
};

class A_DoWhile : public Ast {
public:
  A_DoWhile(Ast *a_body, Ast *a_condition, const Location &location);
  virtual ~A_DoWhile();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *body;
  Ast *condition;
};

class A_Try : public Ast {
public:
  A_Try(Ast *a_tryp, Ast *a_catchp, Ast *a_finallyp, const Location &location);
  virtual ~A_Try();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *tryp;
  Ast *catchp;
  Ast *finallyp;
};

class A_Block : public Ast, public Scoped {
public:
  A_Block(A_BlockStats *a_blockStats, const Location &location);
  virtual ~A_Block();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  A_BlockStats *blockStats;
};

class A_BlockStats : public Ast {
public:
  A_BlockStats(Ast *a_blockStat, A_BlockStats *a_next,
               const Location &location);
  virtual ~A_BlockStats();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *blockStat;
  A_BlockStats *next;
};

// statement like expression with block }

// type {

// class AstType : public Ast {
// public:
//   AstType(const Cowstr &literal, const Location &location,
//           Ast *parent = nullptr);
//   virtual ~AstType() = default;
// };

class A_PlainType : public Ast {
public:
  A_PlainType(int a_token, const Location &location);
  virtual ~A_PlainType() = default;
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  int token;
};

// type }

// definition and declaration {

class A_FuncDef : public Ast {
public:
  A_FuncDef(Ast *a_funcSign, Ast *a_resultType, Ast *a_body,
            const Location &location);
  virtual ~A_FuncDef();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *funcSign;
  Ast *resultType;
  Ast *body;
};

class A_FuncSign : public Ast {
public:
  A_FuncSign(Ast *a_id, A_Params *a_params, const Location &location);
  virtual ~A_FuncSign();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *id;
  A_Params *params;
};

class A_Params : public Ast {
public:
  A_Params(A_Param *a_param, A_Params *a_next, const Location &location);
  virtual ~A_Params();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  A_Param *param;
  A_Params *next;
};

class A_Param : public Ast {
public:
  A_Param(Ast *a_id, Ast *a_type, const Location &location);
  virtual ~A_Param();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *id;
  Ast *type;
};

class A_VarDef : public Ast {
public:
  A_VarDef(Ast *a_id, Ast *a_type, Ast *a_expr, const Location &location);
  virtual ~A_VarDef();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

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
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  Ast *topStat;
  A_TopStats *next;
};

class A_CompileUnit : public Ast, public Scoped {
public:
  A_CompileUnit(const Cowstr &name, A_TopStats *a_topStats,
                const Location &location);
  virtual ~A_CompileUnit();
  virtual AstKind kind() const;
  virtual Cowstr str() const;

  A_TopStats *topStats;
};

// compile unit }
