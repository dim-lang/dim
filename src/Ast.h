// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

/* Ast::type */

#define FA_IDENTIFIER_CONSTANT 101
#define FA_INTEGER_CONSTANT 102
#define FA_DOUBLE_CONSTANT 103
#define FA_STRING_CONSTANT 104
#define FA_FUNCTION_CALL_EXPRESSION 105
#define FA_UNARY_OPERATION_EXPRESSION 106
#define FA_BINARY_OPERATION_EXPRESSION 107
#define FA_CONDITIONAL_EXPRESSION 108

#define FA_EXPRESSION_STATEMENT 201
#define FA_COMPOUND_STATEMENT 202
#define FA_IF_STATEMENT 203
#define FA_WHILE_STATEMENT 204
#define FA_FOR_STATEMENT 205
#define FA_CONTINUE_STATEMENT 206
#define FA_BREAK_STATEMENT 207
#define FA_RETURN_STATEMENT 207

#define FA_VARIABLE_DECLARATIONS 301
#define FA_VARIABLE_DECLARATION 302
#define FA_FUNCTION_DECLARATION 303

class AstExpression;
class AstStatement;
class AstVariableDeclaration;
using AstExpressionList = std::vector<AstExpression *>;
using AstStatementList = std::vector<AstStatement *>;
using AstVariableDeclarationList = std::vector<AstVariableDeclaration *>;
using AstStringList = std::vector<std::string>;

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

/* declaration */
class AstDeclaration : public Ast {
public:
  virtual ~AstDeclaration() = default;
  virtual int type() const = 0;
  virtual std::string toString() const = 0;
};

/* constant expression - FT_IDENTIFIER */
class AstIdentifierConstant : public AstExpression {
public:
  AstIdentifierConstant(const std::string &value);
  virtual ~AstIdentifierConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;

private:
  std::string value_;
};

/* constant expression - FT_INTEGER */
class AstIntegerConstant : public AstExpression {
public:
  AstIntegerConstant(const int64_t &value);
  virtual ~AstIntegerConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const int64_t &value() const;

private:
  int64_t value_;
};

/* constant expression - FT_DOUBLE */
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

/* constant expression - FT_STRING */
class AstStringConstant : public AstExpression {
public:
  AstStringConstant(const std::string &value);
  virtual ~AstStringConstant();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;

private:
  std::string value_;
};

/* function call expression */
class AstFunctionCallExpression : public AstExpression {
public:
  AstFunctionCallExpression(const std::string &identifier,
                            const AstExpressionList &expressionList);
  virtual ~AstFunctionCallExpression();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual const AstExpressionList &argumentList() const;

private:
  std::string identifier_;
  AstExpressionList argumentList_;
};

/* unary operation expression */
class AstUnaryOperationExpression : public AstExpression {
public:
  AstUnaryOperationExpression(int token, AstExpression *expression);
  virtual ~AstUnaryOperationExpression();
  virtual int type() const;
  virtual std::string toString() const;

  virtual int token() const;
  virtual AstExpression *expression() const;

private:
  int token_;
  AstExpression *expression_;
};

/* binary operation expression */
class AstBinaryOperationExpression : public AstExpression {
public:
  AstBinaryOperationExpression(AstExpression *left, int token,
                               AstExpression *right);
  virtual ~AstBinaryOperationExpression();
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
  AstConditionalExpression(AstExpression *conditionExpression,
                           AstExpression *ifExpression,
                           AstExpression *elseExpression);
  virtual ~AstConditionalExpression();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstExpression *conditionExpression() const;
  virtual AstExpression *ifExpression() const;
  virtual AstExpression *elseExpression() const;

private:
  AstExpression *conditionExpression_;
  AstExpression *ifExpression_;
  AstExpression *elseExpression_;
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
  AstCompoundStatement(const AstStatementList &statementList);
  virtual ~AstCompoundStatement();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const AstStatementList &statementList() const;

private:
  AstStatementList statementList_;
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
  AstForStatement(AstExpression *initExpression,
                  AstExpression *conditionExpression,
                  AstExpression *postExpression, AstStatement *statement);
  virtual ~AstForStatement();
  virtual int type() const;
  virtual std::string toString() const;

  virtual AstExpression *initExpression() const;
  virtual AstExpression *conditionExpression() const;
  virtual AstExpression *postExpression() const;
  virtual AstStatement *statement() const;

private:
  AstExpression *initExpression_;
  AstExpression *conditionExpression_;
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

/* multiple variable declarations */
class AstVariableDeclarations : public AstDeclaration {
public:
  AstVariableDeclarations(const AstVariableDeclarationList &declarationList);
  virtual ~AstVariableDeclarations();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const AstVariableDeclarationList &declarationList() const;

private:
  AstVariableDeclarationList declarationList_;
};

/* single variable declaration */
class AstVariableDeclaration : public AstDeclaration {
public:
  AstVariableDeclaration(const std::string &identifier,
                         AstExpression *expression);
  virtual ~AstVariableDeclaration();
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
  AstFunctionDeclaration(const std::string &identifier,
                         const AstStringList &argumentList,
                         AstCompoundStatement *compoundStatement);
  virtual ~AstFunctionDeclaration();
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual const AstStringList &argumentList() const;
  virtual AstCompoundStatement *compoundStatement() const;

private:
  std::string identifier_;
  AstStringList argumentList_;
  AstCompoundStatement *compoundStatement_;
};
