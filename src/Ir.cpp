// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "Dump.h"
#include "Log.h"
#include "NameGenerator.h"
#include "Parser.tab.hpp"
#include "container/LinkedHashMap.hpp"
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

LinkedHashMap<std::string, llvm::Value *> &IrContext::symtable() {
  return symtable_;
}

const LinkedHashMap<std::string, llvm::Value *> &IrContext::symtable() const {
  return symtable_;
}

static Ir *createIr(Ast *node) {
  if (!node) {
    return nullptr;
  }
  switch (node->type()) {
  case AstType::IdentifierConstant:
    return new IrIdentifierConstant(DC(AstIdentifierConstant, node));
  case AstType::F32Constant:
    return new IrF32Constant(DC(AstF32Constant, node));
  case AstType::F64Constant:
    return new IrF64Constant(DC(AstF64Constant, node));
  case AstType::StringConstant:
    return new IrStringConstant(DC(AstStringConstant, node));
  case AstType::I8Constant:
    return new IrI8Constant(DC(AstI8Constant, node));
  case AstType::U8Constant:
    return new IrU8Constant(DC(AstU8Constant, node));
  case AstType::I16Constant:
    return new IrI16Constant(DC(AstI16Constant, node));
  case AstType::U16Constant:
    return new IrU16Constant(DC(AstU16Constant, node));
  case AstType::I32Constant:
    return new IrI32Constant(DC(AstI32Constant, node));
  case AstType::U32Constant:
    return new IrU32Constant(DC(AstU32Constant, node));
  case AstType::I64Constant:
    return new IrI64Constant(DC(AstI64Constant, node));
  case AstType::U64Constant:
    return new IrU64Constant(DC(AstU64Constant, node));
  case AstType::BooleanConstant:
    return new IrBooleanConstant(DC(AstBooleanConstant, node));
  case AstType::AssignmentExpression:
    return new IrAssignmentExpression(DC(AstAssignmentExpression, node));
  case AstType::SequelExpression:
    return new IrSequelExpression(DC(AstSequelExpression, node));
  case AstType::CallExpression:
    return new IrCallExpression(DC(AstCallExpression, node));
  case AstType::UnaryExpression:
    return new IrUnaryExpression(DC(AstUnaryExpression, node));
  case AstType::BinaryExpression:
    return new IrBinaryExpression(DC(AstBinaryExpression, node));
  case AstType::ConditionalExpression:
    return new IrConditionalExpression(DC(AstConditionalExpression, node));
  case AstType::ExpressionStatement:
    return new IrExpressionStatement(DC(AstExpressionStatement, node));
  case AstType::IfStatement:
    return new IrIfStatement(DC(AstIfStatement, node));
  case AstType::WhileStatement:
    return new IrWhileStatement(DC(AstWhileStatement, node));
  case AstType::ForStatement:
    return new IrForStatement(DC(AstForStatement, node));
  case AstType::CompoundStatement:
    return new IrCompoundStatement(DC(AstCompoundStatement, node));
  case AstType::VariableDeclaration:
    return new IrVariableDeclaration(DC(AstVariableDeclaration, node));
  case AstType::FunctionDeclaration:
    return new IrFunctionDeclaration(DC(AstFunctionDeclaration, node));
  default:
    LOG_ASSERT(false, "invalid ast node: {}", node->toString());
  }
  return nullptr;
}

/* interface */
Ir::Ir(const std::string &name) : name_(name) {}

std::string Ir::name() const { return name_; }

IrExpression::IrExpression(const std::string &name) : Ir(name) {}

IrStatement::IrStatement(const std::string &name) : Ir(name) {}

IrDeclaration::IrDeclaration(const std::string &name) : Ir(name) {}

/* list */
IrExpressionList::IrExpressionList()
    : detail::IrList<IrExpression>(nameGen("ExprList")) {}

IrType IrExpressionList::type() const { return IrType::ExpressionList; }

std::string IrExpressionList::stringify() const { return "IrExpressionList"; }

IrStatementList::IrStatementList()
    : detail::IrList<IrStatement>(nameGen("StmtList")) {}

IrType IrStatementList::type() const { return IrType::StatementList; }

std::string IrStatementList::stringify() const { return "IrStatementList"; }

IrDeclarationList::IrDeclarationList()
    : detail::IrList<IrDeclaration>(nameGen("DeclList")) {}

IrType IrDeclarationList::type() const { return IrType::DeclarationList; }

std::string IrDeclarationList::stringify() const { return "IrDeclarationList"; }

/* translate unit */
IrTranslateUnit::IrTranslateUnit(AstTranslateUnit *node)
    : detail::IrList<IrDeclaration>(nameGen("TUnit")), node_(node) {
  for (int i = 0; i < node_->size(); i++) {
    AstDeclaration *ast = node_->get(i);
    switch (ast->type()) {
    case AstType::VariableDeclaration: {
      IrVariableDeclaration *vd =
          new IrVariableDeclaration(DC(AstVariableDeclaration, ast));
      add(vd);
    } break;
    case AstType::FunctionDeclaration: {
      IrFunctionDeclaration *fd =
          new IrFunctionDeclaration(DC(AstFunctionDeclaration, ast));
      add(fd);
    } break;
    default:
      LOG_ASSERT(false, "invalid ast:{}", ast->toString());
    }
  }
}

IrType IrTranslateUnit::type() const { return IrType::TranslateUnit; }

std::string IrTranslateUnit::dumpCodeGen(IrContext *context) {
  LOG_ASSERT(context, "context is null");
  std::stringstream ss;
  for (int i = 0; i < size(); i++) {
    IrDeclaration *ir = get(i);
    switch (ir->type()) {
    case IrType::VariableDeclaration: {
      llvm::Value *v = DC(IrVariableDeclaration, ir)->codeGen(context);
      ss << dumpLLVMValue(v);
    } break;
    case IrType::FunctionDeclaration: {
      llvm::Function *f = DC(IrFunctionDeclaration, ir)->codeGen(context);
      ss << dumpLLVMFunction(f);
    } break;
    default:
      LOG_ASSERT(false, "invalid ir:{}", ir->toString());
    }
  }
  return ss.str();
}

std::string IrTranslateUnit::stringify() const { return "IrTranslateUnit"; }

/* identifier constant */
IrIdentifierConstant::IrIdentifierConstant(AstIdentifierConstant *node)
    : IrExpression(nameGen("Id")), node_(node) {}

IrType IrIdentifierConstant::type() const { return IrType::IdentifierConstant; }

llvm::Value *IrIdentifierConstant::codeGen(IrContext *context) {
  LOG_ASSERT(context, "context is null");
  LOG_ASSERT(node_, "node_ is null");
  llvm::Value *v = context->symtable()[node_->value()];
  llvm::Value *vec{v};
  return vec;
}

std::string IrIdentifierConstant::toString() const {
  return fmt::format("[ @IrIdentifierConstant node_:{} ]", node_->toString());
}

/* i8 constant */
IrI8Constant::IrI8Constant(AstI8Constant *node)
    : IrExpression(nameGen("IrI8Constant")), node_(node) {}

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
    : IrExpression(nameGen("IrU8Constant")), node_(node) {}

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
    : IrExpression(nameGen("IrI16Constant")), node_(node) {}

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
    : IrExpression(nameGen("IrU16Constant")), node_(node) {}

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
    : IrExpression(nameGen("IrI32Constant")), node_(node) {}

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
    : IrExpression(nameGen("IrU32Constant")), node_(node) {}

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
    : IrExpression(nameGen("IrI64Constant")), node_(node) {}

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
    : IrExpression(nameGen("IrU64Constant")), node_(node) {}

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
    : IrExpression(nameGen("IrF32Constant")), node_(node) {}

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
    : IrExpression(nameGen("IrF64Constant")), node_(node) {}

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
    : IrExpression(nameGen("IrStringConstant")), node_(node) {}

IrType IrStringConstant::type() const { return IrType::StringConstant; }

llvm::Value *IrStringConstant::codeGen(IrContext *context) { return nullptr; }

std::string IrStringConstant::toString() const {
  return fmt::format("[ @IrStringConstant node_:{} ]", node_->toString());
}

/* boolean constant */
IrBooleanConstant::IrBooleanConstant(AstBooleanConstant *node)
    : IrExpression(nameGen("IrBooleanConstant")), node_(node) {}

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
    : IrExpression(nameGen("IrCallExpression")), node_(node) {}

IrType IrCallExpression::type() const { return IrType::CallExpression; }

llvm::Value *IrCallExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrCallExpression::toString() const {
  return fmt::format("[ @IrCallExpression node_:{} ]", node_->toString());
}

/* unary expression */
IrUnaryExpression::IrUnaryExpression(AstUnaryExpression *node)
    : IrExpression(nameGen("IrUnaryExpression")), node_(node) {}

IrType IrUnaryExpression::type() const { return IrType::UnaryExpression; }

llvm::Value *IrUnaryExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrUnaryExpression::toString() const {
  return fmt::format("[ @IrUnaryExpression node_:{} ]", node_->toString());
}

/* binary expression */
IrBinaryExpression::IrBinaryExpression(AstBinaryExpression *node)
    : IrExpression(nameGen("IrBinaryExpression")), node_(node),
      left_(DC(IrExpression, createIr(node->left()))),
      right_(DC(IrExpression, createIr(node->right()))) {}

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
        llvm::Value *d = context->builder().CreateSDiv(l, r, "divtmp");
        // second do sub
        return context->builder().CreateSub(l, d, "subtmp");
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
    : IrExpression(nameGen("IrConditionalExpression")), node_(node) {}

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
    : IrExpression(nameGen("IrAssignmentExpression")), node_(node) {}

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
    : IrExpression(nameGen("IrSequelExpression")), node_(node) {}

IrType IrSequelExpression::type() const { return IrType::SequelExpression; }

llvm::Value *IrSequelExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrSequelExpression::toString() const {
  return fmt::format("[ @IrSequelExpression node_:{} ]", node_->toString());
}

/* expression statement */
IrExpressionStatement::IrExpressionStatement(AstExpressionStatement *node)
    : IrStatement(nameGen("IrExpressionStatement")), node_(node),
      expr_(DC(IrExpression, createIr(node))) {}

IrExpressionStatement::~IrExpressionStatement() {
  delete expr_;
  expr_ = nullptr;
}

IrType IrExpressionStatement::type() const {
  return IrType::ExpressionStatement;
}

llvm::Value *IrExpressionStatement::codeGen(IrContext *context) {
  switch (expr_->type()) {
  case IrType::I8Constant:
    return DC(IrI8Constant, expr_)->codeGen(context);
  case IrType::U8Constant:
    return DC(IrU8Constant, expr_)->codeGen(context);
  case IrType::I16Constant:
    return DC(IrI16Constant, expr_)->codeGen(context);
  case IrType::U16Constant:
    return DC(IrU16Constant, expr_)->codeGen(context);
  case IrType::I32Constant:
    return DC(IrI32Constant, expr_)->codeGen(context);
  case IrType::U32Constant:
    return DC(IrU32Constant, expr_)->codeGen(context);
  case IrType::I64Constant:
    return DC(IrI64Constant, expr_)->codeGen(context);
  case IrType::U64Constant:
    return DC(IrU64Constant, expr_)->codeGen(context);
  case IrType::IdentifierConstant:
    return DC(IrIdentifierConstant, expr_)->codeGen(context);
  case IrType::StringConstant:
    return DC(IrStringConstant, expr_)->codeGen(context);
  case IrType::BooleanConstant:
    return DC(IrBooleanConstant, expr_)->codeGen(context);
  case IrType::CallExpression:
    return DC(IrCallExpression, expr_)->codeGen(context);
  case IrType::UnaryExpression:
    return DC(IrUnaryExpression, expr_)->codeGen(context);
  case IrType::BinaryExpression:
    return DC(IrBinaryExpression, expr_)->codeGen(context);
  case IrType::ConditionalExpression:
    return DC(IrConditionalExpression, expr_)->codeGen(context);
  case IrType::AssignmentExpression:
    return DC(IrAssignmentExpression, expr_)->codeGen(context);
  case IrType::SequelExpression:
    return DC(IrSequelExpression, expr_)->codeGen(context);
  default:
    LOG_ASSERT(false, "invalid ir type:{}, ast type:{}",
               expr_->type()._to_string(), node_->type()._to_string());
  }
  return nullptr;
}

std::string IrExpressionStatement::toString() const {
  return fmt::format("[ @IrExpressionStatement node_:{} ]", node_->toString());
}

/* compound statement */
IrCompoundStatement::IrCompoundStatement(AstCompoundStatement *node)
    : IrStatement(nameGen("IrCompoundStatement")), node_(node),
      statementList_(nullptr) {
  if (node_->statementList()) {
    statementList_ = new IrStatementList();
    for (int i = 0; i < node_->statementList()->size(); i++) {
      Ast *ast = node_->statementList()->get(i);
      statementList_->add(DC(IrStatement, createIr(ast)));
    }
  }
}

IrCompoundStatement::~IrCompoundStatement() {
  if (statementList_) {
    for (int i = 0; i < statementList_->size(); i++) {
      Ir *ir = statementList_->get(i);
      delete ir;
    }
    delete statementList_;
    statementList_ = nullptr;
  }
}

IrType IrCompoundStatement::type() const { return IrType::CompoundStatement; }

llvm::Value *IrCompoundStatement::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrCompoundStatement::toString() const {
  return fmt::format("[ @IrCompoundStatement node_:{} ]", node_->toString());
}

/* if statement */
IrIfStatement::IrIfStatement(AstIfStatement *node)
    : IrStatement(nameGen("IrIfStatement")), node_(node),
      condition_(DC(IrExpression, createIr(node->condition()))),
      thens_(DC(IrStatement, createIr(node->thens()))),
      elses_(DC(IrStatement, createIr(node->elses()))) {}

IrIfStatement::~IrIfStatement() {
  delete condition_;
  condition_ = nullptr;
  delete thens_;
  thens_ = nullptr;
  delete elses_;
  elses_ = nullptr;
}

IrType IrIfStatement::type() const { return IrType::IfStatement; }

llvm::Value *IrIfStatement::codeGen(IrContext *context) {
  llvm::Value *condV = condition_->codeGen(context);
  if (!condV) {
    return nullptr;
  }
  condV = context->builder().CreateFCmpONE(
      condV, llvm::ConstantFP::get(context->context(), llvm::APFloat(0.0)),
      "ifcond");
  llvm::Function *f = context->builder().GetInsertBlock()->getParent();
  llvm::BasicBlock *thenBlock =
      llvm::BasicBlock::Create(context->context(), "then", f);
  llvm::BasicBlock *elseBlock =
      llvm::BasicBlock::Create(context->context(), "else");
  llvm::BasicBlock *mergeBlock =
      llvm::BasicBlock::Create(context->context(), "ifcont");
  context->builder().CreateCondBr(condV, thenBlock, elseBlock);
  context->builder().SetInsertPoint(thenBlock);
  llvm::Value *thenV = thens_->codeGen(context);
  if (!thenV) {
    return nullptr;
  }
  context->builder().CreateBr(mergeBlock);
  thenBlock = context->builder().GetInsertBlock();
  f->getBasicBlockList().push_back(elseBlock);
  context->builder().SetInsertPoint(elseBlock);
  llvm::Value *elseV = elses_->codeGen(context);
  if (!elseV) {
    return nullptr;
  }
  context->builder()->CreateBr(mergeBlock);
  elseBlock = context->builder().GetInsertBlock();
  f->getBasicBlockList().push_back(mergeBlock);
  context->builder().SetInsertPoint(mergeBlock);
  llvm::PHINode *pn = context->builder().CreatePHI(
      llvm::Type::getDoubleTy(context->context()), 2, "iftmp");
  pn->addIncoming(thenV, thenBlock);
  pn->addIncoming(elseV, elseBlock);
  return pn;
}

std::string IrIfStatement::toString() const {
  return fmt::format("[ @IrIfStatement node_:{} ]", node_->toString());
}

/* while statement */
IrWhileStatement::IrWhileStatement(AstWhileStatement *node)
    : IrStatement(nameGen("IrWhileStatement")), node_(node) {}

IrType IrWhileStatement::type() const { return IrType::WhileStatement; }

llvm::Value *IrWhileStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrWhileStatement::toString() const {
  return fmt::format("[ @IrWhileStatement node_:{} ]", node_->toString());
}

/* for statement */
IrForStatement::IrForStatement(AstForStatement *node)
    : IrStatement(nameGen("IrForStatement")), node_(node) {}

IrType IrForStatement::type() const { return IrType::ForStatement; }

llvm::Value *IrForStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrForStatement::toString() const {
  return fmt::format("[ @IrForStatement node_:{} ]", node_->toString());
}

/* variable declaration */
IrVariableDeclaration::IrVariableDeclaration(AstVariableDeclaration *node)
    : IrDeclaration(nameGen("IrVariableDeclaration")), node_(node) {}

IrType IrVariableDeclaration::type() const {
  return IrType::VariableDeclaration;
}

llvm::Value *IrVariableDeclaration::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrVariableDeclaration::toString() const {
  return fmt::format("[ @IrVariableDeclaration node_:{} ]", node_->toString());
}

/* function declaration */
IrFunctionDeclaration::IrFunctionDeclaration(AstFunctionDeclaration *node)
    : IrDeclaration(nameGen("IrFunctionDeclaration")), node_(node),
      signature_(new IrFunctionSignatureDeclaration(node->signature())),
      statement_(nullptr) {
  switch (node_->statement()->type()) {
  case AstType::ExpressionStatement: {
    statement_ = new IrExpressionStatement(
        DC(AstExpressionStatement, node_->statement()));
  } break;
  case AstType::IfStatement: {
    statement_ = new IrIfStatement(DC(AstIfStatement, node_->statement()));
  } break;
  case AstType::WhileStatement: {
    statement_ =
        new IrWhileStatement(DC(AstWhileStatement, node_->statement()));
  } break;
  case AstType::ForStatement: {
    statement_ = new IrForStatement(DC(AstForStatement, node_->statement()));
  } break;
  case AstType::CompoundStatement: {
    statement_ =
        new IrCompoundStatement(DC(AstCompoundStatement, node_->statement()));
  } break;
  default:
    LOG_ASSERT(false, "invalid ast function body statement:{}",
               node_->statement()->toString());
  }
}

IrType IrFunctionDeclaration::type() const {
  return IrType::FunctionDeclaration;
}

llvm::Function *IrFunctionDeclaration::codeGen(IrContext *context) {
  llvm::Function *f =
      context->module()->getFunction(node_->signature()->identifier());
  if (!f) {
    f = signature_->codeGen(context);
  }
  if (!f) {
    return nullptr;
  }
  LOG_ASSERT(f->empty(), "Function {} cannot be redefined!",
             node_->signature()->identifier());
  llvm::BasicBlock *bb =
      llvm::BasicBlock::Create(context->context(), "entry", f);
  context->builder().SetInsertPoint(bb);
  context->symtable().clear();
  for (auto &a : f->args()) {
    context->symtable().insert(a.getName(), &a);
  }
  llvm::Value *ret = statement_->codeGen(context);
  if (ret) {
    context->builder().CreateRet(ret);
    llvm::verifyFunction(*f);
    return f;
  }
  f->eraseFromParent();
  return nullptr;
}

std::string IrFunctionDeclaration::toString() const {
  return fmt::format("[ @IrFunctionDeclaration node_:{} ]", node_->toString());
}

/* function signature declaration */
IrFunctionSignatureDeclaration::IrFunctionSignatureDeclaration(
    AstFunctionSignatureDeclaration *node)
    : IrDeclaration(nameGen("IrFunctionSignatureDeclaration")), node_(node) {}

std::string IrFunctionSignatureDeclaration::toString() const {
  return fmt::format("[ @IrFunctionSignatureDeclaration node_:{} ]",
                     node_->toString());
}

IrType IrFunctionSignatureDeclaration::type() const {
  return IrType::FunctionSignatureDeclaration;
}

llvm::Function *IrFunctionSignatureDeclaration::codeGen(IrContext *context) {
  AstDeclarationList *args = node_->argumentList();
  std::vector<llvm::Type *> doubleArgs(
      args ? args->size() : 0, llvm::Type::getDoubleTy(context->context()));
  // result, parameters
  llvm::FunctionType *ft = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(context->context()), doubleArgs, false);
  llvm::Function *f =
      llvm::Function::Create(ft, llvm::Function::ExternalLinkage,
                             node_->identifier(), context->module());
  // set function arg names
  int i = 0;
  for (auto &a : f->args()) {
    LOG_ASSERT(args, "args is null");
    LOG_ASSERT(args->get(i), "args->get({}) is null", i);
    AstFunctionArgumentDeclaration *ast =
        DC(AstFunctionArgumentDeclaration, args->get(i++));
    a.setName(ast->value());
  }
  return f;
}

#undef DC
