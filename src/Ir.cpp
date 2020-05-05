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

LinkedHashMap<std::string, llvm::Value *> &IrContext::symtable() {
  return symtable_;
}

const LinkedHashMap<std::string, llvm::Value *> &IrContext::symtable() const {
  return symtable_;
}

/* interface */
Ir::Ir(const std::string &name) : name_(name) {}

std::string Ir::name() const { return name_; }

IrExpression::IrExpression(const std::string &name) : Ir(name) {}

IrStatement::IrStatement(const std::string &name) : Ir(name) {}

IrDeclaration::IrDeclaration(const std::string &name) : IrStatement(name) {}

/* list */
IrExpressionList::IrExpressionList()
    : detail::IrList<IrExpression>(nameGen("ExprList")) {}

IrType IrExpressionList::type() const { return IrType::ExpressionList; }

std::vector<llvm::Value *> IrExpressionList::codeGen(IrContext *context) {
  LOG_ASSERT(context, "context is null");
  std::vector<llvm::Value *> vec;
  for (int i = 0; i < size(); i++) {
    std::vector<llvm::Value *> tmp = get(i)->codeGen(context);
    vec.reserve(vec.size() + tmp.size());
    vec.insert(vec.end(), tmp.begin(), tmp.end());
  }
  return vec;
}

std::string IrExpressionList::stringify() const { return "IrExpressionList"; }

IrStatementList::IrStatementList()
    : detail::IrList<IrStatement>(nameGen("StmtList")) {}

IrType IrStatementList::type() const { return IrType::StatementList; }

std::vector<llvm::Value *> IrStatementList::codeGen(IrContext *context) {
  LOG_ASSERT(context, "context is null");
  std::vector<llvm::Value *> vec;
  for (int i = 0; i < size(); i++) {
    std::vector<llvm::Value *> tmp = get(i)->codeGen(context);
    vec.reserve(vec.size() + tmp.size());
    vec.insert(vec.end(), tmp.begin(), tmp.end());
  }
  return vec;
}

std::string IrStatementList::stringify() const { return "IrStatementList"; }

IrDeclarationList::IrDeclarationList()
    : detail::IrList<IrDeclaration>(nameGen("DeclList")) {}

IrType IrDeclarationList::type() const { return IrType::DeclarationList; }

std::vector<llvm::Value *> IrDeclarationList::codeGen(IrContext *context) {
  LOG_ASSERT(context, "context is null");
  std::vector<llvm::Value *> vec;
  for (int i = 0; i < size(); i++) {
    std::vector<llvm::Value *> tmp = get(i)->codeGen(context);
    vec.reserve(vec.size() + tmp.size());
    vec.insert(vec.end(), tmp.begin(), tmp.end());
  }
  return vec;
}

std::string IrDeclarationList::stringify() const { return "IrDeclarationList"; }

/* translate unit */
IrTranslateUnit::IrTranslateUnit(AstTranslateUnit *node)
    : detail::IrList<IrDeclaration>(nameGen("TUnit")), node_(node) {
  for (int i = 0; i < node_->size(); i++) {
    AstDeclaration *ast = node_->get(i);
    switch (ast->type()) {
    case AstType::VariableDeclaration:
      add(DC(IrDeclaration,
             new IrVariableDeclaration(DC(AstVariableDeclaration, ast))));
      break;
    case AstType::FunctionDeclaration:
      add(DC(IrDeclaration,
             new IrFunctionDeclaration(DC(AstFunctionDeclaration, ast))));
      break;
    default:
      LOG_ASSERT(false, "invalid ast:{}", ast->toString());
    }
  }
}

IrType IrTranslateUnit::type() const { return IrType::TranslateUnit; }

std::vector<llvm::Value *> IrTranslateUnit::codeGen(IrContext *context) {
  LOG_ASSERT(context, "context is null");
  std::vector<llvm::Value *> vec;
  for (int i = 0; i < size(); i++) {
    std::vector<llvm::Value *> tmp = get(i)->codeGen(context);
    vec.reserve(vec.size() + tmp.size());
    vec.insert(vec.end(), tmp.begin(), tmp.end());
  }
  return vec;
}

std::string IrTranslateUnit::stringify() const { return "IrTranslateUnit"; }

/* identifier constant */
IrIdentifierConstant::IrIdentifierConstant(AstIdentifierConstant *node)
    : Ir(nameGen("Id")), node_(node) {}

IrType IrIdentifierConstant::type() const { return IrType::IdentifierConstant; }

std::vector<llvm::Value *> IrIdentifierConstant::codeGen(IrContext *context) {
  LOG_ASSERT(context, "context is null");
  LOG_ASSERT(node_, "node_ is null");
  llvm::Value *v = context->symtable()[node_->value()];
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrIdentifierConstant::toString() const {
  return fmt::format("[ @IrIdentifierConstant node_:{} ]", node_->toString());
}

/* i8 constant */
IrI8Constant::IrI8Constant(AstI8Constant *node)
    : Ir(nameGen("IrI8Constant")), node_(node) {}

IrType IrI8Constant::type() const { return IrType::I8Constant; }

std::vector<llvm::Value *> IrI8Constant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantInt::get(
      context->context(), llvm::APInt(8, (uint64_t)node_->value(), true));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrI8Constant::toString() const {
  return fmt::format("[ @IrI8Constant node_:{} ]", node_->toString());
}

/* u8 constant */
IrU8Constant::IrU8Constant(AstU8Constant *node)
    : Ir(nameGen("IrU8Constant")), node_(node) {}

IrType IrU8Constant::type() const { return IrType::U8Constant; }

std::vector<llvm::Value *> IrU8Constant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantInt::get(
      context->context(), llvm::APInt(8, (uint64_t)node_->value(), false));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrU8Constant::toString() const {
  return fmt::format("[ @IrU8Constant node_:{} ]", node_->toString());
}

/* i16 constant */
IrI16Constant::IrI16Constant(AstI16Constant *node)
    : Ir(nameGen("IrI16Constant")), node_(node) {}

IrType IrI16Constant::type() const { return IrType::I16Constant; }

std::vector<llvm::Value *> IrI16Constant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantInt::get(
      context->context(), llvm::APInt(16, (uint64_t)node_->value(), true));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrI16Constant::toString() const {
  return fmt::format("[ @IrI16Constant node_:{} ]", node_->toString());
}

/* u16 constant */
IrU16Constant::IrU16Constant(AstU16Constant *node)
    : Ir(nameGen("IrU16Constant")), node_(node) {}

IrType IrU16Constant::type() const { return IrType::U16Constant; }

std::vector<llvm::Value *> IrU16Constant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantInt::get(
      context->context(), llvm::APInt(16, (uint64_t)node_->value(), false));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrU16Constant::toString() const {
  return fmt::format("[ @IrU16Constant node_:{} ]", node_->toString());
}

/* i32 constant */
IrI32Constant::IrI32Constant(AstI32Constant *node)
    : Ir(nameGen("IrI32Constant")), node_(node) {}

IrType IrI32Constant::type() const { return IrType::I32Constant; }

std::vector<llvm::Value *> IrI32Constant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantInt::get(
      context->context(), llvm::APInt(32, (uint64_t)node_->value(), true));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrI32Constant::toString() const {
  return fmt::format("[ @IrI32Constant node_:{} ]", node_->toString());
}

/* u32 constant */
IrU32Constant::IrU32Constant(AstU32Constant *node)
    : Ir(nameGen("IrU32Constant")), node_(node) {}

IrType IrU32Constant::type() const { return IrType::U32Constant; }

std::vector<llvm::Value *> IrU32Constant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantInt::get(
      context->context(), llvm::APInt(32, (uint64_t)node_->value(), false));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrU32Constant::toString() const {
  return fmt::format("[ @IrU32Constant node_:{} ]", node_->toString());
}

/* i64 constant */
IrI64Constant::IrI64Constant(AstI64Constant *node)
    : Ir(nameGen("IrI64Constant")), node_(node) {}

IrType IrI64Constant::type() const { return IrType::I64Constant; }

std::vector<llvm::Value *> IrI64Constant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantInt::get(
      context->context(), llvm::APInt(64, (uint64_t)node_->value(), true));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrI64Constant::toString() const {
  return fmt::format("[ @IrI64Constant node_:{} ]", node_->toString());
}

/* u64 constant */
IrU64Constant::IrU64Constant(AstU64Constant *node)
    : Ir(nameGen("IrU64Constant")), node_(node) {}

IrType IrU64Constant::type() const { return IrType::U64Constant; }

std::vector<llvm::Value *> IrU64Constant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantInt::get(
      context->context(), llvm::APInt(64, (uint64_t)node_->value(), false));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrU64Constant::toString() const {
  return fmt::format("[ @IrU64Constant node_:{} ]", node_->toString());
}

/* f32 constant */
IrF32Constant::IrF32Constant(AstF32Constant *node)
    : Ir(nameGen("IrF32Constant")), node_(node) {}

IrType IrF32Constant::type() const { return IrType::F32Constant; }

std::vector<llvm::Value *> IrF32Constant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantFP::get(context->context(),
                                         llvm::APFloat((float)node_->value()));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrF32Constant::toString() const {
  return fmt::format("[ @IrF32Constant node_:{} ]", node_->toString());
}

/* f64 constant */
IrF64Constant::IrF64Constant(AstF64Constant *node)
    : Ir(nameGen("IrF64Constant")), node_(node) {}

IrType IrF64Constant::type() const { return IrType::F64Constant; }

std::vector<llvm::Value *> IrF64Constant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantFP::get(context->context(),
                                         llvm::APFloat((double)node_->value()));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrF64Constant::toString() const {
  return fmt::format("[ @IrF64Constant node_:{} ]", node_->toString());
}

/* string constant */
IrStringConstant::IrStringConstant(AstStringConstant *node)
    : Ir(nameGen("IrStringConstant")), node_(node) {}

IrType IrStringConstant::type() const { return IrType::StringConstant; }

std::vector<llvm::Value *> IrStringConstant::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrStringConstant::toString() const {
  return fmt::format("[ @IrStringConstant node_:{} ]", node_->toString());
}

/* boolean constant */
IrBooleanConstant::IrBooleanConstant(AstBooleanConstant *node)
    : Ir(nameGen("IrBooleanConstant")), node_(node) {}

IrType IrBooleanConstant::type() const { return IrType::BooleanConstant; }

std::vector<llvm::Value *> IrBooleanConstant::codeGen(IrContext *context) {
  llvm::Value *v = llvm::ConstantInt::get(
      context->context(),
      llvm::APInt(1, node_->value() ? (uint64_t)1U : (uint64_t)0U, false));
  std::vector<llvm::Value *> vec{v};
  return vec;
}

std::string IrBooleanConstant::toString() const {
  return fmt::format("[ @IrBooleanConstant node_:{} ]", node_->toString());
}

/* call expression */
IrCallExpression::IrCallExpression(AstCallExpression *node)
    : Ir(nameGen("IrCallExpression")), node_(node) {}

IrType IrCallExpression::type() const { return IrType::CallExpression; }

std::vector<llvm::Value *> IrCallExpression::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrCallExpression::toString() const {
  return fmt::format("[ @IrCallExpression node_:{} ]", node_->toString());
}

/* unary expression */
IrUnaryExpression::IrUnaryExpression(AstUnaryExpression *node)
    : Ir(nameGen("IrUnaryExpression")), node_(node) {}

IrType IrUnaryExpression::type() const { return IrType::UnaryExpression; }

std::vector<llvm::Value *> IrUnaryExpression::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

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

std::vector<llvm::Value *> IrBinaryExpression::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  std::vector<llvm::Value *> lv = left_->codeGen(context);
  std::vector<llvm::Value *> rv = right_->codeGen(context);
  LOG_ASSERT((int)lv.size() == 1, "lv.size() {} != 1", lv.size());
  LOG_ASSERT((int)rv.size() == 1, "rv.size() {} != 1", rv.size());
  llvm::Value *l = lv[0];
  llvm::Value *r = rv[0];
  LOG_ASSERT(l, "l is null");
  LOG_ASSERT(r, "r is null");
  switch (node_->token()) {
  case T_ADD: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      vec.push_back(context->builder().CreateFAdd(l, r, "addtmp"));
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        vec.push_back(context->builder().CreateFAdd(l, r, "addtmp"));
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        vec.push_back(context->builder().CreateAdd(l, r, "addtmp"));
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
      vec.push_back(context->builder().CreateFSub(l, r, "subtmp"));
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        vec.push_back(context->builder().CreateFSub(l, r, "subtmp"));
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        vec.push_back(context->builder().CreateSub(l, r, "subtmp"));
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
      vec.push_back(context->builder().CreateFMul(l, r, "multmp"));
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        vec.push_back(context->builder().CreateFMul(l, r, "multmp"));
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        vec.push_back(context->builder().CreateMul(l, r, "multmp"));
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
      vec.push_back(context->builder().CreateSDiv(l, r, "divtmp"));
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        vec.push_back(context->builder().CreateSDiv(l, r, "divtmp"));
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        vec.push_back(context->builder().CreateSDiv(l, r, "divtmp"));
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
        vec.push_back(context->builder().CreateSub(l, dr, "subtmp"));
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
  return vec;
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

std::vector<llvm::Value *>
IrConditionalExpression::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
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

std::vector<llvm::Value *> IrAssignmentExpression::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrAssignmentExpression::toString() const {
  return fmt::format("[ @IrAssignmentExpression node_:{} ]", node_->toString());
}

/* sequel expression */
IrSequelExpression::IrSequelExpression(AstSequelExpression *node)
    : Ir(nameGen("IrSequelExpression")), node_(node) {}

IrType IrSequelExpression::type() const { return IrType::SequelExpression; }

std::vector<llvm::Value *> IrSequelExpression::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrSequelExpression::toString() const {
  return fmt::format("[ @IrSequelExpression node_:{} ]", node_->toString());
}

/* expression statement */
IrExpressionStatement::IrExpressionStatement(AstExpressionStatement *node)
    : Ir(nameGen("IrExpressionStatement")), node_(node) {}

IrType IrExpressionStatement::type() const {
  return IrType::ExpressionStatement;
}

std::vector<llvm::Value *> IrExpressionStatement::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrExpressionStatement::toString() const {
  return fmt::format("[ @IrExpressionStatement node_:{} ]", node_->toString());
}

/* compound statement */
IrCompoundStatement::IrCompoundStatement(AstCompoundStatement *node)
    : Ir(nameGen("IrCompoundStatement")), node_(node) {}

IrType IrCompoundStatement::type() const { return IrType::CompoundStatement; }

std::vector<llvm::Value *> IrCompoundStatement::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrCompoundStatement::toString() const {
  return fmt::format("[ @IrCompoundStatement node_:{} ]", node_->toString());
}

/* if statement */
IrIfStatement::IrIfStatement(AstIfStatement *node)
    : Ir(nameGen("IrIfStatement")), node_(node) {}

IrType IrIfStatement::type() const { return IrType::IfStatement; }

std::vector<llvm::Value *> IrIfStatement::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrIfStatement::toString() const {
  return fmt::format("[ @IrIfStatement node_:{} ]", node_->toString());
}

/* while statement */
IrWhileStatement::IrWhileStatement(AstWhileStatement *node)
    : Ir(nameGen("IrWhileStatement")), node_(node) {}

IrType IrWhileStatement::type() const { return IrType::WhileStatement; }

std::vector<llvm::Value *> IrWhileStatement::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrWhileStatement::toString() const {
  return fmt::format("[ @IrWhileStatement node_:{} ]", node_->toString());
}

/* for statement */
IrForStatement::IrForStatement(AstForStatement *node)
    : Ir(nameGen("IrForStatement")), node_(node) {}

IrType IrForStatement::type() const { return IrType::ForStatement; }

std::vector<llvm::Value *> IrForStatement::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrForStatement::toString() const {
  return fmt::format("[ @IrForStatement node_:{} ]", node_->toString());
}

/* continue statement */
IrContinueStatement::IrContinueStatement(AstContinueStatement *node)
    : Ir(nameGen("IrContinueStatement")), node_(node) {}

IrType IrContinueStatement::type() const { return IrType::ContinueStatement; }

std::vector<llvm::Value *> IrContinueStatement::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrContinueStatement::toString() const {
  return fmt::format("[ @IrContinueStatement node_:{} ]", node_->toString());
}

/* break statement */
IrBreakStatement::IrBreakStatement(AstBreakStatement *node)
    : Ir(nameGen("IrBreakStatement")), node_(node) {}

IrType IrBreakStatement::type() const { return IrType::BreakStatement; }

std::vector<llvm::Value *> IrBreakStatement::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrBreakStatement::toString() const {
  return fmt::format("[ @IrBreakStatement node_:{} ]", node_->toString());
}

/* return statement */
IrReturnStatement::IrReturnStatement(AstReturnStatement *node)
    : Ir(nameGen("IrReturnStatement")), node_(node) {}

IrType IrReturnStatement::type() const { return IrType::ReturnStatement; }

std::vector<llvm::Value *> IrReturnStatement::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrReturnStatement::toString() const {
  return fmt::format("[ @IrReturnStatement node_:{} ]", node_->toString());
}

/* empty statement */
IrEmptyStatement::IrEmptyStatement(AstEmptyStatement *node)
    : Ir(nameGen("IrEmptyStatement")), node_(node) {}

IrType IrEmptyStatement::type() const { return IrType::EmptyStatement; }

std::vector<llvm::Value *> IrEmptyStatement::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrEmptyStatement::toString() const {
  return fmt::format("[ @IrEmptyStatement node_:{} ]", node_->toString());
}

/* variable declaration */
IrVariableDeclaration::IrVariableDeclaration(AstVariableDeclaration *node)
    : Ir(nameGen("IrVariableDeclaration")), node_(node) {}

IrType IrVariableDeclaration::type() const {
  return IrType::VariableDeclaration;
}

std::vector<llvm::Value *> IrVariableDeclaration::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
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

std::vector<llvm::Value *>
IrVariableInitialDeclaration::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
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

std::vector<llvm::Value *> IrFunctionDeclaration::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
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

std::vector<llvm::Value *>
IrFunctionSignatureDeclaration::codeGen(IrContext *context) {
  AstDeclarationList *args = node_->argumentList();
  std::vector<llvm::Type *> doubleArgs(
      args->size(), llvm::Type::getDoubleTy(context->context()));
  // result, parameters
  llvm::FunctionType *ft = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(context->context()), doubleArgs, false);
  llvm::Function *f =
      llvm::Function::Create(ft, llvm::Function::ExternalLinkage,
                             node_->identifier(), context->module());
  std::vector<llvm::Value *> vec{f};
  return vec;
}

/* function argument declaration */
IrFunctionArgumentDeclaration::IrFunctionArgumentDeclaration(
    AstFunctionArgumentDeclaration *node)
    : Ir(nameGen("IrFunctionArgumentDeclaration")), node_(node) {}

IrType IrFunctionArgumentDeclaration::type() const {
  return IrType::FunctionArgumentDeclaration;
}

std::vector<llvm::Value *>
IrFunctionArgumentDeclaration::codeGen(IrContext *context) {
  std::vector<llvm::Value *> vec;
  return vec;
}

std::string IrFunctionArgumentDeclaration::toString() const {
  return fmt::format("[ @IrFunctionArgumentDeclaration node_:{} ]",
                     node_->toString());
}

#undef DC
