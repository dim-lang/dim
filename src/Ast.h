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
            // id
            PlainId = 1000, StableId,
            // type
            FunctionType, PrimitiveType,
            // literal
            IntegerLiteral, StringLiteral, CharLiteral, FloatLiteral,
            BooleanLiteral, NilLiteral, VoidLiteral,
            // expression
            AssignmentExpression, SequelExpression, CallExpression,
            UnaryExpression, BinaryExpression, ConditionalExpression,
            VoidExpression,
            // statement
            ExpressionStatement, IfStatement, WhileStatement, ForStatement,
            ContinueStatement, BreakStatement, ReturnStatement, EmptyStatement,
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

/* id */
class AstId;
class AstPlainId;

/* type */
class AstType;
class A_FunctionType;
class A_PrimitiveType;

/* expression */
class AstExpression;

/* literal */
class AstLiteral;
class A_IntegerLiteral;
class A_FloatLiteral;
class A_StringLiteral;
class A_CharLiteral;
class A_BooleanLiteral;
class A_NilLiteral;
class A_VoidLiteral;

class AstStatement;
class AstDefinition;

/* translate unit */
class AstTranslateUnit;

/* list */
class AstExpressionList;
class AstStatementList;
class AstDefinitionList;

/* constant */
class AstInt8Literal;
class AstUInt8Literal;
class AstInt16Literal;
class AstUInt16Literal;
class AstInt32Literal;
class AstUInt32Literal;
class AstInt64Literal;
class AstUInt64Literal;
class AstFloat32Literal;
class AstFloat64Literal;
class AstStringLiteral;
class AstBooleanLiteral;

/* expression */
class AstCallExpression;
class AstUnaryExpression;
class AstBinaryExpression;
class AstConditionalExpression;
class AstAssignmentExpression;
class AstSequelExpression;
class AstVoidExpression;

/* statement */
class AstExpressionStatement;
class AstCompoundStatement;
class AstIfStatement;
class AstWhileStatement;
class AstForStatement;
class AstContinueStatement;
class AstBreakStatement;
class AstReturnStatement;
class AstEmptyStatement;

/* definition */
class AstVariableDefinition;
class AstVariableInitialDefinition;
class AstFunctionDefinition;
class AstFunctionSignatureDefinition;
class AstFunctionArgumentDefinition;

/*================ definition ================*/

// Ast {

// interface
class Ast : private boost::noncopyable {
public:
  Ast(const std::string &name);
  virtual ~Ast() = default;
  virtual const std::string &name() const;
  virtual const Position &position() const;
  virtual AstCategory category() const = 0;
  virtual std::string toString() const = 0;

protected:
  void collect(const Position &position);
  void collect(const Ast *node);

private:
  Position position_;
  std::string name_;
};

// Ast }

// A_List {

namespace detail {

template <class T> class A_List : public Ast {
public:
  A_List(const std::string &name) : Ast(name) {}
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
    ss << fmt::format("[@{} size:{}, position:{}", items_.size(), namely(),
                      position().toString());
    if (items_.empty()) {
      ss << "]";
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
    ss << "]";
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
    EX_ASSERT(item, "item is null");
    collect(dynamic_cast<const Ast *>(item)->position());
    items_.push_front(item);
  }

protected:
  virtual std::string namely() const = 0;
  std::deque<const T *> items_;
};

} // namespace detail

// A_List }

// id {

// interface
class AstId : public Ast {
public:
  AstId(const std::string &name);
  virtual ~AstId() = default;
};

class A_PlainId : public AstId {
public:
  A_PlainId(const std::string &literal, const Position &position);
  virtual ~A_PlainId() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual const std::string &literal() const;

private:
  std::string literal_;
};

/* class A_StableId : public AstId { */
/* public: */
/*   virtual ~A_StableId() = default; */
/*   virtual AstCategory category() const; */
/*   virtual std::string toString() const; */
/*   virtual const std::string &literal() const; */

/*   static const A_StableId *get(int token, const Position &position); */

/* private: */
/*   A_StableId(const std::string &literal, const Position &position); */
/*   std::string literal_; */
/* }; */

// id }

// type {

// interface
class AstType : public Ast {
public:
  AstType(const std::string &name);
  virtual ~AstType() = default;
};

class A_FunctionType : public AstType {
public:
  A_FunctionType(const std::string &name);
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

// expression {

class AstExpression : public Ast {
public:
  AstExpression(const std::string &name);
  virtual ~AstExpression() = default;
};

// literal {

class AstLiteral : public AstExpression {
public:
  AstLiteral(const std::string &name);
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
  enum class BitCategory { SIGNED = 200, UNSIGNED, LONG, ULONG };

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

class A_FloatLiteral : public AstLiteral {
public:
  // DecimalCategory:
  // DEC: base 10
  // HEX: base 16
  enum class DecimalCategory { DEC = 300, HEX };

  // BitCategory:
  // FLT: 32 bit
  // DBL: 64 bit
  enum class BitCategory { FLT = 400, DBL };

  A_FloatLiteral(const std::string &literal, const Position &position);
  virtual ~A_FloatLiteral() = default;
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

class A_StringLiteral : public AstLiteral {
public:
  // QuoteCategory
  // Single: "
  // Triple: """
  enum class QuoteCategory { SINGLE = 500, TRIPLE };

  A_StringLiteral(const std::string &literal, const Position &position);
  virtual ~A_StringLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const std::string &parsed() const;
  virtual QuoteCategory quoteCategory() const;
  virtual const std::deque<std::string> &literals() const;
  virtual void join(const std::string &literal, const Position &position);

private:
  std::deque<std::string> literals_;
  std::string parsed_;
  QuoteCategory quoteCategory_;
};

class A_CharLiteral : public AstLiteral {
  A_CharLiteral(const std::string &literal, const Position &position);
  virtual ~A_CharLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const std::string &literal() const;
  virtual const char &parsed() const;

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

// expression }

// statement {

class AstStatement : public Ast {
public:
  AstStatement(const std::string &name);
  virtual ~AstStatement() = default;
};

// statement }

/* definition is statement */
class AstDefinition : public AstStatement {
public:
  AstDefinition(const std::string &name);
  AstDefinition(const std::string &name, const Position &position);
  virtual ~AstDefinition() = default;
  virtual AstCategory category() const = 0;
  virtual std::string toString() const = 0;
};

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

/* type identifier */
class AstInt8TypeIdentifier : public AstExpression {};

/* constant expression */
class AstInt8Literal : public AstLiteral {
public:
  AstInt8Literal(const int8_t &value, const Position &position);
  virtual ~AstInt8Literal() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const int8_t &value() const;

private:
  int8_t value_;
};

/* constant expression */
class AstUInt8Literal : public AstLiteral {
public:
  AstUInt8Literal(const uint8_t &value, const Position &position);
  virtual ~AstUInt8Literal() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const uint8_t &value() const;

private:
  uint8_t value_;
};

/* constant expression */
class AstInt16Literal : public AstLiteral {
public:
  AstInt16Literal(const int16_t &value, const Position &position);
  virtual ~AstInt16Literal() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const int16_t &value() const;

private:
  int16_t value_;
};

/* constant expression */
class AstUInt16Literal : public AstLiteral {
public:
  AstUInt16Literal(const uint16_t &value, const Position &position);
  virtual ~AstUInt16Literal() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const uint16_t &value() const;

private:
  uint16_t value_;
};

/* constant expression - T_INT32_LITERAL */
class AstInt32Literal : public AstLiteral {
public:
  AstInt32Literal(const int32_t &value, const Position &position);
  virtual ~AstInt32Literal() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const int32_t &value() const;

private:
  int32_t value_;
};

/* constant expression - T_UINT32_LITERAL */
class AstUInt32Literal : public AstLiteral {
public:
  AstUInt32Literal(const uint32_t &value, const Position &position);
  virtual ~AstUInt32Literal() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const uint32_t &value() const;

private:
  uint32_t value_;
};

/* constant expression - T_INT64_LITERAL */
class AstInt64Literal : public AstLiteral {
public:
  AstInt64Literal(const int64_t &value, const Position &position);
  virtual ~AstInt64Literal() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const int64_t &value() const;

private:
  int64_t value_;
};

/* constant expression - T_UINT64_LITERAL */
class AstUInt64Literal : public AstLiteral {
public:
  AstUInt64Literal(const uint64_t &value, const Position &position);
  virtual ~AstUInt64Literal() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const uint64_t &value() const;

private:
  uint64_t value_;
};

/* constant expression - T_FLOAT32_LITERAL */
class AstFloat32Literal : public AstLiteral {
public:
  AstFloat32Literal(const float &value, const Position &position);
  virtual ~AstFloat32Literal() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const float &value() const;

private:
  float value_;
};

/* constant expression - T_FLOAT64_LITERAL */
class AstFloat64Literal : public AstLiteral {
public:
  AstFloat64Literal(const double &value, const Position &position);
  virtual ~AstFloat64Literal() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const double &value() const;

private:
  double value_;
};

/* constant expression - T_STRING_LITERAL */
class AstStringLiteral : public AstLiteral {
public:
  AstStringLiteral(const char *value, const Position &position);
  virtual ~AstStringLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;
  virtual void add(const char *value, const Position &position);

private:
  std::string value_;
};

/* constant expression - T_TRUE T_FALSE */
class AstBooleanLiteral : public AstLiteral {
public:
  AstBooleanLiteral(const bool &value, const Position &position);
  virtual ~AstBooleanLiteral() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const bool &value() const;

private:
  bool value_;
};

/* function call expression */
class AstCallExpression : public AstExpression {
public:
  AstCallExpression(const char *identifier, AstExpressionList *expressionList,
                    const Position &identifierPosition);
  virtual ~AstCallExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual AstExpressionList *argumentList() const;

private:
  std::string identifier_;
  AstExpressionList *argumentList_;
};

/* unary operation expression */
class AstUnaryExpression : public AstExpression {
public:
  AstUnaryExpression(int token, AstExpression *expression,
                     const Position &tokenPosition);
  virtual ~AstUnaryExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual int token() const;
  virtual AstExpression *expression() const;

private:
  int token_;
  AstExpression *expression_;
};

/* binary operation expression */
class AstBinaryExpression : public AstExpression {
public:
  AstBinaryExpression(AstExpression *left, int token, AstExpression *right,
                      const Position &tokenPosition);
  virtual ~AstBinaryExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstExpression *left() const;
  virtual int token() const;
  virtual AstExpression *right() const;

private:
  AstExpression *left_;
  int token_;
  AstExpression *right_;
};

/* ternary conditional expression */
class AstConditionalExpression : public AstExpression {
public:
  AstConditionalExpression(AstExpression *condition, AstExpression *thens,
                           AstExpression *elses);
  virtual ~AstConditionalExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstExpression *condition() const;
  virtual AstExpression *thens() const;
  virtual AstExpression *elses() const;

private:
  AstExpression *condition_;
  AstExpression *thens_;
  AstExpression *elses_;
};

/* assignment expression */
class AstAssignmentExpression : public AstExpression {
public:
  AstAssignmentExpression(AstExpression *variable, int token,
                          AstExpression *value, const Position &tokenPosition);
  virtual ~AstAssignmentExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;

  virtual AstExpression *variable() const;
  virtual int token() const;
  virtual AstExpression *value() const;

private:
  AstExpression *variable_;
  int token_;
  AstExpression *value_;
};

/* sequel expression */
class AstSequelExpression : public AstExpression {
public:
  AstSequelExpression(AstExpressionList *expressionList);
  virtual ~AstSequelExpression();
  virtual AstCategory category() const;
  virtual std::string toString() const;
  virtual AstExpressionList *expressionList() const;

private:
  AstExpressionList *expressionList_;
};

/* void expression */
class AstVoidExpression : public AstExpression {
public:
  AstVoidExpression();
  virtual ~AstVoidExpression() = default;
  virtual AstCategory category() const;
  virtual std::string toString() const;
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
class AstIfStatement : public AstStatement {
public:
  AstIfStatement(AstExpression *condition, AstStatement *thens,
                 AstStatement *elses, const Position &ifTokenPosition);
  virtual ~AstIfStatement();
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
class AstReturnStatement : public AstStatement {
public:
  AstReturnStatement(AstExpression *expression,
                     const Position &returnTokenPosition,
                     const Position &semiTokenPosition);
  virtual ~AstReturnStatement();
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
