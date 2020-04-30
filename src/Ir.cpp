// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "Log.h"
#include "NameGenerator.h"
#include "Parser.tab.hpp"
#include "fmt/format.h"
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

#define DC(x, y) dynamic_cast<x *>(y)

/* ir context */
IrContext::IrContext()
    : context_(), builder_(context_), module_(nullptr), symtable_() {
  module_ = new llvm::Module("shepherd jit", context_);
}

IrContext::~IrContext() {
  delete module_;
  module_ = nullptr;
}

llvm::LLVMContext &IrContext::context() { return context_; }

const llvm::LLVMContext &IrContext::context() const { return context_; }

llvm::IRBuilder<> &IrContext::builder() { return builder_; }

const llvm::IRBuilder<> &IrContext::builder() const { return builder_; }

llvm::Module *&IrContext::module() { return module_; }

const llvm::Module *IrContext::module() const { return module_; }

std::unordered_map<std::string, llvm::Value *> &IrContext::symtable() {
  return symtable_;
}

const std::unordered_map<std::string, llvm::Value *> &
IrContext::symtable() const {
  return symtable_;
}

/* Ir */
Ir::Ir(const std::string &name) : name_(name) {}

std::string Ir::name() const { return name_; }

/* translate unit */
IrTranslateUnit::IrTranslateUnit(AstTranslateUnit *node)
    : detail::IrList<Ir>(nameGen("IrTranslateUnit")), node_(node) {}

IrType IrTranslateUnit::type() const { return IrType::TranslateUnit; }

llvm::Value *IrTranslateUnit::codeGen(IrContext *context) {
  for (int i = 0; i < node_->size(); i++) {
    Ast *ast = node_->get(i);
    switch (ast->type()) {
    case AstType::VariableDeclaration: {
      add(new IrVariableDeclaration(
          dynamic_cast<AstVariableDeclaration *>(ast)));
    } break;
    case AstType::FunctionDeclaration: {
      add(new IrFunctionDeclaration(
          dynamic_cast<AstFunctionDeclaration *>(ast)));
    } break;
    default:
      LOG_ASSERT(false, "invalid ast:{}", ast->toString());
    }
  }
  return nullptr;
}

/* expression */
IrExpression::IrExpression(AstExpression *node)
    : Ir(nameGen("IrExpression")), node_(node) {}

IrType IrExpression::type() const { return IrType::Expression; }

llvm::Value *IrExpression::codeGen(IrContext *context) {
  switch (node_->type()) {
  case AstType::IdentifierConstant:
    return IrIdentifierConstant(DC(AstIdentifierConstant, node_))
        .codeGen(context);
  case AstType::F32Constant:
    return IrF32Constant(DC(AstF32Constant, node_)).codeGen(context);
  case AstType::F64Constant:
    return IrF64Constant(DC(AstF64Constant, node_)).codeGen(context);
  default:
    LOG_ASSERT(false, "node {} invalid", node_->toString());
  }
  return nullptr;
}

std::string IrExpression::toString() const {
  return fmt::format("[ @IrExpression node_:{} ]", node_->toString());
}

/* identifier constant */
IrIdentifierConstant::IrIdentifierConstant(AstIdentifierConstant *node)
    : Ir(nameGen("IrIdentifierConstant")), node_(node) {}

IrType IrIdentifierConstant::type() const { return IrType::IdentifierConstant; }

llvm::Value *IrIdentifierConstant::codeGen(IrContext *context) {
  LOG_ASSERT(context, "context is null");
  LOG_ASSERT(node_, "node_ is null");
  LOG_ASSERT(context->symtable().find(node_->value()) ==
                 context->symtable().end(),
             "symbol {} not found in context", node_->value());
  return context->symtable()[node_->value()];
}

std::string IrIdentifierConstant::toString() const {
  return fmt::format("[ @IrIdentifierConstant node_:{} ]", node_->toString());
}

/* i8 constant */
IrI8Constant::IrI8Constant(AstI8Constant *node)
    : Ir(nameGen("IrI8Constant")), node_(node) {}

IrType IrI8Constant::type() const { return IrType::I8Constant; }

llvm::Value *IrI8Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(8, (uint64_t)node_->value(), true));
}

std::string IrI8Constant::toString() const {
  return fmt::format("[ @IrI8Constant node_:{} ]", node_->toString());
}

/* u8 constant */
IrU8Constant::IrU8Constant(AstU8Constant *node)
    : Ir(nameGen("IrU8Constant")), node_(node) {}

IrType IrU8Constant::type() const { return IrType::U8Constant; }

llvm::Value *IrU8Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(8, (uint64_t)node_->value(), false));
}

std::string IrU8Constant::toString() const {
  return fmt::format("[ @IrU8Constant node_:{} ]", node_->toString());
}

/* i16 constant */
IrI16Constant::IrI16Constant(AstI16Constant *node)
    : Ir(nameGen("IrI16Constant")), node_(node) {}

IrType IrI16Constant::type() const { return IrType::I16Constant; }

llvm::Value *IrI16Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(16, (uint64_t)node_->value(), true));
}

std::string IrI16Constant::toString() const {
  return fmt::format("[ @IrI16Constant node_:{} ]", node_->toString());
}

/* u16 constant */
IrU16Constant::IrU16Constant(AstU16Constant *node)
    : Ir(nameGen("IrU16Constant")), node_(node) {}

IrType IrU16Constant::type() const { return IrType::U16Constant; }

llvm::Value *IrU16Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(16, (uint64_t)node_->value(), false));
}

std::string IrU16Constant::toString() const {
  return fmt::format("[ @IrU16Constant node_:{} ]", node_->toString());
}

/* i32 constant */
IrI32Constant::IrI32Constant(AstI32Constant *node)
    : Ir(nameGen("IrI32Constant")), node_(node) {}

IrType IrI32Constant::type() const { return IrType::I32Constant; }

llvm::Value *IrI32Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(32, (uint64_t)node_->value(), true));
}

std::string IrI32Constant::toString() const {
  return fmt::format("[ @IrI32Constant node_:{} ]", node_->toString());
}

/* u32 constant */
IrU32Constant::IrU32Constant(AstU32Constant *node)
    : Ir(nameGen("IrU32Constant")), node_(node) {}

IrType IrU32Constant::type() const { return IrType::U32Constant; }

llvm::Value *IrU32Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(32, (uint64_t)node_->value(), false));
}

std::string IrU32Constant::toString() const {
  return fmt::format("[ @IrU32Constant node_:{} ]", node_->toString());
}

/* i64 constant */
IrI64Constant::IrI64Constant(AstI64Constant *node)
    : Ir(nameGen("IrI64Constant")), node_(node) {}

IrType IrI64Constant::type() const { return IrType::I64Constant; }

llvm::Value *IrI64Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(64, (uint64_t)node_->value(), true));
}

std::string IrI64Constant::toString() const {
  return fmt::format("[ @IrI64Constant node_:{} ]", node_->toString());
}

/* u64 constant */
IrU64Constant::IrU64Constant(AstU64Constant *node)
    : Ir(nameGen("IrU64Constant")), node_(node) {}

IrType IrU64Constant::type() const { return IrType::U64Constant; }

llvm::Value *IrU64Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(64, (uint64_t)node_->value(), false));
}

std::string IrU64Constant::toString() const {
  return fmt::format("[ @IrU64Constant node_:{} ]", node_->toString());
}

/* f32 constant */
IrF32Constant::IrF32Constant(AstF32Constant *node)
    : Ir(nameGen("IrF32Constant")), node_(node) {}

IrType IrF32Constant::type() const { return IrType::F32Constant; }

llvm::Value *IrF32Constant::codeGen(IrContext *context) {
  return llvm::ConstantFP::get(context->context(),
                               llvm::APFloat((float)node_->value()));
}

std::string IrF32Constant::toString() const {
  return fmt::format("[ @IrF32Constant node_:{} ]", node_->toString());
}

/* f64 constant */
IrF64Constant::IrF64Constant(AstF64Constant *node)
    : Ir(nameGen("IrF64Constant")), node_(node) {}

IrType IrF64Constant::type() const { return IrType::F64Constant; }

llvm::Value *IrF64Constant::codeGen(IrContext *context) {
  return llvm::ConstantFP::get(context->context(),
                               llvm::APFloat((double)node_->value()));
}

std::string IrF64Constant::toString() const {
  return fmt::format("[ @IrF64Constant node_:{} ]", node_->toString());
}

/* string constant */
IrStringConstant::IrStringConstant(AstStringConstant *node)
    : Ir(nameGen("IrStringConstant")), node_(node) {}

IrType IrStringConstant::type() const { return IrType::StringConstant; }

llvm::Value *IrStringConstant::codeGen(IrContext *context) { return nullptr; }

std::string IrStringConstant::toString() const {
  return fmt::format("[ @IrStringConstant node_:{} ]", node_->toString());
}

/* boolean constant */
IrBooleanConstant::IrBooleanConstant(AstBooleanConstant *node)
    : Ir(nameGen("IrBooleanConstant")), node_(node) {}

IrType IrBooleanConstant::type() const { return IrType::BooleanConstant; }

llvm::Value *IrBooleanConstant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(),
      llvm::APInt(1, node_->value() ? (uint64_t)1U : (uint64_t)0U, false));
}

std::string IrBooleanConstant::toString() const {
  return fmt::format("[ @IrBooleanConstant node_:{} ]", node_->toString());
}

/* call expression */
IrCallExpression::IrCallExpression(AstCallExpression *node)
    : Ir(nameGen("IrCallExpression")), node_(node) {}

IrType IrCallExpression::type() const { return IrType::CallExpression; }

llvm::Value *IrCallExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrCallExpression::toString() const {
  return fmt::format("[ @IrCallExpression node_:{} ]", node_->toString());
}

/* unary expression */
IrUnaryExpression::IrUnaryExpression(AstUnaryExpression *node)
    : Ir(nameGen("IrUnaryExpression")), node_(node) {}

IrType IrUnaryExpression::type() const { return IrType::UnaryExpression; }

llvm::Value *IrUnaryExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrUnaryExpression::toString() const {
  return fmt::format("[ @IrUnaryExpression node_:{} ]", node_->toString());
}

/* binary expression */
IrBinaryExpression::IrBinaryExpression(AstBinaryExpression *node,
                                       IrExpression *left, IrExpression *right)
    : Ir(nameGen("IrBinaryExpression")), node_(node), left_(left),
      right_(right) {}

IrBinaryExpression::~IrBinaryExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

IrType IrBinaryExpression::type() const { return IrType::BinaryExpression; }

llvm::Value *IrBinaryExpression::codeGen(IrContext *context) {
  llvm::Value *l = left_->codeGen(context);
  llvm::Value *r = right_->codeGen(context);
  LOG_ASSERT(l, "l is null");
  LOG_ASSERT(r, "r is null");
  switch (node_->token()) {
  case T_ADD: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateFAdd(l, r, "addtmp");
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFAdd(l, r, "addtmp");
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateAdd(l, r, "addtmp");
      } break;
      default:
        LOG_ASSERT(false, "r->getType->getTypeID {} invalid",
                   r->getType()->getTypeID());
      }
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_SUB: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateFSub(l, r, "subtmp");
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFSub(l, r, "subtmp");
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateSub(l, r, "subtmp");
      } break;
      default:
        LOG_ASSERT(false, "r->getType->getTypeID {} invalid",
                   r->getType()->getTypeID());
      }
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_MUL: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateFMul(l, r, "multmp");
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFMul(l, r, "multmp");
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateMul(l, r, "multmp");
      } break;
      default:
        LOG_ASSERT(false, "r->getType->getTypeID {} invalid",
                   r->getType()->getTypeID());
      }
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_DIV: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateSDiv(l, r, "divtmp");
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateSDiv(l, r, "divtmp");
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateSDiv(l, r, "divtmp");
      } break;
      default:
        LOG_ASSERT(false, "r->getType->getTypeID {} invalid",
                   r->getType()->getTypeID());
      }
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_MOD: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::IntegerTyID: {
        // first do div
        llvm::Value *dr = context->builder().CreateSDiv(l, r, "divtmp");
        // second do sub
        return context->builder().CreateSub(l, dr, "subtmp");
      } break;
      default:
        LOG_ASSERT(false, "r->getType->getTypeID {} invalid",
                   r->getType()->getTypeID());
      }
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_BIT_LSHIFT:
  case T_BIT_RSHIFT:
  case T_BIT_ARSHIFT:
  case T_EQ:
  case T_NEQ:
  case T_LE:
  case T_LT:
  case T_GE:
  case T_GT:
  case T_BIT_AND:
  case T_BIT_OR:
  case T_BIT_XOR:
  case T_LOGIC_AND:
  case T_LOGIC_OR:
  default:
    LOG_ASSERT(false, "token {} invalid", node_->token());
  }
  return nullptr;
}

std::string IrBinaryExpression::toString() const {
  return fmt::format("[ @IrBinaryExpression node_:{} ]", node_->toString());
}

/* conditional expression */
IrConditionalExpression::IrConditionalExpression(AstConditionalExpression *node)
    : Ir(nameGen("IrConditionalExpression")), node_(node) {}

IrType IrConditionalExpression::type() const {
  return IrType::ConditionalExpression;
}

llvm::Value *IrConditionalExpression::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrConditionalExpression::toString() const {
  return fmt::format("[ @IrConditionalExpression node_:{} ]",
                     node_->toString());
}

/* assignment expression */
IrAssignmentExpression::IrAssignmentExpression(AstAssignmentExpression *node)
    : Ir(nameGen("IrAssignmentExpression")), node_(node) {}

IrType IrAssignmentExpression::type() const {
  return IrType::AssignmentExpression;
}

llvm::Value *IrAssignmentExpression::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrAssignmentExpression::toString() const {
  return fmt::format("[ @IrAssignmentExpression node_:{} ]", node_->toString());
}

/* sequel expression */
IrSequelExpression::IrSequelExpression(AstSequelExpression *node)
    : Ir(nameGen("IrSequelExpression")), node_(node) {}

IrType IrSequelExpression::type() const { return IrType::SequelExpression; }

llvm::Value *IrSequelExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrSequelExpression::toString() const {
  return fmt::format("[ @IrSequelExpression node_:{} ]", node_->toString());
}

/* expression statement */
IrExpressionStatement::IrExpressionStatement(AstExpressionStatement *node)
    : Ir(nameGen("IrExpressionStatement")), node_(node) {}

IrType IrExpressionStatement::type() const {
  return IrType::ExpressionStatement;
}

llvm::Value *IrExpressionStatement::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrExpressionStatement::toString() const {
  return fmt::format("[ @IrExpressionStatement node_:{} ]", node_->toString());
}

/* compound statement */
IrCompoundStatement::IrCompoundStatement(AstCompoundStatement *node)
    : Ir(nameGen("IrCompoundStatement")), node_(node) {}

IrType IrCompoundStatement::type() const { return IrType::CompoundStatement; }

llvm::Value *IrCompoundStatement::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrCompoundStatement::toString() const {
  return fmt::format("[ @IrCompoundStatement node_:{} ]", node_->toString());
}

/* if statement */
IrIfStatement::IrIfStatement(AstIfStatement *node)
    : Ir(nameGen("IrIfStatement")), node_(node) {}

IrType IrIfStatement::type() const { return IrType::IfStatement; }

llvm::Value *IrIfStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrIfStatement::toString() const {
  return fmt::format("[ @IrIfStatement node_:{} ]", node_->toString());
}

/* while statement */
IrWhileStatement::IrWhileStatement(AstWhileStatement *node)
    : Ir(nameGen("IrWhileStatement")), node_(node) {}

IrType IrWhileStatement::type() const { return IrType::WhileStatement; }

llvm::Value *IrWhileStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrWhileStatement::toString() const {
  return fmt::format("[ @IrWhileStatement node_:{} ]", node_->toString());
}

/* for statement */
IrForStatement::IrForStatement(AstForStatement *node)
    : Ir(nameGen("IrForStatement")), node_(node) {}

IrType IrForStatement::type() const { return IrType::ForStatement; }

llvm::Value *IrForStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrForStatement::toString() const {
  return fmt::format("[ @IrForStatement node_:{} ]", node_->toString());
}

/* continue statement */
IrContinueStatement::IrContinueStatement(AstContinueStatement *node)
    : Ir(nameGen("IrContinueStatement")), node_(node) {}

IrType IrContinueStatement::type() const { return IrType::ContinueStatement; }

llvm::Value *IrContinueStatement::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrContinueStatement::toString() const {
  return fmt::format("[ @IrContinueStatement node_:{} ]", node_->toString());
}

/* break statement */
IrBreakStatement::IrBreakStatement(AstBreakStatement *node)
    : Ir(nameGen("IrBreakStatement")), node_(node) {}

IrType IrBreakStatement::type() const { return IrType::BreakStatement; }

llvm::Value *IrBreakStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrBreakStatement::toString() const {
  return fmt::format("[ @IrBreakStatement node_:{} ]", node_->toString());
}

/* return statement */
IrReturnStatement::IrReturnStatement(AstReturnStatement *node)
    : Ir(nameGen("IrReturnStatement")), node_(node) {}

IrType IrReturnStatement::type() const { return IrType::ReturnStatement; }

llvm::Value *IrReturnStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrReturnStatement::toString() const {
  return fmt::format("[ @IrReturnStatement node_:{} ]", node_->toString());
}

/* empty statement */
IrEmptyStatement::IrEmptyStatement(AstEmptyStatement *node)
    : Ir(nameGen("IrEmptyStatement")), node_(node) {}

IrType IrEmptyStatement::type() const { return IrType::EmptyStatement; }

llvm::Value *IrEmptyStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrEmptyStatement::toString() const {
  return fmt::format("[ @IrEmptyStatement node_:{} ]", node_->toString());
}

/* variable declaration */
IrVariableDeclaration::IrVariableDeclaration(AstVariableDeclaration *node)
    : Ir(nameGen("IrVariableDeclaration")), node_(node) {}

IrType IrVariableDeclaration::type() const {
  return IrType::VariableDeclaration;
}

llvm::Value *IrVariableDeclaration::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrVariableDeclaration::toString() const {
  return fmt::format("[ @IrVariableDeclaration node_:{} ]", node_->toString());
}

/* variable initial declaration */
IrVariableInitialDeclaration::IrVariableInitialDeclaration(
    AstVariableInitialDeclaration *node)
    : Ir(nameGen("IrVariableInitialDeclaration")), node_(node) {}

IrType IrVariableInitialDeclaration::type() const {
  return IrType::VariableInitialDeclaration;
}

llvm::Value *IrVariableInitialDeclaration::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrVariableInitialDeclaration::toString() const {
  return fmt::format("[ @IrVariableInitialDeclaration node_:{} ]",
                     node_->toString());
}

/* function declaration */
IrFunctionDeclaration::IrFunctionDeclaration(AstFunctionDeclaration *node)
    : Ir(nameGen("IrFunctionDeclaration")), node_(node) {}

IrType IrFunctionDeclaration::type() const {
  return IrType::FunctionDeclaration;
}

llvm::Value *IrFunctionDeclaration::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrFunctionDeclaration::toString() const {
  return fmt::format("[ @IrFunctionDeclaration node_:{} ]", node_->toString());
}

/* function signature declaration */
IrFunctionSignatureDeclaration::IrFunctionSignatureDeclaration(
    AstFunctionSignatureDeclaration *node)
    : Ir(nameGen("IrFunctionSignatureDeclaration")), node_(node) {}

std::string IrFunctionSignatureDeclaration::toString() const {
  return fmt::format("[ @IrFunctionSignatureDeclaration node_:{} ]",
                     node_->toString());
}

IrType IrFunctionSignatureDeclaration::type() const {
  return IrType::FunctionSignatureDeclaration;
}

llvm::Value *IrFunctionSignatureDeclaration::codeGen(IrContext *context) {
  AstDeclarationList *args = node_->argumentList();
  std::vector<llvm::Type *> doubleArgs(
      args->size(), llvm::Type::getDoubleTy(context->context()));
  // result, parameters
  llvm::FunctionType *ft = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(context->context()), doubleArgs, false);
  return nullptr;
}

/* function argument declaration */
IrFunctionArgumentDeclaration::IrFunctionArgumentDeclaration(
    AstFunctionArgumentDeclaration *node)
    : Ir(nameGen("IrFunctionArgumentDeclaration")), node_(node) {}

IrType IrFunctionArgumentDeclaration::type() const {
  return IrType::FunctionArgumentDeclaration;
}

llvm::Value *IrFunctionArgumentDeclaration::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrFunctionArgumentDeclaration::toString() const {
  return fmt::format("[ @IrFunctionArgumentDeclaration node_:{} ]",
                     node_->toString());
}

#undef DC
