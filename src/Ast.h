// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

/* Ast::type */

#define A_IDENTIFIER_CONSTANT 101
#define A_FLOAT_CONSTANT 102
#define A_DOUBLE_CONSTANT 103
#define A_STRING_CONSTANT 104
#define A_FUNCTION_CALL_EXPRESSION 105
#define A_UNARY_OPERATION_EXPRESSION 106
#define A_BINARY_OPERATION_EXPRESSION 107
#define A_CONDITIONAL_EXPRESSION 108
#define A_BYTE_CONSTANT 109
#define A_UBYTE_CONSTANT 110
#define A_SHORT_CONSTANT 111
#define A_USHORT_CONSTANT 112
#define A_INT_CONSTANT 113
#define A_UINT_CONSTANT 114
#define A_LONG_CONSTANT 115
#define A_ULONG_CONSTANT 116
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

class AstExpression;
class AstStatement;
using AstExpressionList = std::vector<AstExpression *>;
using AstStatementList = std::vector<AstStatement *>;

class Ast : public Stringify {
public:
  virtual ~Ast() = default;
  virtual int type() const = 0;
  virtual std::string toString() const = 0;

  static std::string dump(const AstExpressionList *expressionList);
  static std::string dump(const AstStatementList *statementList);
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

/* constant expression - T_BYTE_CONSTANT */
class AstByteConstant : public AstExpression {
public:
  AstByteConstant(const int8_t &value);
  virtual ~AstByteConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const int8_t &value() const;

private:
  int8_t value_;
};

/* constant expression - T_UBYTE_CONSTANT */
class AstUByteConstant : public AstExpression {
public:
  AstUByteConstant(const uint8_t &value);
  virtual ~AstUByteConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const uint8_t &value() const;

private:
  uint8_t value_;
};

/* constant expression - T_SHORT_CONSTANT */
class AstShortConstant : public AstExpression {
public:
  AstShortConstant(const int16_t &value);
  virtual ~AstShortConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const int16_t &value() const;

private:
  int16_t value_;
};

/* constant expression - T_USHORT_CONSTANT */
class AstUShortConstant : public AstExpression {
public:
  AstUShortConstant(const uint16_t &value);
  virtual ~AstUShortConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const uint16_t &value() const;

private:
  uint16_t value_;
};

/* constant expression - T_INT_CONSTANT */
class AstIntConstant : public AstExpression {
public:
  AstIntConstant(const int32_t &value);
  virtual ~AstIntConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const int32_t &value() const;

private:
  int32_t value_;
};

/* constant expression - T_UINT_CONSTANT */
class AstUIntConstant : public AstExpression {
public:
  AstUIntConstant(const uint32_t &value);
  virtual ~AstUIntConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const uint32_t &value() const;

private:
  uint32_t value_;
};

/* constant expression - T_LONG_CONSTANT */
class AstLongConstant : public AstExpression {
public:
  AstLongConstant(const int64_t &value);
  virtual ~AstLongConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const int64_t &value() const;

private:
  int64_t value_;
};

/* constant expression - T_ULONG_CONSTANT */
class AstULongConstant : public AstExpression {
public:
  AstULongConstant(const uint64_t &value);
  virtual ~AstULongConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const uint64_t &value() const;

private:
  uint64_t value_;
};

/* constant expression - T_FLOAT_CONSTANT */
class AstFloatConstant : public AstExpression {
public:
  AstFloatConstant(const float &value);
  virtual ~AstFloatConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const float &value() const;

private:
  float value_;
};

/* constant expression - T_DOUBLE_CONSTANT */
class AstDoubleConstant : public AstExpression {
public:
  AstDoubleConstant(const double &value);
  virtual ~AstDoubleConstant();
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
class AstFunctionCallExpression : public AstExpression {
public:
  AstFunctionCallExpression(const char *identifier,
                            AstExpressionList *expressionList);
  virtual ~AstFunctionCallExpression();
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
  AstConditionalExpression(AstExpression *condExpression,
                           AstExpression *ifExpression,
                           AstExpression *elseExpression);
  virtual ~AstConditionalExpression();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstExpression *condExpression() const;
  virtual AstExpression *ifExpression() const;
  virtual AstExpression *elseExpression() const;

private:
  AstExpression *condExpression_;
  AstExpression *ifExpression_;
  AstExpression *elseExpression_;
};

/* assignment expression */
class AstAssignmentExpression : public AstExpression {
public:
  AstAssignmentExpression(AstExpressionList *left, int token,
                          AstExpressionList *right);
  virtual ~AstAssignmentExpression();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstExpressionList *left() const;
  virtual int token() const;
  virtual AstExpressionList *right() const;

private:
  AstExpressionList *left_;
  int token_;
  AstExpressionList *right_;
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
  virtual AstStatementList *releaseStatementList();

private:
  AstStatementList *statementList_;
};

/* selection statement - if else */
class AstIfStatement : public AstStatement {
public:
  AstIfStatement(AstExpression *expression, AstStatement *ifStatement,
                 AstStatement *elseStatement);
  virtual ~AstIfStatement();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstExpression *expression() const;
  virtual AstStatement *ifStatement() const;
  virtual AstStatement *elseStatement() const;

private:
  AstExpression *expression_;
  AstStatement *ifStatement_;
  AstStatement *elseStatement_;
};

/* iteration statement - while */
class AstWhileStatement : public AstStatement {
public:
  AstWhileStatement(AstExpression *expression, AstStatement *statement);
  virtual ~AstWhileStatement();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstExpression *expression() const;
  virtual AstStatement *statement() const;

private:
  AstExpression *expression_;
  AstStatement *statement_;
};

/* iteration statement - for */
class AstForStatement : public AstStatement {
public:
  AstForStatement(AstStatement *initStatement, AstStatement *condStatement,
                  AstExpression *postExpression, AstStatement *statement);
  virtual ~AstForStatement();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstStatement *initStatement() const;
  virtual AstStatement *condStatement() const;
  virtual AstExpression *postExpression() const;
  virtual AstStatement *statement() const;

private:
  AstStatement *initStatement_;
  AstStatement *condStatement_;
  AstExpression *postExpression_;
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
  AstVariableDeclaration(AstExpressionList *identifierList,
                         AstExpressionList *expressionList);
  virtual ~AstVariableDeclaration();
  virtual int type() const;
  virtual std::string toString() const;
  virtual AstExpressionList *identifierList() const;
  virtual AstExpressionList *expressionList() const;

private:
  AstExpressionList *identifierList_;
  AstExpressionList *expressionList_;
};

/* function declaration */
class AstFunctionDeclaration : public AstDeclaration {
public:
  AstFunctionDeclaration(const char *identifier,
                         AstExpressionList *argumentList,
                         AstStatementList *statementList);
  virtual ~AstFunctionDeclaration();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual AstExpressionList *argumentList() const;
  virtual AstStatementList *statementList() const;

private:
  std::string identifier_;
  AstExpressionList *argumentList_;
  AstStatementList *statementList_;
};
