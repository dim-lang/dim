// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrFactory.h"
#include "Ast.h"
#include "Dump.h"
#include "Ir.h"
#include "Log.h"

#define DC(x, y) dynamic_cast<x *>(y)

IrExpression *IrFactory::expr(IrContext *context, Ast *node) {
  LOG_ASSERT(context, "context is null");
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case A_ty::Identifier:
    return new IrIdentifier(context, DC(AstIdentifier, node));
  case A_ty::Float32Literal:
    return new IrFloat32Constant(context, DC(AstFloat32Literal, node));
  case A_ty::Float64Literal:
    return new IrFloat64Constant(context, DC(AstFloat64Literal, node));
  case A_ty::StringLiteral:
    return new IrStringConstant(context, DC(AstStringLiteral, node));
  case A_ty::Int8Literal:
    return new IrInt8Constant(context, DC(AstInt8Literal, node));
  case A_ty::UInt8Literal:
    return new IrUInt8Constant(context, DC(AstUInt8Literal, node));
  case A_ty::Int16Literal:
    return new IrInt16Constant(context, DC(AstInt16Literal, node));
  case A_ty::UInt16Literal:
    return new IrUInt16Constant(context, DC(AstUInt16Literal, node));
  case A_ty::Int32Literal:
    return new IrInt32Constant(context, DC(AstInt32Literal, node));
  case A_ty::UInt32Literal:
    return new IrUInt32Constant(context, DC(AstUInt32Literal, node));
  case A_ty::Int64Literal:
    return new IrInt64Constant(context, DC(AstInt64Literal, node));
  case A_ty::UInt64Literal:
    return new IrUInt64Constant(context, DC(AstUInt64Literal, node));
  case A_ty::BooleanLiteral:
    return new IrBooleanConstant(context, DC(AstBooleanLiteral, node));
  case A_ty::AssignmentExpression:
    return new IrAssignmentExpression(context,
                                      DC(AstAssignmentExpression, node));
  case A_ty::SequelExpression:
    return new IrSequelExpression(context, DC(AstSequelExpression, node));
  case A_ty::CallExpression:
    return new IrCallExpression(context, DC(AstCallExpression, node));
  case A_ty::UnaryExpression:
    return new IrUnaryExpression(context, DC(AstUnaryExpression, node));
  case A_ty::BinaryExpression:
    return new IrBinaryExpression(context, DC(AstBinaryExpression, node));
  case A_ty::ConditionalExpression:
    return new IrConditionalExpression(context,
                                       DC(AstConditionalExpression, node));
  case A_ty::VoidExpression:
    return new IrVoidExpression(context, DC(AstVoidExpression, node));
  default:
    LOG_ASSERT(false, "invalid ast node:{}, source:{}", node->toString(),
               dumpSource(context->sourceName, node->position()));
  }
  return nullptr;
}

IrStatement *IrFactory::stmt(IrContext *context, Ast *node) {
  LOG_ASSERT(context, "context is null");
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case A_ty::ExpressionStatement:
    return new IrExpressionStatement(context, DC(AstExpressionStatement, node));
  case A_ty::CompoundStatement:
    return new IrCompoundStatement(context, DC(AstCompoundStatement, node));
  case A_ty::IfStatement:
    return new IrIfStatement(context, DC(AstIfStatement, node));
  case A_ty::WhileStatement:
    return new IrWhileStatement(context, DC(AstWhileStatement, node));
  case A_ty::ForStatement:
    return new IrForStatement(context, DC(AstForStatement, node));
  case A_ty::ReturnStatement:
    return new IrReturnStatement(context, DC(AstReturnStatement, node));
  case A_ty::VariableDefinition:
    return new IrLocalVariableDefinition(context,
                                         DC(AstVariableDefinition, node));
  case A_ty::FunctionDefinition:
    return new IrFunctionDefinition(context, DC(AstFunctionDefinition, node));
  case A_ty::FunctionSignatureDefinition:
    return new IrFunctionSignatureDefinition(
        context, DC(AstFunctionSignatureDefinition, node));
  case A_ty::ContinueStatement:
  case A_ty::BreakStatement:
  case A_ty::EmptyStatement:
  default:
    LOG_ASSERT(false, "invalid ast node:{}, source:{}", node->toString(),
               dumpSource(context->sourceName, node->position()));
  }
  return nullptr;
}

IrDefinition *IrFactory::unit(IrContext *context, Ast *node) {
  LOG_ASSERT(context, "context is null");
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case A_ty::VariableDefinition:
    return new IrGlobalVariableDefinition(context,
                                          DC(AstVariableDefinition, node));
  case A_ty::FunctionDefinition:
    return new IrFunctionDefinition(context, DC(AstFunctionDefinition, node));
  default:
    LOG_ASSERT(false, "invalid ast node:{}, source:{}", node->toString(),
               dumpSource(context->sourceName, node->position()));
  }
}
