// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace fastype {

/**
 * interface Ast#codeGen
 * interface AstExpression -> Ast
 * interface AstStatement -> Ast
 *
 * AstInteger -> AstExpression
 * AstDouble -> AstExpression
 * AstStringLiteral -> AstExpression
 * AstBoolean -> AstExpression
 * AstIdentifier -> AstExpression
 * AstMethodCall -> AstExpression
 * AstBinaryOperator -> AstExpression
 * AstAssignment -> AstExpression
 * AstBlock -> AstExpression
 *
 * AstExpressionStatement -> AstStatement
 * AstVariableDeclaration -> AstStatement
 * AstFunctionDeclaration -> AstStatement
 */

class AstStatement;
class AstExpression;
class AstVariableDeclaration;

typedef std::vector<AstStatement *> StatementList;
typedef std::vector<AstExpression *> ExpressionList;
typedef std::vector<AstVariableDeclaration *> VariableList;

class Ast {
public:
  virtual ~Ast() = default;
};

class AstExpression : public Ast {};

class AstStatement : public Ast {};

class AstInteger : public AstExpression {
public:
  long long value;
  AstInteger(long long value) : value(value) {}
};

class AstDouble : public AstExpression {
public:
  double value;
  AstDouble(double value) : value(value) {}
};

class AstStringLiteral : public AstExpression {
public:
  std::string value;
  AstDouble(std::string value) : value(value) {}
};

class AstBoolean : public AstExpression {
public:
  bool value;
  AstDouble(bool value) : value(value) {}
};

class AstIdentifier : public AstExpression {
public:
  std::string name;
  AstIdentifier(const std::string &name) : name(name) {}
};

class AstMethodCall : public AstExpression {
public:
  const AstIdentifier &id;
  ExpressionList arguments;
  AstMethodCall(const AstIdentifier &id, ExpressionList &arguments)
      : id(id), arguments(arguments) {}
  AstMethodCall(const AstIdentifier &id) : id(id) {}
};

class AstBinaryOperator : public AstExpression {
public:
  int op;
  AstExpression &lhs;
  AstExpression &rhs;
  AstBinaryOperator(AstExpression &lhs, int op, AstExpression &rhs)
      : lhs(lhs), rhs(rhs), op(op) {}
};

class AstAssignment : public AstExpression {
public:
  AstIdentifier &lhs;
  AstExpression &rhs;
  AstAssignment(AstIdentifier &lhs, AstExpression &rhs) : lhs(lhs), rhs(rhs) {}
};

class AstBlock : public AstExpression {
public:
  StatementList statements;
  AstBlock() {}
};

class AstExpressionStatement : public AstStatement {
public:
  AstExpression &expression;
  AstExpressionStatement(AstExpression &expression) : expression(expression) {}
};

class AstVariableDeclaration : public AstStatement {
public:
  const AstIdentifier &type;
  AstIdentifier &id;
  AstExpression *assignmentExpr;
  AstVariableDeclaration(const AstIdentifier &type, AstIdentifier &id)
      : type(type), id(id) {}
  AstVariableDeclaration(const AstIdentifier &type, AstIdentifier &id,
                         AstExpression *assignmentExpr)
      : type(type), id(id), assignmentExpr(assignmentExpr) {}
};

class AstFunctionDeclaration : public AstStatement {
public:
  const AstIdentifier &type;
  const AstIdentifier &id;
  VariableList arguments;
  AstBlock &block;
  AstFunctionDeclaration(const AstIdentifier &type, const AstIdentifier &id,
                         const VariableList &arguments, AstBlock &block)
      : type(type), id(id), arguments(arguments), block(block) {}
};

} // namespace fastype
