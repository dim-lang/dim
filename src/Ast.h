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
            InfixExpr,
            // type
            FunctionType, PrimitiveType,
            // statement
            ExprStatement, IfStatement, WhileStatement, ForStatement,
            ContinueStatement, BreakStatement, EmptyStatement,
            CompoundStatement,
            // definition
            VariableDefinition, VariableInitialDefinition, FunctionDefinition,
            FunctionSignatureDefinition, FunctionArgumentDefinition,
            // list
            ExprList, StatementList, DefinitionList,
            // translate unit
            TranslateUnit)

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

/* expr */
class AstExpr;

class A_Throw;
class A_Return;
class A_CallExpr;
class A_UnaryExpr;
class A_BinaryExpr;
class A_ConditionalExpr;
class A_AssignmentExpr;
class A_VoidExpr;

/* statement */
class AstStatement;

class AstDefinition;

/* translate unit */
class AstTranslateUnit;

/* expr */

/* statement */
class AstExprStatement;
class AstCompoundStatement;
class A_IfStatement;
class AstWhileStatement;
class AstForStatement;
class AstContinueStatement;
class AstBreakStatement;
class AstEmptyStatement;

/* definition */
class AstVariableDefinition;
class AstVariableInitialDefinition;
class AstFunctionDefinition;
class AstFunctionSignatureDefinition;
class AstFunctionArgumentDefinition;

/* type */
class AstType;
class A_FunctionType;
class A_PrimitiveType;

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

// expr {

class AstExpr : public Ast {
public:
  AstExpr(const std::string &name);
  AstExpr(const std::string &name, const Position &position);
  virtual ~AstExpr() = default;
};

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
  int assignOp;
  const AstExpr *assignor_; // right
};

class A_PostfixExpr : public AstExpr {
public:
  A_PostfixExpr(const AstExpr *expr, const AstId *postfixOperator);
  virtual ~A_PostfixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *expr() const;
  virtual const AstId *postfixOperator() const;

private:
  const AstExpr *expr_;
  const AstId *postfixOperator_;
};

class A_InfixExpr : public AstExpr {
public:
  A_InfixExpr(const AstExpr *left, const AstId *infixOperator,
              const AstExpr *right);
  virtual ~A_InfixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *left() const;
  virtual const AstId *infixOperator() const;
  virtual const AstExpr *right() const;

private:
  const AstExpr *left_;
  const AstId *infixOperator_;
  const AstExpr *right_;
};

class A_PrefixExpr : public AstExpr {
public:
  A_PrefixExpr(const AstId *prefixOperator, const AstExpr *expr);
  virtual ~A_PrefixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstId *prefixOperator() const;
  virtual const AstExpr *expr() const;

private:
  const AstId *prefixOperator_;
  const AstExpr *expr_;
};

class A_IfThenExpr : public AstExpr {
public:
  A_IfThenExpr(const AstExpr *condition, const AstExpr *thens);
  virtual ~A_IfThenExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *condition() const;
  virtual const AstExpr *thens() const;

private:
  const AstExpr *condition_;
  const AstExpr *thens_;
};

class A_IfElseExpr : public AstExpr {
public:
  A_IfElseExpr(const AstExpr *condition, const AstExpr *thens,
               const AstExpr *elses);
  virtual ~A_IfElseExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *condition() const;
  virtual const AstExpr *thens() const;
  virtual const AstExpr *elses() const;

private:
  const AstExpr *condition_;
  const AstExpr *thens_;
  const AstExpr *elses_;
};

// expr }

// type {

// interface
class AstType : public Ast, public AstNamely, public AstPositional {
public:
  AstType(const std::string &name, const Position &position);
  virtual ~AstType() = default;
};

class A_FunctionType : public AstType {
public:
  A_FunctionType(const std::string &literal, const Position &position);
  virtual ~A_FunctionType() = default;
};

class A_PrimitiveType : public AstType {
public:
  virtual ~A_PrimitiveType() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const std::string &literal() const;

  // integer, float, boolean
  static const A_PrimitiveType *get(int token, const Position &position);

private:
  A_PrimitiveType(const std::string &literal, const Position &position);
  std::string literal_;
};

// type }

class A_CallExpr : public AstExpr {
public:
  A_CallExpr(const AstId *id, const A_ExprList *argumentList,
             const Position &idPosition);
  virtual ~A_CallExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstId *id() const;
  virtual const A_ExprList *argumentList() const;

private:
  const AstId *id_;
  const A_ExprList *argumentList_;
};

class A_UnaryExpr : public AstExpr {
public:
  A_UnaryExpr(const AstToken *token, const AstExpr *expr);
  virtual ~A_UnaryExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstToken *token() const;
  virtual const AstExpr *expr() const;

private:
  const AstToken *token_;
  const AstExpr *expr_;
};

class A_BinaryExpr : public AstExpr {
public:
  A_BinaryExpr(const AstExpr *left, const AstToken *token,
               const AstExpr *right);
  virtual ~A_BinaryExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstExpr *left() const;
  virtual const AstToken *token() const;
  virtual const AstExpr *right() const;

private:
  const AstExpr *left_;
  const AstToken *token_;
  const AstExpr *right_;
};

class A_ConditionalExpr : public AstExpr {
public:
  A_ConditionalExpr(const AstExpr *condition, const AstExpr *thens,
                    const AstExpr *elses);
  virtual ~A_ConditionalExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstExpr *condition() const;
  virtual const AstExpr *thens() const;
  virtual const AstExpr *elses() const;

private:
  const AstExpr *condition_;
  const AstExpr *thens_;
  const AstExpr *elses_;
};

class A_AssignmentExpr : public AstExpr {
public:
  A_AssignmentExpr(const AstExpr *left, int token, const AstExpr *right,
                   const Position &tokenPosition);
  virtual ~A_AssignmentExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstExpr *left() const;
  virtual int token() const;
  virtual const AstExpr *right() const;

private:
  const AstExpr *left_;
  int token_;
  const AstExpr *right_;
};

class A_VoidExpr : public AstExpr {
public:
  A_VoidExpr();
  virtual ~A_VoidExpr() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

// statement {

class AstStatement : public Ast, public AstNamely, public AstPositional {
public:
  AstStatement(const std::string &name);
  AstStatement(const std::string &name, const Position &position);
  virtual ~AstStatement() = default;
};

// definition {

class AstDefinition : public AstStatement {
public:
  AstDefinition(const std::string &name);
  AstDefinition(const std::string &name, const Position &position);
  virtual ~AstDefinition() = default;
  virtual AstCategory category() const = 0;
  virtual std::string toString() const = 0;
};

// definition }

// statement }

class AstExprList : public detail::AstList<AstExpr> {
public:
  AstExprList();
  virtual ~AstExprList() = default;
  virtual AstCategory category() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual bool empty() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

private:
  virtual std::string stringify() const;
};

class AstStatementList : public detail::AstList<AstStatement> {
public:
  AstStatementList();
  virtual ~AstStatementList() = default;
  virtual AstCategory category() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual bool empty() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

private:
  virtual std::string stringify() const;
};

class AstDefinitionList : public detail::AstList<AstDefinition> {
public:
  AstDefinitionList();
  virtual ~AstDefinitionList() = default;
  virtual AstCategory category() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual bool empty() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

private:
  virtual std::string stringify() const;
};

/* translate unit is actually definition list */

class AstTranslateUnit : public detail::AstList<AstDefinition> {
public:
  AstTranslateUnit();
  virtual ~AstTranslateUnit() = default;
  virtual AstCategory category() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual bool empty() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

private:
  virtual std::string stringify() const;
};

/* expr statement */
class AstExprStatement : public AstStatement {
public:
  AstExprStatement(AstExpr *expr, const Position &semiTokenPosition);
  virtual ~AstExprStatement();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstExpr *expr() const;

private:
  AstExpr *expr_;
};

/* compound statement - { ... } */
class AstCompoundStatement : public AstStatement {
public:
  AstCompoundStatement(AstStatementList *statementList,
                       const Position &lparenTokenPosition,
                       const Position &rparenTokenPosition);
  virtual ~AstCompoundStatement();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstStatementList *statementList() const;

private:
  AstStatementList *statementList_;
};

/* selection statement - if else */
class A_IfStatement : public AstStatement {
public:
  A_IfStatement(AstExpr *condition, AstStatement *thens, AstStatement *elses,
                const Position &ifTokenPosition);
  virtual ~A_IfStatement();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstExpr *condition() const;
  virtual AstStatement *thens() const;
  virtual AstStatement *elses() const;

private:
  AstExpr *condition_;
  AstStatement *thens_;
  AstStatement *elses_;
};

/* iteration statement - while */
class AstWhileStatement : public AstStatement {
public:
  AstWhileStatement(AstExpr *condition, AstStatement *statement,
                    const Position &whileTokenPosition);
  virtual ~AstWhileStatement();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstExpr *condition() const;
  virtual AstStatement *statement() const;

private:
  AstExpr *condition_;
  AstStatement *statement_;
};

/* iteration statement - for */
class AstForStatement : public AstStatement {
public:
  AstForStatement(AstStatement *initial, AstStatement *condition, AstExpr *post,
                  AstStatement *statement, const Position &forTokenPosition);
  virtual ~AstForStatement();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstStatement *start() const;
  virtual AstStatement *step() const;
  virtual AstExpr *end() const;
  virtual AstStatement *statement() const;

private:
  AstStatement *start_;
  AstStatement *step_;
  AstExpr *end_;
  AstStatement *statement_;
};

/* jump statement - continue */
class AstContinueStatement : public AstStatement {
public:
  AstContinueStatement(const Position &continueTokenPosition,
                       const Position &semiTokenPosition);
  virtual ~AstContinueStatement() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

private:
};

/* jump statement - break */
class AstBreakStatement : public AstStatement {
public:
  AstBreakStatement(const Position &breakTokenPosition,
                    const Position &semiTokenPosition);
  virtual ~AstBreakStatement() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

private:
};

/* jump statement - return */
class A_ReturnExpr : public AstStatement {
public:
  A_ReturnExpr(AstExpr *expr, const Position &returnTokenPosition,
               const Position &semiTokenPosition);
  virtual ~A_ReturnExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual AstExpr *expr() const;

private:
  AstExpr *expr_;
};

/* empty statement */
class AstEmptyStatement : public AstStatement {
public:
  AstEmptyStatement();
  AstEmptyStatement(const Position &semiTokenPosition);
  virtual ~AstEmptyStatement() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

/* variable definition */
class AstVariableDefinition : public AstDefinition {
public:
  AstVariableDefinition(AstDefinitionList *definitionList,
                        const Position &varTokenPosition,
                        const Position &semiTokenPosition);
  virtual ~AstVariableDefinition();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual AstDefinitionList *definitionList() const;

private:
  AstDefinitionList *definitionList_;
};

/* variable initial definition */
class AstVariableInitialDefinition : public AstDefinition {
public:
  AstVariableInitialDefinition(const char *identifier, AstId *typeId,
                               AstExpr *expr,
                               const Position &identifierPosition,
                               const Position &typePosition);
  virtual ~AstVariableInitialDefinition();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const std::string &identifier() const;
  virtual AstId *typeId() const;
  virtual AstExpr *expr() const;

private:
  std::string identifier_;
  AstId *typeId_;
  AstExpr *expr_;
};

/* function definition */
class AstFunctionDefinition : public AstDefinition {
public:
  AstFunctionDefinition(AstFunctionSignatureDefinition *signature,
                        AstStatement *statement);
  virtual ~AstFunctionDefinition();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstFunctionSignatureDefinition *signature() const;
  virtual AstStatement *statement() const;

private:
  AstFunctionSignatureDefinition *signature_;
  AstStatement *statement_;
};

/* function signature definition */
class AstFunctionSignatureDefinition : public AstDefinition {
public:
  AstFunctionSignatureDefinition(const char *identifier,
                                 AstDefinitionList *argumentList,
                                 AstExpr *result,
                                 const Position &funcTokenPosition,
                                 const Position &identifierPosition);
  virtual ~AstFunctionSignatureDefinition();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual AstDefinitionList *argumentList() const;
  virtual AstExpr *result() const;

private:
  std::string identifier_;
  AstDefinitionList *argumentList_;
  AstExpr *result_;
};

/* function argument definition */
class AstFunctionArgumentDefinition : public AstDefinition {
public:
  AstFunctionArgumentDefinition(const char *identifier,
                                const Position &argumentPosition);
  virtual ~AstFunctionArgumentDefinition() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;

private:
  std::string identifier_;
};
