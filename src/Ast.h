// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

/* Ast::type */

#define A_IDENTIFIER_CONSTANT 101
#define A_F32_CONSTANT 102
#define A_F64_CONSTANT 103
#define A_STRING_CONSTANT 104
#define A_FUNCTION_CALL_EXPRESSION 105
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

#define A_EXPRESSION_STATEMENT 201
#define A_COMPOUND_STATEMENT 202
#define A_IF_STATEMENT 203
#define A_WHILE_STATEMENT 204
#define A_FOR_STATEMENT 205
#define A_CONTINUE_STATEMENT 206
#define A_BREAK_STATEMENT 207
#define A_RETURN_STATEMENT 207

#define A_VARIABLE_DECLARATIONS 301
#define A_VARIABLE_DECLARATION 302
#define A_FUNCTION_DECLARATION 303

#define A_EXPRESSION_LIST 401
#define A_STATEMENT_LIST 402

/* ================================== */

/* interface */
class Ast;

/* derived interface */
class AstExpression;
class AstStatement;
class AstDeclaration;

/* interface list */
class AstExpressionList;
class AstStatementList;

/* node */
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
class AstFunctionCallExpression;
class AstUnaryExpression;
class AstBinaryExpression;
class AstConditionalExpression;
class AstAssignmentExpression;
class AstExpressionStatement;
class AstCompoundStatement;
class AstIfStatement;
class AstWhileStatement;
class AstForStatement;
class AstContinueStatement;
class AstBreakStatement;
class AstReturnStatement;
class AstVariableDeclaration;
class AstFunctionDeclaration;

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

class AstExpressionList : public Ast {
public:
  AstExpressionList();
  virtual ~AstExpressionList() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual int size() const;
  virtual std::shared_ptr<AstExpression> get(int pos) const;
  virtual void add(AstExpression *expression);

private:
  std::vector<std::shared_ptr<AstExpression>> expressions_;
};

class AstStatementList : public Ast {
public:
  AstStatementList();
  virtual ~AstStatementList() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual int size() const;
  virtual std::shared_ptr<AstStatement> get(int pos) const;
  virtual void add(AstStatement *statement);

private:
  std::vector<std::shared_ptr<AstStatement>> statements_;
};

/* constant expression - T_IDENTIFIER */
class AstIdentifierConstant : public AstExpression {
public:
  AstIdentifierConstant(const char *value);
  virtual ~AstIdentifierConstant() = default;
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
  virtual ~AstI8Constant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const int8_t &value);
  virtual const int8_t &value() const;

private:
  int8_t value_;
};

/* constant expression - T_UI8_CONSTANT */
class AstUI8Constant : public AstExpression {
public:
  AstUI8Constant(const uint8_t &value);
  virtual ~AstUI8Constant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const uint8_t &value);
  virtual const uint8_t &value() const;

private:
  uint8_t value_;
};

/* constant expression - T_I16_CONSTANT */
class AstI16Constant : public AstExpression {
public:
  AstI16Constant(const int16_t &value);
  virtual ~AstI16Constant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const int16_t &value);
  virtual const int16_t &value() const;

private:
  int16_t value_;
};

/* constant expression - T_UI16_CONSTANT */
class AstUI16Constant : public AstExpression {
public:
  AstUI16Constant(const uint16_t &value);
  virtual ~AstUI16Constant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const uint16_t &value);
  virtual const uint16_t &value() const;

private:
  uint16_t value_;
};

/* constant expression - T_I32_CONSTANT */
class AstI32Constant : public AstExpression {
public:
  AstI32Constant(const int32_t &value);
  virtual ~AstI32Constant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const int32_t &value);
  virtual const int32_t &value() const;

private:
  int32_t value_;
};

/* constant expression - T_UI32_CONSTANT */
class AstUI32Constant : public AstExpression {
public:
  AstUI32Constant(const uint32_t &value);
  virtual ~AstUI32Constant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const uint32_t &value);
  virtual const uint32_t &value() const;

private:
  uint32_t value_;
};

/* constant expression - T_I64_CONSTANT */
class AstI64Constant : public AstExpression {
public:
  AstI64Constant(const int64_t &value);
  virtual ~AstI64Constant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const int64_t &value);
  virtual const int64_t &value() const;

private:
  int64_t value_;
};

/* constant expression - T_UI64_CONSTANT */
class AstUI64Constant : public AstExpression {
public:
  AstUI64Constant(const uint64_t &value);
  virtual ~AstUI64Constant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const uint64_t &value);
  virtual const uint64_t &value() const;

private:
  uint64_t value_;
};

/* constant expression - T_F32_CONSTANT */
class AstF32Constant : public AstExpression {
public:
  AstF32Constant(const float &value);
  virtual ~AstF32Constant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const float &value);
  virtual const float &value() const;

private:
  float value_;
};

/* constant expression - T_F64_CONSTANT */
class AstF64Constant : public AstExpression {
public:
  AstF64Constant(const double &value);
  virtual ~AstF64Constant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const double &value);
  virtual const double &value() const;

private:
  double value_;
};

/* constant expression - T_STRING_CONSTANT */
class AstStringConstant : public AstExpression {
public:
  AstStringConstant(const char *value);
  virtual ~AstStringConstant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const std::string &value);
  virtual const std::string &value() const;

private:
  std::string value_;
};

/* constant expression - T_TRUE T_FALSE */
class AstBooleanConstant : public AstExpression {
public:
  AstBooleanConstant(const bool &value);
  virtual ~AstBooleanConstant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual void reset(const bool &value);
  virtual const bool &value() const;

private:
  bool value_;
};

/* function call expression */
class AstFunctionCallExpression : public AstExpression {
public:
  AstFunctionCallExpression(const char *identifier,
                            AstExpressionList *expressionList);
  virtual ~AstFunctionCallExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual std::shared_ptr<AstExpressionList> argumentList() const;

private:
  std::string identifier_;
  std::shared_ptr<AstExpressionList> argumentList_;
};

/* unary operation expression */
class AstUnaryExpression : public AstExpression {
public:
  AstUnaryExpression(int token, AstExpression *expression);
  virtual ~AstUnaryExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual int token() const;
  virtual std::shared_ptr<AstExpression> expression() const;

private:
  int token_;
  std::shared_ptr<AstExpression> expression_;
};

/* binary operation expression */
class AstBinaryExpression : public AstExpression {
public:
  AstBinaryExpression(AstExpression *left, int token, AstExpression *right);
  virtual ~AstBinaryExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstExpression> left() const;
  virtual int token() const;
  virtual std::shared_ptr<AstExpression> right() const;

private:
  std::shared_ptr<AstExpression> left_;
  int token_;
  std::shared_ptr<AstExpression> right_;
};

/* ternary conditional expression */
class AstConditionalExpression : public AstExpression {
public:
  AstConditionalExpression(AstExpression *condExpression,
                           AstExpression *ifExpression,
                           AstExpression *elseExpression);
  virtual ~AstConditionalExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstExpression> condExpression() const;
  virtual std::shared_ptr<AstExpression> ifExpression() const;
  virtual std::shared_ptr<AstExpression> elseExpression() const;

private:
  std::shared_ptr<AstExpression> condExpression_;
  std::shared_ptr<AstExpression> ifExpression_;
  std::shared_ptr<AstExpression> elseExpression_;
};

/* assignment expression */
class AstAssignmentExpression : public AstExpression {
public:
  AstAssignmentExpression(AstExpressionList *left, int token,
                          AstExpressionList *right);
  virtual ~AstAssignmentExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstExpressionList> left() const;
  virtual int token() const;
  virtual std::shared_ptr<AstExpressionList> right() const;

private:
  std::shared_ptr<AstExpressionList> left_;
  int token_;
  std::shared_ptr<AstExpressionList> right_;
};

/* expression statement */
class AstExpressionStatement : public AstStatement {
public:
  AstExpressionStatement(AstExpression *expression);
  virtual ~AstExpressionStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstExpression> expression() const;

private:
  std::shared_ptr<AstExpression> expression_;
};

/* compound statement - { ... } */
class AstCompoundStatement : public AstStatement {
public:
  AstCompoundStatement(AstStatementList *statementList);
  virtual ~AstCompoundStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstStatementList> statementList() const;

private:
  std::shared_ptr<AstStatementList> statementList_;
};

/* selection statement - if else */
class AstIfStatement : public AstStatement {
public:
  AstIfStatement(AstExpression *condExpression, AstStatement *ifStatement,
                 AstStatement *elseStatement);
  virtual ~AstIfStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstExpression> condExpression() const;
  virtual std::shared_ptr<AstStatement> ifStatement() const;
  virtual std::shared_ptr<AstStatement> elseStatement() const;

private:
  std::shared_ptr<AstExpression> condExpression_;
  std::shared_ptr<AstStatement> ifStatement_;
  std::shared_ptr<AstStatement> elseStatement_;
};

/* iteration statement - while */
class AstWhileStatement : public AstStatement {
public:
  AstWhileStatement(AstExpression *condExpression, AstStatement *statement);
  virtual ~AstWhileStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstExpression> condExpression() const;
  virtual std::shared_ptr<AstStatement> statement() const;

private:
  std::shared_ptr<AstExpression> condExpression_;
  std::shared_ptr<AstStatement> statement_;
};

/* iteration statement - for */
class AstForStatement : public AstStatement {
public:
  AstForStatement(AstStatement *initStatement, AstStatement *condStatement,
                  AstExpression *postExpression, AstStatement *statement);
  virtual ~AstForStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstStatement> initStatement() const;
  virtual std::shared_ptr<AstStatement> condStatement() const;
  virtual std::shared_ptr<AstExpression> postExpression() const;
  virtual std::shared_ptr<AstStatement> statement() const;

private:
  std::shared_ptr<AstStatement> initStatement_;
  std::shared_ptr<AstStatement> condStatement_;
  std::shared_ptr<AstExpression> postExpression_;
  std::shared_ptr<AstStatement> statement_;
};

/* jump statement - continue */
class AstContinueStatement : public AstStatement {
public:
  AstContinueStatement();
  virtual ~AstContinueStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
};

/* jump statement - break */
class AstBreakStatement : public AstStatement {
public:
  AstBreakStatement();
  virtual ~AstBreakStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
};

/* jump statement - return */
class AstReturnStatement : public AstStatement {
public:
  AstReturnStatement(AstExpression *expression);
  virtual ~AstReturnStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::shared_ptr<AstExpression> expression() const;

private:
  std::shared_ptr<AstExpression> expression_;
};

/* variable declaration */
class AstVariableDeclaration : public AstDeclaration {
public:
  AstVariableDeclaration(AstExpressionList *identifierList,
                         AstExpressionList *expressionList);
  virtual ~AstVariableDeclaration() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::shared_ptr<AstExpressionList> identifierList() const;
  virtual std::shared_ptr<AstExpressionList> expressionList() const;

private:
  std::shared_ptr<AstExpressionList> identifierList_;
  std::shared_ptr<AstExpressionList> expressionList_;
};

/* function declaration */
class AstFunctionDeclaration : public AstDeclaration {
public:
  AstFunctionDeclaration(const char *identifier,
                         AstExpressionList *argumentList,
                         AstStatementList *statementList);
  AstFunctionDeclaration(const char *identifier,
                         AstExpressionList *argumentList,
                         std::shared_ptr<AstStatementList> statementList);
  virtual ~AstFunctionDeclaration() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual std::shared_ptr<AstExpressionList> argumentList() const;
  virtual std::shared_ptr<AstStatementList> statementList() const;

private:
  std::string identifier_;
  std::shared_ptr<AstExpressionList> argumentList_;
  std::shared_ptr<AstStatementList> statementList_;
};
