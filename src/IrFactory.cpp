// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "IrFactory.h"
#include "Ast.h"
#include "Dump.h"
#include "Exception.h"
#include "Ir.h"

#define DC(x, y) dynamic_cast<x *>(y)

IrExpression *IrFactory::expr(IrContext *context, Ast *node) {
  EX_ASSERT(context, "context is null");
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::IdentifierConstant:
    return new IrIdentifierConstant(context, DC(AstIdentifierConstant, node));
  case AstType::Float32Constant:
    return new IrFloat32Constant(context, DC(AstFloat32Constant, node));
  case AstType::Float64Constant:
    return new IrFloat64Constant(context, DC(AstFloat64Constant, node));
  case AstType::StringConstant:
    return new IrStringConstant(context, DC(AstStringConstant, node));
  case AstType::Int8Constant:
    return new IrInt8Constant(context, DC(AstInt8Constant, node));
  case AstType::UInt8Constant:
    return new IrUInt8Constant(context, DC(AstUInt8Constant, node));
  case AstType::Int16Constant:
    return new IrInt16Constant(context, DC(AstInt16Constant, node));
  case AstType::UInt16Constant:
    return new IrUInt16Constant(context, DC(AstUInt16Constant, node));
  case AstType::Int32Constant:
    return new IrInt32Constant(context, DC(AstInt32Constant, node));
  case AstType::UInt32Constant:
    return new IrUInt32Constant(context, DC(AstUInt32Constant, node));
  case AstType::Int64Constant:
    return new IrInt64Constant(context, DC(AstInt64Constant, node));
  case AstType::UInt64Constant:
    return new IrUInt64Constant(context, DC(AstUInt64Constant, node));
  case AstType::BooleanConstant:
    return new IrBooleanConstant(context, DC(AstBooleanConstant, node));
  case AstType::AssignmentExpression:
    return new IrAssignmentExpression(context,
                                      DC(AstAssignmentExpression, node));
  case AstType::SequelExpression:
    return new IrSequelExpression(context, DC(AstSequelExpression, node));
  case AstType::CallExpression:
    return new IrCallExpression(context, DC(AstCallExpression, node));
  case AstType::UnaryExpression:
    return new IrUnaryExpression(context, DC(AstUnaryExpression, node));
  case AstType::BinaryExpression:
    return new IrBinaryExpression(context, DC(AstBinaryExpression, node));
  case AstType::ConditionalExpression:
    return new IrConditionalExpression(context,
                                       DC(AstConditionalExpression, node));
  case AstType::VoidExpression:
    return new IrVoidExpression(context, DC(AstVoidExpression, node));
  default:
    EX_ASSERT(false, "invalid ast node:{}, source:{}", node->toString(),
              dumpSource(context->moduleName, node->position()));
  }
  return nullptr;
}

IrStatement *IrFactory::stmt(IrContext *context, Ast *node) {
  EX_ASSERT(context, "context is null");
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::ExpressionStatement:
    return new IrExpressionStatement(context, DC(AstExpressionStatement, node));
  case AstType::CompoundStatement:
    return new IrCompoundStatement(context, DC(AstCompoundStatement, node));
  case AstType::IfStatement:
    return new IrIfStatement(context, DC(AstIfStatement, node));
  case AstType::WhileStatement:
    return new IrWhileStatement(context, DC(AstWhileStatement, node));
  case AstType::ForStatement:
    return new IrForStatement(context, DC(AstForStatement, node));
  case AstType::ReturnStatement:
    return new IrReturnStatement(context, DC(AstReturnStatement, node));
  case AstType::VariableDefinition:
    return new IrLocalVariableDefinition(context,
                                         DC(AstVariableDefinition, node));
  case AstType::FunctionDefinition:
    return new IrFunctionDefinition(context, DC(AstFunctionDefinition, node));
  case AstType::FunctionSignatureDefinition:
    return new IrFunctionSignatureDefinition(
        context, DC(AstFunctionSignatureDefinition, node));
  case AstType::ContinueStatement:
  case AstType::BreakStatement:
  case AstType::EmptyStatement:
  default:
    EX_ASSERT(false, "invalid ast node:{}, source:{}", node->toString(),
              dumpSource(context->moduleName, node->position()));
  }
  return nullptr;
}

IrDefinition *IrFactory::unit(IrContext *context, Ast *node) {
  EX_ASSERT(context, "context is null");
  EX_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::VariableDefinition:
    return new IrGlobalVariableDefinition(context,
                                          DC(AstVariableDefinition, node));
  case AstType::FunctionDefinition:
    return new IrFunctionDefinition(context, DC(AstFunctionDefinition, node));
  default:
    EX_ASSERT(false, "invalid ast node:{}, source:{}", node->toString(),
              dumpSource(context->moduleName, node->position()));
  }
}
