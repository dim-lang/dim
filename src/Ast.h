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
  AstStringLiteral(std::string value) : value(value) {}
};

class AstBoolean : public AstExpression {
public:
  bool value;
  AstBoolean(bool value) : value(value) {}
};

class AstIdentifier : public AstExpression {
public:
  std::string name;
  AstIdentifier(const std::string &name) : name(name) {}
};

class AstMethodCall : public AstExpression {
public:
  std::shared_ptr<AstIdentifier> id;
  std::shared_ptr<std::vector<std::shared_ptr<AstExpression>>> argumentList;
  AstMethodCall(
      std::shared_ptr<AstIdentifier> id,
      std::shared_ptr<std::vector<std::shared_ptr<AstExpression>>> argumentList)
      : id(id), argumentList(argumentList) {}
  AstMethodCall(std::shared_ptr<AstIdentifier> id)
      : id(id), argumentList(nullptr) {}
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
  std::shared_ptr<AstIdentifier> lhs;
  std::shared_ptr<AstExpression> rhs;
  AstAssignment(std::shared_ptr<AstIdentifier> lhs,
                std::shared_ptr<AstExpression> rhs)
      : lhs(lhs), rhs(rhs) {}
};

class AstBlock : public AstExpression {
public:
  std::shared_ptr<std::vector<std::shared_ptr<AstStatement>>> statementList;
  AstBlock()
      : statementList(new std::vector<std::shared_ptr<AstStatement>>()) {}
};

class AstExpressionStatement : public AstStatement {
public:
  std::shared_ptr<AstExpression> expression;
  AstExpressionStatement(std::shared_ptr<AstExpression> expression)
      : expression(expression) {}
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
  std::shared_ptr<AstIdentifier> type;
  std::shared_ptr<AstIdentifier> id;
  std::shared_ptr<std::vector<std::shared_ptr<AstVariableDeclaration>>>
      argumentList;
  std::shared_ptr<AstBlock> block;
  AstFunctionDeclaration(
      std::shared_ptr<AstIdentifier> type, std::shared_ptr<AstIdentifier> id,
      std::shared_ptr<std::vector<std::shared_ptr<AstVariableDeclaration>>>
          argumentList,
      std::shared_ptr<AstBlock> block)
      : type(type), id(id), argumentList(argumentList), block(block) {}
};

} // namespace fastype
