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
            Integer = 1000, FloatingPointLiteral, BooleanLiteral,
            CharacterLiteral, StringLiteral, NilLiteral, VoidLiteral,
            // id
            VarId,
            // expression
            ThrowExpr, ReturnExpr, AssignExpr, PostfixExpr, PrefixExpr,
            InfixExpr,
            /* SequelExpr, CallExpr, UnaryExpr, BinaryExpr, */
            /* ConditionalExpr, VoidExpr, */
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
class A_FloatingPointLiteral;
class A_BooleanLiteral;
class A_CharacterLiteral;
class A_StringLiteral;
class A_NilLiteral;
class A_VoidLiteral;

/* id */
class A_VarId;

/* expression */
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

/* expression */

/* statement */
class AstExprStatement;
class AstCompoundStatement;
class A_IfStatement;
class AstWhileStatement;
class AstForStatement;
class AstContinueStatement;
class AstBreakStatement;
class A_ReturnExpr;
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
  virtual void expand(const Position &position);

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

  virtual const std::string &parsed() const;
  virtual int bits() const;
  virtual int base() const;
  virtual DecimalCategory decimalCategory() const;
  virtual BitCategory bitCategory() const;
  virtual const std::string &literal() const;
  virtual int32_t toInt32() const;
  virtual uint32_t toUInt32() const;
  virtual int64_t toInt64() const;
  virtual uint64_t toUInt64() const;

private:
  std::string literal_;
  std::string parsed_;
  int bits_;
  int base_;
  DecimalCategory decimalCategory_;
  BitCategory bitCategory_;
};

class A_FloatingPointLiteral : public AstExpr {
public:
  // BitCategory:
  // FLT: 32 bit
  // DBL: 64 bit
  enum class BitCategory { FLT = 130, DBL };

  A_FloatingPointLiteral(const std::string &literal, const Position &position);
  virtual ~A_FloatingPointLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const std::string &parsed() const;
  virtual int bits() const;
  virtual BitCategory bitCategory() const;
  virtual const std::string &literal() const;
  virtual float toFloat() const;
  virtual double toDouble() const;

private:
  std::string literal_;
  std::string parsed_;
  int bits_;
  BitCategory bitCategory_;
};

// string literal
class A_StringLiteral : public AstExpr {
public:
  // QuoteCategory
  // Single: "
  // TRIPLE: """
  enum class QuoteCategory { SINGLE = 140, TRIPLE };

  A_StringLiteral(const std::string &literal, const Position &position);
  virtual ~A_StringLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual QuoteCategory quoteCategory() const;
  virtual const std::string &parsed() const;
  virtual const std::string &literal() const;

private:
  std::string literal_;
  std::string parsed_;
  QuoteCategory quoteCategory_;
};

class A_CharacterLiteral : public AstExpr {
  A_CharacterLiteral(const std::string &literal, const Position &position);
  virtual ~A_CharacterLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const std::string &literal() const;
  virtual char parsed() const;

private:
  std::string literal_;
  char parsed_;
};

class A_BooleanLiteral : public AstExpr {
public:
  A_BooleanLiteral(const std::string &literal, const Position &position);
  virtual ~A_BooleanLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const std::string &literal() const;
  virtual bool parsed() const;

private:
  std::string literal_;
  bool parsed_;
};

class A_NilLiteral : public AstExpr {
public:
  A_NilLiteral(const Position &position);
  virtual ~A_NilLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_VoidLiteral : public AstExpr {
public:
  A_VoidLiteral(const Position &position);
  virtual ~A_VoidLiteral() = default;
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

// expression {

class AstExpr : public Ast {
public:
  AstExpr(const std::string &name);
  AstExpr(const std::string &name, const Position &position);
  virtual ~AstExpr() = default;
};

class A_Throw : public Ast {
public:
  A_Throw(const AstExpr *expression, const Position &position);
  virtual ~A_Throw();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *expression() const;

private:
  const AstExpr *expression_;
};

class A_Return : public Ast {
public:
  A_Return(const AstExpr *expression, const Position &position);
  virtual ~A_Return();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *expression() const;

private:
  const AstExpr *expression_;
};

class A_AssignExpr : public AstExpr {
public:
  A_AssignExpr(const AstId *id, const AstId *equalOperator,
               const AstExpr *expression);
  virtual ~A_AssignExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstId *id() const;
  virtual const AstId *equalOperator() const;
  virtual const AstExpr *expression() const;

private:
  const AstId *id_;
  const AstId *equalOperator_;
  const AstExpr *expression_;
};

class A_PostfixExpr : public AstExpr {
public:
  A_PostfixExpr(const AstExpr *expression, const AstId *postfixOperator);
  virtual ~A_PostfixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpr *expression() const;
  virtual const AstId *postfixOperator() const;

private:
  const AstExpr *expression_;
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
  A_PrefixExpr(const AstId *prefixOperator, const AstExpr *expression);
  virtual ~A_PrefixExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstId *prefixOperator() const;
  virtual const AstExpr *expression() const;

private:
  const AstId *prefixOperator_;
  const AstExpr *expression_;
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

// expression }

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
  A_UnaryExpr(const AstToken *token, const AstExpr *expression);
  virtual ~A_UnaryExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstToken *token() const;
  virtual const AstExpr *expression() const;

private:
  const AstToken *token_;
  const AstExpr *expression_;
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

/* expression statement */
class AstExprStatement : public AstStatement {
public:
  AstExprStatement(AstExpr *expression, const Position &semiTokenPosition);
  virtual ~AstExprStatement();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstExpr *expression() const;

private:
  AstExpr *expression_;
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
  A_ReturnExpr(AstExpr *expression, const Position &returnTokenPosition,
               const Position &semiTokenPosition);
  virtual ~A_ReturnExpr();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual AstExpr *expression() const;

private:
  AstExpr *expression_;
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
                               AstExpr *expression,
                               const Position &identifierPosition,
                               const Position &typePosition);
  virtual ~AstVariableInitialDefinition();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const std::string &identifier() const;
  virtual AstId *typeId() const;
  virtual AstExpr *expression() const;

private:
  std::string identifier_;
  AstId *typeId_;
  AstExpr *expression_;
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
