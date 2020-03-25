// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "Counter.h"
#include "Log.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"

static Counter NameCounter;

#define GEN_NAME(x) (std::string(x) + "_" + std::to_string(NameCounter.count()))

/* ir context */
IrContext::IrContext()
    : context_(), builder_(context_), module_(), symtable_() {}

IrContext::~IrContext() {}

llvm::LLVMContext &IrContext::context() { return context_; }

const llvm::LLVMContext &IrContext::context() const { return context_; }

llvm::IRBuilder<> &IrContext::builder() { return builder_; }

const llvm::IRBuilder<> &IrContext::builder() const { return builder_; }

llvm::Module *&IrContext::module() { return module_; }

const llvm::Module *IrContext::module() const { return module_; }

std::map<std::string, llvm::Value *> &IrContext::symtable() {
  return symtable_;
}

const std::map<std::string, llvm::Value *> &IrContext::symtable() const {
  return symtable_;
}

/* translate unit */
IrTranslateUnit::IrTranslateUnit(AstTranslateUnit *node)
    : Ir<AstTranslateUnit>(node, GEN_NAME("IrTranslateUnit")) {}

int IrTranslateUnit::type() const { return A_TRANSLATE_UNIT; }

llvm::Value *IrTranslateUnit::codegen(IrContext *context) {
  if (!node_->empty()) {
    for (int i = 0; i < node_->size(); i++) {
      Ast *ast = node_->get(i);
      switch (ast->type()) {
      default:
        CINFO("default ast:{}", ast->toString());
      }
    }
  }
}

std::string IrTranslateUnit::stringify() const { return "IrTranslateUnit"; }

/* identifier constant */
IrIdentifierConstant::IrIdentifierConstant(AstIdentifierConstant *node)
    : Ir<AstIdentifierConstant>(node, GEN_NAME("IrIdentifierConstant")) {}

int IrIdentifierConstant::type() const { return A_IDENTIFIER_CONSTANT; }

llvm::Value *IrIdentifierConstant::codegen(IrContext *context) {}

std::string IrIdentifierConstant::stringify() const {
  return "IrIdentifierConstant";
}

/* i8 constant */
IrI8Constant::IrI8Constant(AstI8Constant *node)
    : Ir<AstI8Constant>(node, GEN_NAME("IrI8Constant")) {}

int IrI8Constant::type() const { return A_I8_CONSTANT; }

llvm::Value *IrI8Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(8, node_->value(), true));
}

std::string IrI8Constant::stringify() const { return "IrI8Constant"; }

/* u8 constant */
IrU8Constant::IrU8Constant(AstU8Constant *node)
    : Ir<AstU8Constant>(node, GEN_NAME("IrU8Constant")) {}

int IrU8Constant::type() const { return A_U8_CONSTANT; }

llvm::Value *IrU8Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(8, node_->value(), false));
}

std::string IrU8Constant::stringify() const { return "IrU8Constant"; }

/* i16 constant */
IrI16Constant::IrI16Constant(AstI16Constant *node)
    : Ir<AstI16Constant>(node, GEN_NAME("IrI16Constant")) {}

int IrI16Constant::type() const { return A_I16_CONSTANT; }

llvm::Value *IrI16Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(16, node_->value(), true));
}

std::string IrI16Constant::stringify() const { return "IrI16Constant"; }

/* u16 constant */
IrU16Constant::IrU16Constant(AstU16Constant *node)
    : Ir<AstU16Constant>(node, GEN_NAME("IrU16Constant")) {}

int IrU16Constant::type() const { return A_U16_CONSTANT; }

llvm::Value *IrU16Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(16, node_->value(), false));
}

std::string IrU16Constant::stringify() const { return "IrU16Constant"; }

/* i32 constant */
IrI32Constant::IrI32Constant(AstI32Constant *node)
    : Ir<AstI32Constant>(node, GEN_NAME("IrI32Constant")) {}

int IrI32Constant::type() const { return A_I32_CONSTANT; }

llvm::Value *IrI32Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(32, node_->value(), true));
}

std::string IrI32Constant::stringify() const { return "IrI32Constant"; }

/* u32 constant */
IrU32Constant::IrU32Constant(AstU32Constant *node)
    : Ir<AstU32Constant>(node, GEN_NAME("IrU32Constant")) {}

int IrU32Constant::type() const { return A_U32_CONSTANT; }

llvm::Value *IrU32Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(32, node_->value(), false));
}

std::string IrU32Constant::stringify() const { return "IrU32Constant"; }

/* i64 constant */
IrI64Constant::IrI64Constant(AstI64Constant *node)
    : Ir<AstI64Constant>(node, GEN_NAME("IrI64Constant")) {}

int IrI64Constant::type() const { return A_I64_CONSTANT; }

llvm::Value *IrI64Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(64, node_->value(), true));
}

std::string IrI64Constant::stringify() const { return "IrI64Constant"; }

/* u64 constant */
IrU64Constant::IrU64Constant(AstU64Constant *node)
    : Ir<AstU64Constant>(node, GEN_NAME("IrU64Constant")) {}

int IrU64Constant::type() const { return A_U64_CONSTANT; }

llvm::Value *IrU64Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(64, node_->value(), false));
}

std::string IrU64Constant::stringify() const { return "IrU64Constant"; }

/* f32 constant */
IrF32Constant::IrF32Constant(AstF32Constant *node)
    : Ir<AstF32Constant>(node, GEN_NAME("IrF32Constant")) {}

int IrF32Constant::type() const { return A_F32_CONSTANT; }

llvm::Value *IrF32Constant::codegen(IrContext *context) {
  return llvm::ConstantFP::get(context->context(),
                               llvm::APFloat((float)node_->value()));
}

std::string IrF32Constant::stringify() const { return "IrF32Constant"; }

/* f64 constant */
IrF64Constant::IrF64Constant(AstF64Constant *node)
    : Ir<AstF64Constant>(node, GEN_NAME("IrF64Constant")) {}

int IrF64Constant::type() const { return A_F64_CONSTANT; }

llvm::Value *IrF64Constant::codegen(IrContext *context) {
  return llvm::ConstantFP::get(context->context(),
                               llvm::APFloat((double)node_->value()));
}

std::string IrF64Constant::stringify() const { return "IrF64Constant"; }

/* string constant */
IrStringConstant::IrStringConstant(AstStringConstant *node)
    : Ir<AstStringConstant>(node, GEN_NAME("IrStringConstant")) {}

int IrStringConstant::type() const { return A_STRING_CONSTANT; }

llvm::Value *IrStringConstant::codegen(IrContext *context) { return nullptr; }

std::string IrStringConstant::stringify() const { return "IrStringConstant"; }

/* boolean constant */
IrBooleanConstant::IrBooleanConstant(AstBooleanConstant *node)
    : Ir<AstBooleanConstant>(node, GEN_NAME("IrBooleanConstant")) {}

int IrBooleanConstant::type() const { return A_BOOLEAN_CONSTANT; }

llvm::Value *IrBooleanConstant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(1, node_->value() ? 1U : 0U, false));
}

std::string IrBooleanConstant::stringify() const { return "IrBooleanConstant"; }

/* call expression */
IrCallExpression::IrCallExpression(AstCallExpression *node)
    : Ir<AstCallExpression>(node, GEN_NAME("IrCallExpression")) {}

int IrCallExpression::type() const { return A_CALL_EXPRESSION; }

llvm::Value *IrCallExpression::codegen(IrContext *context) { return nullptr; }

std::string IrCallExpression::stringify() const { return "IrCallExpression"; }

/* unary expression */
IrUnaryExpression::IrUnaryExpression(AstUnaryExpression *node)
    : Ir<AstUnaryExpression>(node, GEN_NAME("IrUnaryExpression")) {}

int IrUnaryExpression::type() const { return A_UNARY_EXPRESSION; }

llvm::Value *IrUnaryExpression::codegen(IrContext *context) { return nullptr; }

std::string IrUnaryExpression::stringify() const { return "IrUnaryExpression"; }

/* binary expression */
IrBinaryExpression::IrBinaryExpression(AstBinaryExpression *node)
    : Ir<AstBinaryExpression>(node, GEN_NAME("IrBinaryExpression")) {}

int IrBinaryExpression::type() const { return A_BINARY_EXPRESSION; }

llvm::Value *IrBinaryExpression::codegen(IrContext *context) { return nullptr; }

std::string IrBinaryExpression::stringify() const {
  return "IrBinaryExpression";
}

/* conditional expression */
IrConditionalExpression::IrConditionalExpression(AstConditionalExpression *node)
    : Ir<AstConditionalExpression>(node, GEN_NAME("IrConditionalExpression")) {}

int IrConditionalExpression::type() const { return A_CONDITIONAL_EXPRESSION; }

llvm::Value *IrConditionalExpression::codegen(IrContext *context) {
  return nullptr;
}

std::string IrConditionalExpression::stringify() const {
  return "IrConditionalExpression";
}

/* assignment expression */
IrAssignmentExpression::IrAssignmentExpression(AstAssignmentExpression *node)
    : Ir<AstAssignmentExpression>(node, GEN_NAME("IrAssignmentExpression")) {}

int IrAssignmentExpression::type() const { return A_ASSIGNMENT_EXPRESSION; }

llvm::Value *IrAssignmentExpression::codegen(IrContext *context) {
  return nullptr;
}

std::string IrAssignmentExpression::stringify() const {
  return "IrAssignmentExpression";
}

/* sequel expression */
IrSequelExpression::IrSequelExpression(AstSequelExpression *node)
    : Ir<AstSequelExpression>(node, GEN_NAME("IrSequelExpression")) {}

int IrSequelExpression::type() const { return A_SEQUEL_EXPERSSION; }

llvm::Value *IrSequelExpression::codegen(IrContext *context) { return nullptr; }

std::string IrSequelExpression::stringify() const {
  return "IrSequelExpression";
}

/* expression statement */
IrExpressionStatement::IrExpressionStatement(AstExpressionStatement *node)
    : Ir<AstExpressionStatement>(node, GEN_NAME("IrExpressionStatement")) {}

int IrExpressionStatement::type() const { return A_EXPRESSION_STATEMENT; }

llvm::Value *IrExpressionStatement::codegen(IrContext *context) {
  return nullptr;
}

std::string IrExpressionStatement::stringify() const {
  return "IrExpressionStatement";
}

/* compound statement */
IrCompoundStatement::IrCompoundStatement(AstCompoundStatement *node)
    : Ir<AstCompoundStatement>(node, GEN_NAME("IrCompoundStatement")) {}

int IrCompoundStatement::type() const { return A_COMPOUND_STATEMENT; }

llvm::Value *IrCompoundStatement::codegen(IrContext *context) {
  return nullptr;
}

std::string IrCompoundStatement::stringify() const {
  return "IrCompoundStatement";
}

/* if statement */
IrIfStatement::IrIfStatement(AstIfStatement *node)
    : Ir<AstIfStatement>(node, GEN_NAME("IrIfStatement")) {}

int IrIfStatement::type() const { return A_IF_STATEMENT; }

llvm::Value *IrIfStatement::codegen(IrContext *context) { return nullptr; }

std::string IrIfStatement::stringify() const { return "IrIfStatement"; }

/* while statement */
IrWhileStatement::IrWhileStatement(AstWhileStatement *node)
    : Ir<AstWhileStatement>(node, GEN_NAME("IrWhileStatement")) {}

int IrWhileStatement::type() const { return A_WHILE_STATEMENT; }

llvm::Value *IrWhileStatement::codegen(IrContext *context) { return nullptr; }

std::string IrWhileStatement::stringify() const { return "IrWhileStatement"; }

/* for statement */
IrForStatement::IrForStatement(AstForStatement *node)
    : Ir<AstForStatement>(node, GEN_NAME("IrForStatement")) {}

int IrForStatement::type() const { return A_FOR_STATEMENT; }

llvm::Value *IrForStatement::codegen(IrContext *context) { return nullptr; }

std::string IrForStatement::stringify() const { return "IrForStatement"; }

/* continue statement */
IrContinueStatement::IrContinueStatement(AstContinueStatement *node)
    : Ir<AstContinueStatement>(node, GEN_NAME("IrContinueStatement")) {}

int IrContinueStatement::type() const { return A_CONTINUE_STATEMENT; }

llvm::Value *IrContinueStatement::codegen(IrContext *context) {
  return nullptr;
}

std::string IrContinueStatement::stringify() const {
  return "IrContinueStatement";
}

/* break statement */
IrBreakStatement::IrBreakStatement(AstBreakStatement *node)
    : Ir<AstBreakStatement>(node, GEN_NAME("IrBreakStatement")) {}

int IrBreakStatement::type() const { return A_BREAK_STATEMENT; }

llvm::Value *IrBreakStatement::codegen(IrContext *context) { return nullptr; }

std::string IrBreakStatement::stringify() const { return "IrBreakStatement"; }

/* return statement */
IrReturnStatement::IrReturnStatement(AstReturnStatement *node)
    : Ir<AstReturnStatement>(node, GEN_NAME("IrReturnStatement")) {}

int IrReturnStatement::type() const { return A_RETURN_STATEMENT; }

llvm::Value *IrReturnStatement::codegen(IrContext *context) { return nullptr; }

std::string IrReturnStatement::stringify() const { return "IrReturnStatement"; }

/* empty statement */
IrEmptyStatement::IrEmptyStatement(AstEmptyStatement *node)
    : Ir<AstEmptyStatement>(node, GEN_NAME("IrEmptyStatement")) {}

int IrEmptyStatement::type() const { return A_EMPTY_STATEMENT; }

llvm::Value *IrEmptyStatement::codegen(IrContext *context) { return nullptr; }

std::string IrEmptyStatement::stringify() const { return "IrEmptyStatement"; }

/* variable declaration */
IrVariableDeclaration::IrVariableDeclaration(AstVariableDeclaration *node)
    : Ir<AstVariableDeclaration>(node, GEN_NAME("IrVariableDeclaration")) {}

int IrVariableDeclaration::type() const { return A_VARIABLE_DECLARATION; }

llvm::Value *IrVariableDeclaration::codegen(IrContext *context) {
  return nullptr;
}

std::string IrVariableDeclaration::stringify() const {
  return "IrVariableDeclaration";
}

/* variable assignment declaration */
IrVariableAssignmentDeclaration::IrVariableAssignmentDeclaration(
    AstVariableAssignmentDeclaration *node)
    : Ir<AstVariableAssignmentDeclaration>(
          node, GEN_NAME("IrVariableAssignmentDeclaration")) {}

int IrVariableAssignmentDeclaration::type() const {
  return A_VARIABLE_ASSIGNMENT_DECLARATION;
}

llvm::Value *IrVariableAssignmentDeclaration::codegen(IrContext *context) {
  return nullptr;
}

std::string IrVariableAssignmentDeclaration::stringify() const {
  return "IrVariableAssignmentDeclaration";
}

/* function declaration */
IrFunctionDeclaration::IrFunctionDeclaration(AstFunctionDeclaration *node)
    : Ir<AstFunctionDeclaration>(node, GEN_NAME("IrFunctionDeclaration")) {}

int IrFunctionDeclaration::type() const { return A_FUNCTION_DECLARATION; }

llvm::Value *IrFunctionDeclaration::codegen(IrContext *context) {
  return nullptr;
}

std::string IrFunctionDeclaration::stringify() const {
  return "IrFunctionDeclaration";
}

/* function argument declaration */
IrFunctionArgumentDeclaration::IrFunctionArgumentDeclaration(
    AstFunctionArgumentDeclaration *node)
    : Ir<AstFunctionArgumentDeclaration>(
          node, GEN_NAME("IrFunctionArgumentDeclaration")) {}

int IrFunctionArgumentDeclaration::type() const {
  return A_FUNCTION_ARGUMENT_DECLARATION;
}

llvm::Value *IrFunctionArgumentDeclaration::codegen(IrContext *context) {
  return nullptr;
}

std::string IrFunctionArgumentDeclaration::stringify() const {
  return "IrFunctionArgumentDeclaration";
}

#undef GEN_NAME
