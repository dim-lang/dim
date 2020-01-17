// Copyright 2019- <fastype.org>
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

#define FA_VARIABLE_DECLARATION 301
#define FA_FUNCTION_DECLARATION 302

class AstExpression;
class AstStatement;
using AstExpressionList = std::vector<std::shared_ptr<AstExpression>>;
using AstStatementList = std::vector<std::shared_ptr<AstStatement>>;
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
  virtual ~AstIdentifierConstant() = default;
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
  virtual ~AstIntegerConstant() = default;
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
  virtual ~AstDoubleConstant() = default;
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
  virtual ~AstStringConstant() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &value() const;

private:
  std::string value_;
};

/* function call expression */
class AstFunctionCallExpresion : public AstExpression {
public:
  AstFunctionCallExpresion(const std::string &value);
  virtual ~AstFunctionCallExpresion() = default;
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
  AstUnaryOperationExpression(int token,
                              std::shared_ptr<AstExpression> expression);
  virtual ~AstUnaryOperationExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual int token() const;
  virtual std::shared_ptr<AstExpression> expression() const;

private:
  int token_;
  std::shared_ptr<AstExpression> expression_;
};

/* binary operation expression */
class AstBinaryOperationExpression : public AstExpression {
public:
  AstBinaryOperationExpression(std::shared_ptr<AstExpression> left, int token,
                               std::shared_ptr<AstExpression> right);
  virtual ~AstBinaryOperationExpression() = default;
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
  AstConditionalExpression(std::shared_ptr<AstExpression> condition,
                           std::shared_ptr<AstExpression> ifExpression,
                           std::shared_ptr<AstExpression> elseExpression);
  virtual ~AstConditionalExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstExpression> condition() const;
  virtual std::shared_ptr<AstExpression> ifExpression() const;
  virtual std::shared_ptr<AstExpression> elseExpression() const;

private:
  std::shared_ptr<AstExpression> condition_;
  std::shared_ptr<AstExpression> ifExpression_;
  std::shared_ptr<AstExpression> elseExpression_;
};

/* expression statement */
class AstExpressionStatement : public AstStatement {
public:
  AstExpressionStatement(std::shared_ptr<AstExpression> expression);
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
  AstCompoundStatement(std::shared_ptr<AstExpression> expression);
  virtual ~AstCompoundStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual const AstStatementList &statementList() const;

private:
  AstStatementList statementList_;
};

/* selection statement - if else */
class AstIfStatement : public AstStatement {
public:
  AstIfStatement(std::shared_ptr<AstExpression> expression,
                 std::shared_ptr<AstStatement> ifStatement,
                 std::shared_ptr<AstStatement> elseStatement);
  virtual ~AstIfStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstExpression> expression() const;
  virtual std::shared_ptr<AstStatement> ifStatement() const;
  virtual std::shared_ptr<AstStatement> elseStatement() const;

private:
  std::shared_ptr<AstExpression> expression_;
  std::shared_ptr<AstStatement> ifStatement_;
  std::shared_ptr<AstStatement> elseStatement_;
};

/* iteration statement - while */
class AstWhileStatement : public AstStatement {
public:
  AstWhileStatement(std::shared_ptr<AstExpression> expression,
                    std::shared_ptr<AstStatement> statement);
  virtual ~AstWhileStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstExpression> expression() const;
  virtual std::shared_ptr<AstStatement> statement() const;

private:
  std::shared_ptr<AstExpression> expression_;
  std::shared_ptr<AstStatement> statement_;
};

/* iteration statement - for */
class AstForStatement : public AstStatement {
public:
  AstForStatement(std::shared_ptr<AstExpression> initExpression,
                  std::shared_ptr<AstExpression> conditionExpression,
                  std::shared_ptr<AstExpression> postExpression,
                  std::shared_ptr<AstStatement> statement);
  virtual ~AstForStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual std::shared_ptr<AstExpression> initExpression() const;
  virtual std::shared_ptr<AstExpression> conditionExpression() const;
  virtual std::shared_ptr<AstExpression> postExpression() const;
  virtual std::shared_ptr<AstStatement> statement() const;

private:
  std::shared_ptr<AstExpression> initExpression_;
  std::shared_ptr<AstExpression> conditionExpression_;
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
  AstReturnStatement(std::shared_ptr<AstExpression> expression);
  virtual ~AstReturnStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;

private:
  std::shared_ptr<AstExpression> expression_;
};

/* variable declaration */
class AstVariableDeclaration : public AstDeclaration {
public:
  AstVariableDeclaration(const AstStringList &identifierList,
                         const AstExpressionList &expressionList);
  virtual ~AstVariableDeclaration() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual const AstStringList &identifierList() const;
  virtual const AstExpressionList &expressionList() const;

private:
  AstStringList identifierList_;
  AstExpressionList expressionList_;
};

/* function declaration */
class AstFunctionDeclaration : public AstDeclaration {
public:
  AstFunctionDeclaration(
      const std::string &identifier, const AstStringList &argumentList,
      std::shared_ptr<AstCompoundStatement> compoundStatement);
  virtual ~AstFunctionDeclaration() = default;
  virtual int type() const;
  virtual std::string toString() const;

  virtual const std::string &identifier() const;
  virtual const AstStringList &argumentList() const;
  virtual std::shared_ptr<AstCompoundStatement> compoundStatement() const;

private:
  std::string identifier_;
  AstStringList argumentList_;
  std::shared_ptr<AstCompoundStatement> compoundStatement_;
};
