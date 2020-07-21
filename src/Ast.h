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
BETTER_ENUM(A_ty, int,
            // constant literal expression
            Identifier = 1000, Int8Literal, UInt8Literal, Int16Literal,
            UInt16Literal, Int32Literal, UInt32Literal, Int64Literal,
            UInt64Literal, Float32Literal, Float64Literal, StringLiteral,
            BooleanLiteral,
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

/* base interface */
class Ast;
class AstLiteral;
class AstExpression;
class AstType;
class AstStatement;
class AstDefinition;

/* translate unit */
class AstTranslateUnit;

/* list */
class AstExpressionList;
class AstStatementList;
class AstDefinitionList;

/* constant */
class AstIdentifier;
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

/* base interface */
class Ast : public Namely, private boost::noncopyable {
public:
  Ast(const std::string &name);
  Ast(const std::string &name, const Position &position);
  virtual ~Ast() = default;
  virtual A_ty type() const = 0;
  virtual std::string toString() const = 0;
  virtual std::string name() const;
  virtual const Position &position() const;

  // contains FloatConstant and IntegerConstant
  static bool isConstant(const Ast *node);
  static bool isFloatConstant(const Ast *node);
  // contains SignedIntegerConstant and UnsignedIntegerConstant
  static bool isIntegerConstant(const Ast *node);
  static bool isSignedIntegerConstant(const Ast *node);
  static bool isUnsignedIntegerConstant(const Ast *node);
  static bool isExpression(const Ast *node);
  static bool isStatement(const Ast *node);
  static bool isDefinition(const Ast *node);

protected:
  Position position_;

private:
  std::string name_;
};

/* expression */
class AstExpression : public Ast {
public:
  AstExpression(const std::string &name);
  AstExpression(const std::string &name, const Position &position);
  virtual ~AstExpression() = default;
  virtual A_ty type() const = 0;
  virtual std::string toString() const = 0;
};

/* constant */
class AstLiteral : public AstExpression {
public:
  AstLiteral(const std::string &name);
  AstLiteral(const std::string &name, const Position &position);
  virtual ~AstLiteral() = default;
  virtual A_ty type() const = 0;
  virtual std::string toString() const = 0;
};

/* statement */
class AstStatement : public Ast {
public:
  AstStatement(const std::string &name);
  AstStatement(const std::string &name, const Position &position);
  virtual ~AstStatement() = default;
  virtual A_ty type() const = 0;
  virtual std::string toString() const = 0;
};

/* definition is statement */
class AstDefinition : public AstStatement {
public:
  AstDefinition(const std::string &name);
  AstDefinition(const std::string &name, const Position &position);
  virtual ~AstDefinition() = default;
  virtual A_ty type() const = 0;
  virtual std::string toString() const = 0;
};

namespace detail {

template <class T> class AstList : public Ast {
public:
  AstList(const std::string &name) : Ast(name) {}
  virtual ~AstList() {
    for (int i = 0; i < (int)items_.size(); i++) {
      delete items_[i];
      items_[i] = nullptr;
    }
    items_.clear();
  }
  virtual A_ty type() const = 0;
  virtual std::string toString() const {
    std::stringstream ss;
    ss << fmt::format("[@{} {} size:{}", stringify(), position_.toString(),
                      items_.size());
    if (items_.empty()) {
      ss << "]";
      return ss.str();
    }
    ss << ", ";
    for (int i = 0; i < (int)items_.size(); i++) {
      Ast *item = dynamic_cast<Ast *>(items_[i]);
      EX_ASSERT(item, "item is null");
      ss << fmt::format("{}:{}", i, item->toString());
      if (i < (int)items_.size() - 1) {
        ss << ",";
      }
    }
    ss << "]";
    return ss.str();
  }
  virtual bool empty() const { return items_.empty(); }
  virtual int size() const { return items_.size(); }
  virtual T *get(int pos) const {
    EX_ASSERT(pos >= 0, "pos {} >= 0");
    EX_ASSERT(items_[pos], "items_[{}] is null", pos);
    return items_[pos];
  }
  virtual void add(T *item) {
    EX_ASSERT(item, "item is null");
    position_.updatePosition(item->position());
    items_.push_front(item);
  }

protected:
  virtual std::string stringify() const = 0;
  std::deque<T *> items_;
};

} // namespace detail

class AstExpressionList : public detail::AstList<AstExpression> {
public:
  AstExpressionList();
  virtual ~AstExpressionList() = default;
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual bool empty() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

private:
  virtual std::string stringify() const;
};

/* constant expression - T_IDENTIFIER */
class AstIdentifier : public AstExpression {
public:
  AstIdentifier(const char *value, const Position &position);
  virtual ~AstIdentifier() = default;
  virtual A_ty type() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;

private:
  std::string value_;
};

/* type identifier */
class AstInt8TypeIdentifier : public AstExpression {};

/* constant expression */
class AstInt8Literal : public AstLiteral {
public:
  AstInt8Literal(const int8_t &value, const Position &position);
  virtual ~AstInt8Literal() = default;
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
  virtual std::string toString() const;
};

/* expression statement */
class AstExpressionStatement : public AstStatement {
public:
  AstExpressionStatement(AstExpression *expression,
                         const Position &semiTokenPosition);
  virtual ~AstExpressionStatement();
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
  virtual std::string toString() const;

private:
};

/* jump statement - break */
class AstBreakStatement : public AstStatement {
public:
  AstBreakStatement(const Position &breakTokenPosition,
                    const Position &semiTokenPosition);
  virtual ~AstBreakStatement() = default;
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
  virtual std::string toString() const;
};

/* variable definition */
class AstVariableDefinition : public AstDefinition {
public:
  AstVariableDefinition(AstDefinitionList *definitionList,
                        const Position &varTokenPosition,
                        const Position &semiTokenPosition);
  virtual ~AstVariableDefinition();
  virtual A_ty type() const;
  virtual std::string toString() const;
  virtual AstDefinitionList *definitionList() const;

private:
  AstDefinitionList *definitionList_;
};

/* variable initial definition */
class AstVariableInitialDefinition : public AstDefinition {
public:
  AstVariableInitialDefinition(const char *identifier, AstIdentifier *typeId,
                               AstExpression *expression,
                               const Position &identifierPosition,
                               const Position &typePosition);
  virtual ~AstVariableInitialDefinition();
  virtual A_ty type() const;
  virtual std::string toString() const;
  virtual const std::string &identifier() const;
  virtual AstIdentifier *typeId() const;
  virtual AstExpression *expression() const;

private:
  std::string identifier_;
  AstIdentifier *typeId_;
  AstExpression *expression_;
};

/* function definition */
class AstFunctionDefinition : public AstDefinition {
public:
  AstFunctionDefinition(AstFunctionSignatureDefinition *signature,
                        AstStatement *statement);
  virtual ~AstFunctionDefinition();
  virtual A_ty type() const;
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
  virtual A_ty type() const;
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
  virtual A_ty type() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;

private:
  std::string identifier_;
};
