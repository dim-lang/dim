// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

class IrDefinition;
class IrStatement;
class IrExpression;
class IrContext;
class Ast;

class IrFactory {
public:
  static IrStatement *stmt(IrContext *context, Ast *node);
  static IrExpression *expr(IrContext *context, Ast *node);
  static IrDefinition *unit(IrContext *context, Ast *node);
};

