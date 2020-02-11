// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

/* Ast::type */

#define A_IDENTIFIER_CONSTANT 101
#define A_F32_CONSTANT 102
#define A_F64_CONSTANT 103
#define A_STRING_CONSTANT 104
#define A_CALL_EXPRESSION 105
#define A_UNARY_OPERATION_EXPRESSION 106
#define A_BINARY_OPERATION_EXPRESSION 107
#define A_CONDITIONAL_EXPRESSION 108
#define A_I8_CONSTANT 109
#define A_UI8_CONSTANT 110
#define A_I16_CONSTANT 111
#define A_UI16_CONSTANT 112
#define A_I32_CONSTANT 113
#define A_UI32_CONSTANT 114
#define A_I64_CONSTANT 115
#define A_UI64_CONSTANT 116
#define A_BOOLEAN_CONSTANT 117
#define A_ASSIGNMENT_EXPRESSION 118

#define A_EXPRESSION_STATEMENT 201
#define A_COMPOUND_STATEMENT 202
#define A_IF_STATEMENT 203
#define A_WHILE_STATEMENT 204
#define A_FOR_STATEMENT 205
#define A_CONTINUE_STATEMENT 206
#define A_BREAK_STATEMENT 207
#define A_RETURN_STATEMENT 207

#define A_VARIABLE_DECLARATION 301
#define A_VARIABLE_ASSIGNMENT_DECLARATION 302
#define A_FUNCTION_DECLARATION 303
#define A_FUNCTION_ARGUMENT_DECLARATION 304

#define A_EXPRESSION_LIST 401
#define A_STATEMENT_LIST 402
#define A_DECLARATION_LIST 403

#define A_PROGRAM 501

/* ================================== */

/* interface */
class Ast;

/* list */
class AstExpressionList;
class AstStatementList;
class AstDeclarationList;

/* expression */
class AstExpression;
class AstIdentifierConstant;
class AstI8Constant;
class AstUI8Constant;
class AstI16Constant;
class AstUI16Constant;
class AstI32Constant;
class AstUI32Constant;
class AstI64Constant;
class AstUI64Constant;
class AstF32Constant;
class AstF64Constant;
class AstStringConstant;
class AstBooleanConstant;
class AstCallExpression;
class AstUnaryExpression;
class AstBinaryExpression;
class AstConditionalExpression;
class AstAssignmentExpression;

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

/* declaration */
class AstDeclaration;
class AstVariableDeclaration;
class AstVariableAssignmentDeclaration;
class AstFunctionDeclaration;
class AstFunctionArgumentDeclaration;

/* ================================== */

class Ast : public Stringify {
public:
  virtual ~Ast() = default;
  virtual int type() const = 0;
  virtual std::string toString() const = 0;
};

/* expression */
class AstExpression : public Ast {
public:
  virtual ~AstExpression() = default;
  virtual int type() const = 0;
  virtual std::string toString() const = 0;
};

/* statement */
class AstStatement : public Ast {
public:
  virtual ~AstStatement() = default;
  virtual int type() const = 0;
  virtual std::string toString() const = 0;
};

/* declaration is statement */
class AstDeclaration : public AstStatement {
public:
  virtual ~AstDeclaration() = default;
  virtual int type() const = 0;
  virtual std::string toString() const = 0;
};

namespace detail {

template <class T> class AstList : public Ast {
public:
  AstList() {}
  virtual ~AstList() {
    for (int i = 0; i < (int)items_.size(); i++) {
      delete items_[i];
      items_[i] = nullptr;
    }
    items_.clear();
  }
  virtual int type() const = 0;
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
        ss << ", ";
      }
    }
    ss << " ]";
    return ss.str();
  }
  virtual int size() const { return items_.size(); }
  virtual T *get(int pos) const { return items_[pos]; }
  virtual void add(T *item) { items_.push_back(item); }

protected:
  virtual std::string stringify() const = 0;

  std::vector<T *> items_;
};

} // namespace detail

class AstExpressionList : public detail::AstList<AstExpression> {
public:
  AstExpressionList() = default;
  virtual ~AstExpressionList() = default;
  virtual int type() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

private:
  virtual std::string stringify() const;
};

class AstStatementList : public detail::AstList<AstStatement> {
public:
  AstStatementList() = default;
  virtual ~AstStatementList() = default;
  virtual int type() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

private:
  virtual std::string stringify() const;
};

class AstDeclarationList : public detail::AstList<AstDeclaration> {
public:
  AstDeclarationList() = default;
  virtual ~AstDeclarationList() = default;
  virtual int type() const;
  // virtual std::string toString() const;
  // virtual int size() const;
  // virtual T *get(int pos) const;
  // virtual void add(T *item);

  static AstDeclarationList *program();
  static AstDeclarationList *
  resetProgram(AstDeclarationList *program = nullptr);

private:
  static AstDeclarationList *program_;

  virtual std::string stringify() const;
};

/* constant expression - T_IDENTIFIER */
class AstIdentifierConstant : public AstExpression {
public:
  AstIdentifierConstant(const char *value);
  virtual ~AstIdentifierConstant();
  virtual int type() const;
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
  virtual int type() const;
  virtual std::string toString() const;

  virtual const int8_t &value() const;

private:
  int8_t value_;
};

/* constant expression - T_UI8_CONSTANT */
class AstUI8Constant : public AstExpression {
public:
  AstUI8Constant(const uint8_t &value);
  virtual ~AstUI8Constant();
  virtual int type() const;
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
  virtual int type() const;
  virtual std::string toString() const;

  virtual const int16_t &value() const;

private:
  int16_t value_;
};

/* constant expression - T_UI16_CONSTANT */
class AstUI16Constant : public AstExpression {
public:
  AstUI16Constant(const uint16_t &value);
  virtual ~AstUI16Constant();
  virtual int type() const;
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
  virtual int type() const;
  virtual std::string toString() const;

  virtual const int32_t &value() const;

private:
  int32_t value_;
};

/* constant expression - T_UI32_CONSTANT */
class AstUI32Constant : public AstExpression {
public:
  AstUI32Constant(const uint32_t &value);
  virtual ~AstUI32Constant();
  virtual int type() const;
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
  virtual int type() const;
  virtual std::string toString() const;

  virtual const int64_t &value() const;

private:
  int64_t value_;
};

/* constant expression - T_UI64_CONSTANT */
class AstUI64Constant : public AstExpression {
public:
  AstUI64Constant(const uint64_t &value);
  virtual ~AstUI64Constant();
  virtual int type() const;
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
  virtual int type() const;
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
  virtual int type() const;
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
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;
  virtual void append(const char *value);

private:
  std::string value_;
};

/* constant expression - T_TRUE T_FALSE */
class AstBooleanConstant : public AstExpression {
public:
  AstBooleanConstant(const bool &value);
  virtual ~AstBooleanConstant();
  virtual int type() const;
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
  virtual int type() const;
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
  virtual int type() const;
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
  virtual int type() const;
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
  AstConditionalExpression(AstExpression *condition, AstExpression *left,
                           AstExpression *right);
  virtual ~AstConditionalExpression();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstExpression *condition() const;
  virtual AstExpression *left() const;
  virtual AstExpression *right() const;

private:
  AstExpression *condition_;
  AstExpression *left_;
  AstExpression *right_;
};

/* assignment expression */
class AstAssignmentExpression : public AstExpression {
public:
  AstAssignmentExpression(AstExpression *left, int token, AstExpression *right);
  virtual ~AstAssignmentExpression();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstExpression *left() const;
  virtual int token() const;
  virtual AstExpression *right() const;

private:
  AstExpression *left_;
  int token_;
  AstExpression *right_;
};

/* expression statement */
class AstExpressionStatement : public AstStatement {
public:
  AstExpressionStatement(AstExpression *expression);
  virtual ~AstExpressionStatement();
  virtual int type() const;
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
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstStatementList *statementList() const;

private:
  AstStatementList *statementList_;
};

/* selection statement - if else */
class AstIfStatement : public AstStatement {
public:
  AstIfStatement(AstExpression *condition, AstStatement *left,
                 AstStatement *right);
  virtual ~AstIfStatement();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstExpression *condition() const;
  virtual AstStatement *left() const;
  virtual AstStatement *right() const;

private:
  AstExpression *condition_;
  AstStatement *left_;
  AstStatement *right_;
};

/* iteration statement - while */
class AstWhileStatement : public AstStatement {
public:
  AstWhileStatement(AstExpression *condition, AstStatement *statement);
  virtual ~AstWhileStatement();
  virtual int type() const;
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
  AstForStatement(AstStatement *initialize, AstStatement *condition,
                  AstExpression *post, AstStatement *statement);
  virtual ~AstForStatement();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstStatement *initialize() const;
  virtual AstStatement *condition() const;
  virtual AstExpression *post() const;
  virtual AstStatement *statement() const;

private:
  AstStatement *initialize_;
  AstStatement *condition_;
  AstExpression *post_;
  AstStatement *statement_;
};

/* jump statement - continue */
class AstContinueStatement : public AstStatement {
public:
  AstContinueStatement();
  virtual ~AstContinueStatement();
  virtual int type() const;
  virtual std::string toString() const;
};

/* jump statement - break */
class AstBreakStatement : public AstStatement {
public:
  AstBreakStatement();
  virtual ~AstBreakStatement();
  virtual int type() const;
  virtual std::string toString() const;
};

/* jump statement - return */
class AstReturnStatement : public AstStatement {
public:
  AstReturnStatement(AstExpression *expression);
  virtual ~AstReturnStatement();
  virtual int type() const;
  virtual std::string toString() const;
  virtual AstExpression *expression() const;

private:
  AstExpression *expression_;
};

/* variable declaration */
class AstVariableDeclaration : public AstDeclaration {
public:
  AstVariableDeclaration(AstDeclarationList *declarationList);
  virtual ~AstVariableDeclaration();
  virtual int type() const;
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
  virtual int type() const;
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
  virtual int type() const;
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
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;

private:
  std::string value_;
};
