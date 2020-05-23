// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
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
BETTER_ENUM(AstType, int,
            // constant
            IdentifierConstant = 1000, Float32Constant, Float64Constant,
            StringConstant, Int8Constant, UInt8Constant, Int16Constant,
            UInt16Constant, Int32Constant, UInt32Constant, Int64Constant,
            UInt64Constant, BooleanConstant,
            // expression
            AssignmentExpression, SequelExpression, CallExpression,
            UnaryExpression, BinaryExpression, ConditionalExpression,
            // statement
            ExpressionStatement, IfStatement, WhileStatement, ForStatement,
            ContinueStatement, BreakStatement, ReturnStatement, EmptyStatement,
            CompoundStatement,
            // declaration
            VariableDeclaration, VariableInitialDeclaration,
            FunctionDeclaration, FunctionSignatureDeclaration,
            FunctionArgumentDeclaration,
            // list
            ExpressionList, StatementList, DeclarationList,
            // translate unit
            TranslateUnit)

/*================ class ================*/

/* base interface */
class Ast;

/* translate unit */
class AstTranslateUnit;

/* list */
class AstExpressionList;
class AstStatementList;
class AstDeclarationList;

/* constant */
class AstIdentifierConstant;
class AstInt8Constant;
class AstUInt8Constant;
class AstInt16Constant;
class AstUInt16Constant;
class AstInt32Constant;
class AstUInt32Constant;
class AstInt64Constant;
class AstUInt64Constant;
class AstFloat32Constant;
class AstFloat64Constant;
class AstStringConstant;
class AstBooleanConstant;

/* expression */
class AstExpression;
class AstCallExpression;
class AstUnaryExpression;
class AstBinaryExpression;
class AstConditionalExpression;
class AstAssignmentExpression;
class AstSequelExpression;

/* statement */
class AstStatement;
class AstExpressionStatement;
class AstCompoundStatement;
class AstIfStatement;
class AstWhileStatement;
class AstForStatement;
class AstContinueStatement;
class AstBreakStatement;
class AstReturnStatement;
class AstEmptyStatement;

/* declaration */
class AstDeclaration;
class AstVariableDeclaration;
class AstVariableInitialDeclaration;
class AstFunctionDeclaration;
class AstFunctionSignatureDeclaration;
class AstFunctionArgumentDeclaration;

/*================ declaration ================*/

/* base interface */
class Ast : public Namely, public Stringify, private boost::noncopyable {
public:
  Ast(const std::string &name);
  virtual ~Ast() = default;
  virtual AstType type() const = 0;
  virtual std::string toString() const = 0;
  virtual std::string name() const;

  static bool isFloat(const Ast *node);
  static bool isInteger(const Ast *node);
  static bool isSignedInteger(const Ast *node);
  static bool isUnsignedInteger(const Ast *node);

private:
  std::string name_;
};

/* expression */
class AstExpression : public Ast {
public:
  AstExpression(const std::string &name);
  virtual ~AstExpression() = default;
  virtual AstType type() const = 0;
  virtual std::string toString() const = 0;
};

/* statement */
class AstStatement : public Ast {
public:
  AstStatement(const std::string &name);
  virtual ~AstStatement() = default;
  virtual AstType type() const = 0;
  virtual std::string toString() const = 0;
};

/* declaration is statement */
class AstDeclaration : public AstStatement {
public:
  AstDeclaration(const std::string &name);
  virtual ~AstDeclaration() = default;
  virtual AstType type() const = 0;
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
  virtual AstType type() const = 0;
  virtual std::string toString() const {
    std::stringstream ss;
    ss << fmt::format("[ @{} size:{}", stringify(), items_.size());
    if (items_.empty()) {
      ss << " ]";
      return ss.str();
    }
    ss << ", ";
    for (int i = 0; i < (int)items_.size(); i++) {
      Ast *item = dynamic_cast<Ast *>(items_[i]);
      ss << fmt::format("{}:{}", i, item ? item->toString() : "null");
      if (i < (int)items_.size() - 1) {
        ss << ",";
      }
    }
    ss << " ]";
    return ss.str();
  }
  virtual bool empty() const { return items_.empty(); }
  virtual int size() const { return items_.size(); }
  virtual T *get(int pos) const { return items_[pos]; }
  virtual void add(T *item) { items_.push_front(item); }

protected:
  virtual std::string stringify() const = 0;
  std::deque<T *> items_;
};

} // namespace detail

class AstExpressionList : public detail::AstList<AstExpression> {
public:
  AstExpressionList();
  virtual ~AstExpressionList() = default;
  virtual AstType type() const;
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
  virtual AstType type() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual bool empty() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

private:
  virtual std::string stringify() const;
};

class AstDeclarationList : public detail::AstList<AstDeclaration> {
public:
  AstDeclarationList();
  virtual ~AstDeclarationList() = default;
  virtual AstType type() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual bool empty() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

private:
  virtual std::string stringify() const;
};

/* translate unit is actually declaration list */

class AstTranslateUnit : public detail::AstList<AstDeclaration> {
public:
  AstTranslateUnit();
  virtual ~AstTranslateUnit() = default;
  virtual AstType type() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual bool empty() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

private:
  virtual std::string stringify() const;
};

/* constant expression - T_IDENTIFIER */
class AstIdentifierConstant : public AstExpression {
public:
  AstIdentifierConstant(const char *value);
  virtual ~AstIdentifierConstant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;

private:
  std::string value_;
};

/* constant expression - T_INT8_CONSTANT */
class AstInt8Constant : public AstExpression {
public:
  AstInt8Constant(const int8_t &value);
  virtual ~AstInt8Constant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const int8_t &value() const;

private:
  int8_t value_;
};

/* constant expression - T_UINT8_CONSTANT */
class AstUInt8Constant : public AstExpression {
public:
  AstUInt8Constant(const uint8_t &value);
  virtual ~AstUInt8Constant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const uint8_t &value() const;

private:
  uint8_t value_;
};

/* constant expression - T_INT16_CONSTANT */
class AstInt16Constant : public AstExpression {
public:
  AstInt16Constant(const int16_t &value);
  virtual ~AstInt16Constant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const int16_t &value() const;

private:
  int16_t value_;
};

/* constant expression - T_UINT16_CONSTANT */
class AstUInt16Constant : public AstExpression {
public:
  AstUInt16Constant(const uint16_t &value);
  virtual ~AstUInt16Constant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const uint16_t &value() const;

private:
  uint16_t value_;
};

/* constant expression - T_INT32_CONSTANT */
class AstInt32Constant : public AstExpression {
public:
  AstInt32Constant(const int32_t &value);
  virtual ~AstInt32Constant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const int32_t &value() const;

private:
  int32_t value_;
};

/* constant expression - T_UINT32_CONSTANT */
class AstUInt32Constant : public AstExpression {
public:
  AstUInt32Constant(const uint32_t &value);
  virtual ~AstUInt32Constant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const uint32_t &value() const;

private:
  uint32_t value_;
};

/* constant expression - T_INT64_CONSTANT */
class AstInt64Constant : public AstExpression {
public:
  AstInt64Constant(const int64_t &value);
  virtual ~AstInt64Constant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const int64_t &value() const;

private:
  int64_t value_;
};

/* constant expression - T_UINT64_CONSTANT */
class AstUInt64Constant : public AstExpression {
public:
  AstUInt64Constant(const uint64_t &value);
  virtual ~AstUInt64Constant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const uint64_t &value() const;

private:
  uint64_t value_;
};

/* constant expression - T_FLOAT32_CONSTANT */
class AstFloat32Constant : public AstExpression {
public:
  AstFloat32Constant(const float &value);
  virtual ~AstFloat32Constant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const float &value() const;

private:
  float value_;
};

/* constant expression - T_FLOAT64_CONSTANT */
class AstFloat64Constant : public AstExpression {
public:
  AstFloat64Constant(const double &value);
  virtual ~AstFloat64Constant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const double &value() const;

private:
  double value_;
};

/* constant expression - T_STRING_CONSTANT */
class AstStringConstant : public AstExpression {
public:
  AstStringConstant(const char *value);
  virtual ~AstStringConstant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;
  virtual void add(const char *value);

private:
  std::string value_;
};

/* constant expression - T_TRUE T_FALSE */
class AstBooleanConstant : public AstExpression {
public:
  AstBooleanConstant(const bool &value);
  virtual ~AstBooleanConstant() = default;
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const bool &value() const;

private:
  bool value_;
};

/* function call expression */
class AstCallExpression : public AstExpression {
public:
  AstCallExpression(const char *identifier, AstExpressionList *expressionList);
  virtual ~AstCallExpression();
  virtual AstType type() const;
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
  AstUnaryExpression(int token, AstExpression *expression);
  virtual ~AstUnaryExpression();
  virtual AstType type() const;
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
  AstBinaryExpression(AstExpression *left, int token, AstExpression *right);
  virtual ~AstBinaryExpression();
  virtual AstType type() const;
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
  virtual AstType type() const;
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
                          AstExpression *value);
  virtual ~AstAssignmentExpression();
  virtual AstType type() const;
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
  virtual AstType type() const;
  virtual std::string toString() const;
  virtual AstExpressionList *expressionList() const;

private:
  AstExpressionList *expressionList_;
};

/* expression statement */
class AstExpressionStatement : public AstStatement {
public:
  AstExpressionStatement(AstExpression *expression);
  virtual ~AstExpressionStatement();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual AstExpression *expression() const;

private:
  AstExpression *expression_;
};

/* compound statement - { ... } */
class AstCompoundStatement : public AstStatement {
public:
  AstCompoundStatement(AstStatementList *statementList);
  virtual ~AstCompoundStatement();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual AstStatementList *statementList() const;

private:
  AstStatementList *statementList_;
};

/* selection statement - if else */
class AstIfStatement : public AstStatement {
public:
  AstIfStatement(AstExpression *condition, AstStatement *thens,
                 AstStatement *elses);
  virtual ~AstIfStatement();
  virtual AstType type() const;
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
  AstWhileStatement(AstExpression *condition, AstStatement *statement);
  virtual ~AstWhileStatement();
  virtual AstType type() const;
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
                  AstExpression *post, AstStatement *statement);
  virtual ~AstForStatement();
  virtual AstType type() const;
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
  AstContinueStatement();
  virtual ~AstContinueStatement();
  virtual AstType type() const;
  virtual std::string toString() const;

private:
};

/* jump statement - break */
class AstBreakStatement : public AstStatement {
public:
  AstBreakStatement();
  virtual ~AstBreakStatement();
  virtual AstType type() const;
  virtual std::string toString() const;

private:
};

/* jump statement - return */
class AstReturnStatement : public AstStatement {
public:
  AstReturnStatement(AstExpression *expression);
  virtual ~AstReturnStatement();
  virtual AstType type() const;
  virtual std::string toString() const;
  virtual AstExpression *expression() const;

private:
  AstExpression *expression_;
};

/* empty statement */
class AstEmptyStatement : public AstStatement {
public:
  AstEmptyStatement();
  virtual ~AstEmptyStatement();
  virtual AstType type() const;
  virtual std::string toString() const;

private:
};

/* variable declaration */
class AstVariableDeclaration : public AstDeclaration {
public:
  AstVariableDeclaration(AstDeclarationList *declarationList);
  virtual ~AstVariableDeclaration();
  virtual AstType type() const;
  virtual std::string toString() const;
  virtual AstDeclarationList *declarationList() const;

private:
  AstDeclarationList *declarationList_;
};

/* variable initial declaration */
class AstVariableInitialDeclaration : public AstDeclaration {
public:
  AstVariableInitialDeclaration(const char *identifier,
                                AstExpression *expression);
  virtual ~AstVariableInitialDeclaration();
  virtual AstType type() const;
  virtual std::string toString() const;
  virtual const std::string &identifier() const;
  virtual AstExpression *expression() const;

private:
  std::string identifier_;
  AstExpression *expression_;
};

/* function declaration */
class AstFunctionDeclaration : public AstDeclaration {
public:
  AstFunctionDeclaration(AstFunctionSignatureDeclaration *signature,
                         AstStatement *statement);
  virtual ~AstFunctionDeclaration();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual AstFunctionSignatureDeclaration *signature() const;
  virtual AstStatement *statement() const;

private:
  AstFunctionSignatureDeclaration *signature_;
  AstStatement *statement_;
};

/* function signature declaration */
class AstFunctionSignatureDeclaration : public AstDeclaration {
public:
  AstFunctionSignatureDeclaration(const char *identifier,
                                  AstDeclarationList *argumentList,
                                  AstExpression *result);
  virtual ~AstFunctionSignatureDeclaration();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual AstDeclarationList *argumentList() const;
  virtual AstExpression *result() const;

private:
  std::string identifier_;
  AstDeclarationList *argumentList_;
  AstExpression *result_;
};

/* function argument declaration */
class AstFunctionArgumentDeclaration : public AstDeclaration {
public:
  AstFunctionArgumentDeclaration(const char *value);
  virtual ~AstFunctionArgumentDeclaration();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;

private:
  std::string value_;
};
