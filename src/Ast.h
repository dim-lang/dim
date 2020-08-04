// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Exception.h"
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
            IntegerLiteral = 1000, FloatingPointLiteral, BooleanLiteral,
            CharacterLiteral, StringLiteral, NilLiteral, VoidLiteral,
            // token
            Token, RepetibleToken,
            // id
            Id,
            // expression
            ThrowExpression, ReturnExpression, AssignExpression,
            SequelExpression, CallExpression, UnaryExpression, BinaryExpression,
            ConditionalExpression, VoidExpression,
            // type
            FunctionType, PrimitiveType,
            // statement
            ExpressionStatement, IfStatement, WhileStatement, ForStatement,
            ContinueStatement, BreakStatement, EmptyStatement,
            CompoundStatement,
            // definition
            VariableDefinition, VariableInitialDefinition, FunctionDefinition,
            FunctionSignatureDefinition, FunctionArgumentDefinition,
            // list
            ExpressionList, StatementList, DefinitionList,
            // translate unit
            TranslateUnit)

/*================ class ================*/

/* ast */
class Ast;
class AstNamely;
class AstPositional;
class A_ExpressionList;

/* literal */
class AstLiteral;
class A_IntegerLiteral;
class A_FloatingPointLiteral;
class A_BooleanLiteral;
class A_CharacterLiteral;
class A_StringLiteral;
class A_NilLiteral;
class A_VoidLiteral;

/* token */
class AstToken;
class A_Token;

/* id */
class AstId;
class A_Id;

/* type */
class AstType;
class A_FunctionType;
class A_PrimitiveType;

/* expression */
class AstExpression;

class A_CallExpression;
class A_UnaryExpression;
class A_BinaryExpression;
class A_ConditionalExpression;
class A_AssignmentExpression;
class A_VoidExpression;

/* statement */
class AstStatement;

class AstDefinition;

/* translate unit */
class AstTranslateUnit;

/* expression */

/* statement */
class AstExpressionStatement;
class AstCompoundStatement;
class A_IfStatement;
class AstWhileStatement;
class AstForStatement;
class AstContinueStatement;
class AstBreakStatement;
class A_ReturnExpression;
class AstEmptyStatement;

/* definition */
class AstVariableDefinition;
class AstVariableInitialDefinition;
class AstFunctionDefinition;
class AstFunctionSignatureDefinition;
class AstFunctionArgumentDefinition;

/*================ definition ================*/

class AstNamely {
public:
  AstNamely(const std::string &name);
  virtual ~AstNamely() = default;
  virtual const std::string &name() const;

protected:
  std::string namely_;
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

// Ast {

class Ast : private boost::noncopyable {
public:
  virtual ~Ast() = default;
  virtual AstCategory category() const = 0;
  virtual std::string toString() const = 0;
};

class AstExpression : public Ast, public AstNamely, public AstPositional {
public:
  AstExpression(const std::string &name);
  AstExpression(const std::string &name, const Position &position);
  virtual ~AstExpression() = default;
};

// Ast }

// A_List {

namespace detail {

template <class T> class A_List : public AstPositional {
public:
  A_List() {}
  virtual ~A_List() {
    for (int i = 0; i < (int)items_.size(); i++) {
      EX_ASSERT(items_[i], "items_[{}] must not null", i);
      delete items_[i];
      items_[i] = nullptr;
    }
    items_.clear();
  }
  virtual std::string toString() const {
    std::stringstream ss;
    ss << fmt::format("size:{}", items_.size());
    if (items_.empty()) {
      return ss.str();
    }
    ss << ", ";
    for (int i = 0; i < (int)items_.size(); i++) {
      Ast *node = dynamic_cast<Ast *>(items_[i]);
      EX_ASSERT(node, "node must not null");
      ss << fmt::format("{}:{}", i, node->toString());
      if (i < (int)items_.size() - 1) {
        ss << ", ";
      }
    }
    return ss.str();
  }
  virtual bool empty() const { return items_.empty(); }
  virtual int size() const { return items_.size(); }
  virtual const T *get(int pos) const {
    EX_ASSERT(pos >= 0, "pos {} >= 0", pos);
    EX_ASSERT(items_[pos], "items_[{}] must not null", pos);
    return items_[pos];
  }
  virtual void add(const T *item) {
    EX_ASSERT(item, "item must not null");
    locate(dynamic_cast<const AstPositional *>(item)->position());
    items_.push_front(item);
  }
  virtual const Position &position() const { return position_; }

protected:
  std::deque<const T *> items_;
};

} // namespace detail

class A_ExpressionList : public detail::A_List<AstExpression>,
                         public Ast,
                         public AstNamely {
public:
  A_ExpressionList();
  virtual ~A_ExpressionList() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

// A_List }

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

// literal {

class AstLiteral : public AstExpression {
public:
  AstLiteral(const std::string &literal);
  AstLiteral(const std::string &literal, const Position &position);
  virtual ~AstLiteral() = default;
};

class A_IntegerLiteral : public AstLiteral {
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

  A_IntegerLiteral(const std::string &literal, const Position &position);
  virtual ~A_IntegerLiteral() = default;
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

class A_FloatingPointLiteral : public AstLiteral {
public:
  // DecimalCategory:
  // DEC: base 10
  // HEX: base 16
  enum class DecimalCategory { DEC = 120, HEX };

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
  virtual int base() const;
  virtual DecimalCategory decimalCategory() const;
  virtual BitCategory bitCategory() const;
  virtual const std::string &literal() const;
  virtual float toFloat() const;
  virtual double toDouble() const;

private:
  std::string literal_;
  std::string parsed_;
  int bits_;
  int base_;
  DecimalCategory decimalCategory_;
  BitCategory bitCategory_;
};

// string literal
class A_StringLiteral : public AstLiteral {
public:
  // QuoteCategory
  // Single: "
  // Multi: """
  enum class QuoteCategory { SINGLE = 140, MULTI };

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

class A_CharacterLiteral : public AstLiteral {
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

class A_BooleanLiteral : public AstLiteral {
public:
  A_BooleanLiteral(const std::string &literal, const Position &position);
  virtual ~A_BooleanLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual bool parsed() const;
  virtual const std::string &literal() const;

private:
  std::string literal_;
  bool parsed_;
};

class A_NilLiteral : public AstLiteral {
public:
  A_NilLiteral(const Position &position);
  virtual ~A_NilLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

class A_VoidLiteral : public AstLiteral {
public:
  A_VoidLiteral(const Position &position);
  virtual ~A_VoidLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

// literal }

// token {

class AstToken : public Ast, public AstNamely, public AstPositional {
public:
  AstToken(const std::string &name, const Position &position);
  virtual ~AstToken() = default;
};

class A_Token : public AstToken {
public:
  A_Token(const int &token, const Position &position);
  virtual ~A_Token() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual int token() const;

private:
  int token_;
};

class A_RepetibleToken : public AstToken {
public:
  A_RepetibleToken(const int &token, const Position &position);
  virtual ~A_RepetibleToken() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const std::deque<int> &tokens() const;
  virtual void join(const int &token, const Position &position);

private:
  std::deque<int> tokens_;
};

// token }

// id {

class AstId : public Ast, public AstNamely, public AstPositional {
public:
  // IdCategory
  enum class IdCategory { VAR = 200, OP, PREFIX, POSTFIX };

  AstId(const std::string &name, const Position &position);
  virtual ~AstId() = default;
};

class A_Id : public AstId {
public:
  A_Id(AstId::IdCategory idCategory, const std::string &literal,
       const Position &position);
  A_Id(AstId::IdCategory idCategory, const int &token,
       const Position &position);
  A_Id(AstId::IdCategory idCategory, const int &token, const Position &position,
       const Position &extra);
  virtual ~A_Id() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual AstId::IdCategory idCategory() const;
  virtual const std::string &literal() const;
  virtual int token() const;

private:
  AstId::IdCategory idCategory_;
  std::string literal_;
  int token_;
};

// id }

// expression {

class A_ThrowExpression : public AstExpression {
public:
  A_ThrowExpression(const AstExpression *expression, const Position &extra);
  virtual ~A_ThrowExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpression *expression() const;

private:
  const AstExpression *expression_;
};

class A_ReturnExpression : public AstExpression {
public:
  A_ReturnExpression(const AstExpression *expression, const Position &extra);
  virtual ~A_ReturnExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstExpression *expression() const;

private:
  const AstExpression *expression_;
};

class A_AssignExpression : public AstExpression {
public:
  A_AssignExpression(const AstId *id, const AstExpression *expression,
                     const int &token, const Position &position);
  virtual ~A_AssignExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const AstId *id() const;
  virtual const AstExpression *expression() const;
  virtual int token() const;

private:
  const AstId *id_;
  const AstExpression *expression_;
  int token_;
};

class A_CallExpression : public AstExpression {
public:
  A_CallExpression(const AstId *id, const A_ExpressionList *argumentList,
                   const Position &idPosition);
  virtual ~A_CallExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstId *id() const;
  virtual const A_ExpressionList *argumentList() const;

private:
  const AstId *id_;
  const A_ExpressionList *argumentList_;
};

class A_UnaryExpression : public AstExpression {
public:
  A_UnaryExpression(const AstToken *token, const AstExpression *expression);
  virtual ~A_UnaryExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstToken *token() const;
  virtual const AstExpression *expression() const;

private:
  const AstToken *token_;
  const AstExpression *expression_;
};

class A_BinaryExpression : public AstExpression {
public:
  A_BinaryExpression(const AstExpression *left, const AstToken *token,
                     const AstExpression *right);
  virtual ~A_BinaryExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstExpression *left() const;
  virtual const AstToken *token() const;
  virtual const AstExpression *right() const;

private:
  const AstExpression *left_;
  const AstToken *token_;
  const AstExpression *right_;
};

class A_ConditionalExpression : public AstExpression {
public:
  A_ConditionalExpression(const AstExpression *condition,
                          const AstExpression *thens,
                          const AstExpression *elses);
  virtual ~A_ConditionalExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstExpression *condition() const;
  virtual const AstExpression *thens() const;
  virtual const AstExpression *elses() const;

private:
  const AstExpression *condition_;
  const AstExpression *thens_;
  const AstExpression *elses_;
};

class A_AssignmentExpression : public AstExpression {
public:
  A_AssignmentExpression(const AstExpression *left, int token,
                         const AstExpression *right,
                         const Position &tokenPosition);
  virtual ~A_AssignmentExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const AstExpression *left() const;
  virtual int token() const;
  virtual const AstExpression *right() const;

private:
  const AstExpression *left_;
  int token_;
  const AstExpression *right_;
};

class A_VoidExpression : public AstExpression {
public:
  A_VoidExpression();
  virtual ~A_VoidExpression() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
};

// expression }

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

class AstExpressionList : public detail::AstList<AstExpression> {
public:
  AstExpressionList();
  virtual ~AstExpressionList() = default;
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
class AstExpressionStatement : public AstStatement {
public:
  AstExpressionStatement(AstExpression *expression,
                         const Position &semiTokenPosition);
  virtual ~AstExpressionStatement();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstExpression *expression() const;

private:
  AstExpression *expression_;
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
  A_IfStatement(AstExpression *condition, AstStatement *thens,
                AstStatement *elses, const Position &ifTokenPosition);
  virtual ~A_IfStatement();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstExpression *condition() const;
  virtual AstStatement *thens() const;
  virtual AstStatement *elses() const;

private:
  AstExpression *condition_;
  AstStatement *thens_;
  AstStatement *elses_;
};

/* iteration statement - while */
class AstWhileStatement : public AstStatement {
public:
  AstWhileStatement(AstExpression *condition, AstStatement *statement,
                    const Position &whileTokenPosition);
  virtual ~AstWhileStatement();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstExpression *condition() const;
  virtual AstStatement *statement() const;

private:
  AstExpression *condition_;
  AstStatement *statement_;
};

/* iteration statement - for */
class AstForStatement : public AstStatement {
public:
  AstForStatement(AstStatement *initial, AstStatement *condition,
                  AstExpression *post, AstStatement *statement,
                  const Position &forTokenPosition);
  virtual ~AstForStatement();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstStatement *start() const;
  virtual AstStatement *step() const;
  virtual AstExpression *end() const;
  virtual AstStatement *statement() const;

private:
  AstStatement *start_;
  AstStatement *step_;
  AstExpression *end_;
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
class A_ReturnExpression : public AstStatement {
public:
  A_ReturnExpression(AstExpression *expression,
                     const Position &returnTokenPosition,
                     const Position &semiTokenPosition);
  virtual ~A_ReturnExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual AstExpression *expression() const;

private:
  AstExpression *expression_;
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
                               AstExpression *expression,
                               const Position &identifierPosition,
                               const Position &typePosition);
  virtual ~AstVariableInitialDefinition();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const std::string &identifier() const;
  virtual AstId *typeId() const;
  virtual AstExpression *expression() const;

private:
  std::string identifier_;
  AstId *typeId_;
  AstExpression *expression_;
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
                                 AstExpression *result,
                                 const Position &funcTokenPosition,
                                 const Position &identifierPosition);
  virtual ~AstFunctionSignatureDefinition();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual AstDefinitionList *argumentList() const;
  virtual AstExpression *result() const;

private:
  std::string identifier_;
  AstDefinitionList *argumentList_;
  AstExpression *result_;
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
