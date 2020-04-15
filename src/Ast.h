// Copyright 2019- <rsc-lang>
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
            IdentifierConstant = 1000, F32Constant, F64Constant, StringConstant,
            I8Constant, U8Constant, I16Constant, U16Constant, I32Constant,
            U32Constant, I64Constant, U64Constant, BooleanConstant,
            // expression
            AssignmentExpression, SequelExpression, CallExpression,
            UnaryExpression, BinaryExpression, ConditionalExpression,
            // statement
            ExpressionStatement, CompoundStatement, IfStatement, WhileStatement,
            ForStatement, ContinueStatement, BreakStatement, ReturnStatement,
            EmptyStatement,
            // declaration
            VariableDeclaration, VariableAssignmentDeclaration,
            FunctionDeclaration, FunctionArgumentDeclaration,
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

/* expression */
class AstExpression;
class AstIdentifierConstant;
class AstI8Constant;
class AstU8Constant;
class AstI16Constant;
class AstU16Constant;
class AstI32Constant;
class AstU32Constant;
class AstI64Constant;
class AstU64Constant;
class AstF32Constant;
class AstF64Constant;
class AstStringConstant;
class AstBooleanConstant;
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
class AstVariableAssignmentDeclaration;
class AstFunctionDeclaration;
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
  virtual ~AstExpressionList();
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
  virtual ~AstStatementList();
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
  virtual ~AstDeclarationList();
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
  virtual ~AstTranslateUnit();
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
  virtual ~AstIdentifierConstant();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;

private:
  std::string value_;
};

/* constant expression - T_I8_CONSTANT */
class AstI8Constant : public AstExpression {
public:
  AstI8Constant(const int8_t &value);
  virtual ~AstI8Constant();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const int8_t &value() const;

private:
  int8_t value_;
};

/* constant expression - T_U8_CONSTANT */
class AstU8Constant : public AstExpression {
public:
  AstU8Constant(const uint8_t &value);
  virtual ~AstU8Constant();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const uint8_t &value() const;

private:
  uint8_t value_;
};

/* constant expression - T_I16_CONSTANT */
class AstI16Constant : public AstExpression {
public:
  AstI16Constant(const int16_t &value);
  virtual ~AstI16Constant();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const int16_t &value() const;

private:
  int16_t value_;
};

/* constant expression - T_U16_CONSTANT */
class AstU16Constant : public AstExpression {
public:
  AstU16Constant(const uint16_t &value);
  virtual ~AstU16Constant();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const uint16_t &value() const;

private:
  uint16_t value_;
};

/* constant expression - T_I32_CONSTANT */
class AstI32Constant : public AstExpression {
public:
  AstI32Constant(const int32_t &value);
  virtual ~AstI32Constant();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const int32_t &value() const;

private:
  int32_t value_;
};

/* constant expression - T_U32_CONSTANT */
class AstU32Constant : public AstExpression {
public:
  AstU32Constant(const uint32_t &value);
  virtual ~AstU32Constant();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const uint32_t &value() const;

private:
  uint32_t value_;
};

/* constant expression - T_I64_CONSTANT */
class AstI64Constant : public AstExpression {
public:
  AstI64Constant(const int64_t &value);
  virtual ~AstI64Constant();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const int64_t &value() const;

private:
  int64_t value_;
};

/* constant expression - T_U64_CONSTANT */
class AstU64Constant : public AstExpression {
public:
  AstU64Constant(const uint64_t &value);
  virtual ~AstU64Constant();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const uint64_t &value() const;

private:
  uint64_t value_;
};

/* constant expression - T_F32_CONSTANT */
class AstF32Constant : public AstExpression {
public:
  AstF32Constant(const float &value);
  virtual ~AstF32Constant();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const float &value() const;

private:
  float value_;
};

/* constant expression - T_F64_CONSTANT */
class AstF64Constant : public AstExpression {
public:
  AstF64Constant(const double &value);
  virtual ~AstF64Constant();
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
  virtual ~AstStringConstant();
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
  virtual ~AstBooleanConstant();
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
  AstConditionalExpression(AstExpression *condition, AstExpression *hit,
                           AstExpression *miss);
  virtual ~AstConditionalExpression();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual AstExpression *condition() const;
  virtual AstExpression *hit() const;
  virtual AstExpression *miss() const;

private:
  AstExpression *condition_;
  AstExpression *hit_;
  AstExpression *miss_;
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
  AstIfStatement(AstExpression *condition, AstStatement *hit,
                 AstStatement *miss);
  virtual ~AstIfStatement();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual AstExpression *condition() const;
  virtual AstStatement *hit() const;
  virtual AstStatement *miss() const;

private:
  AstExpression *condition_;
  AstStatement *hit_;
  AstStatement *miss_;
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

  virtual AstStatement *initial() const;
  virtual AstStatement *condition() const;
  virtual AstExpression *post() const;
  virtual AstStatement *statement() const;

private:
  AstStatement *initial_;
  AstStatement *condition_;
  AstExpression *post_;
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

/* variable assignment declaration */
class AstVariableAssignmentDeclaration : public AstDeclaration {
public:
  AstVariableAssignmentDeclaration(const char *identifier,
                                   AstExpression *expression);
  virtual ~AstVariableAssignmentDeclaration();
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
  AstFunctionDeclaration(const char *identifier,
                         AstDeclarationList *argumentList,
                         AstExpression *result, AstStatement *statement);
  virtual ~AstFunctionDeclaration();
  virtual AstType type() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual AstDeclarationList *argumentList() const;
  virtual AstExpression *result() const;
  virtual AstStatement *statement() const;

private:
  std::string identifier_;
  AstDeclarationList *argumentList_;
  AstExpression *result_;
  AstStatement *statement_;
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
