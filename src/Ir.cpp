// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "Dump.h"
#include "Log.h"
#include "NameGenerator.h"
#include "Parser.tab.hpp"
#include "container/LinkedHashMap.hpp"
#include "fmt/format.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"

static NameGenerator nameGenerator;
static NameGenerator irNameGenerator;

#define DC(x, y) dynamic_cast<x *>(y)

/* ir context */
IrContext::IrContext(const std::string &moduleName)
    : context_(), builder_(context_), module_(nullptr), symtable_() {
  module_ =
      new llvm::Module(std::string("shepherd_module_") + moduleName, context_);
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
  LOG_ASSERT(node, "node is null");
  switch (node->type()) {
  case AstType::IdentifierConstant:
    return new IrIdentifierConstant(DC(AstIdentifierConstant, node));
  case AstType::Float32Constant:
    return new IrFloat32Constant(DC(AstFloat32Constant, node));
  case AstType::Float64Constant:
    return new IrFloat64Constant(DC(AstFloat64Constant, node));
  case AstType::StringConstant:
    return new IrStringConstant(DC(AstStringConstant, node));
  case AstType::Int8Constant:
    return new IrInt8Constant(DC(AstInt8Constant, node));
  case AstType::UInt8Constant:
    return new IrUInt8Constant(DC(AstUInt8Constant, node));
  case AstType::Int16Constant:
    return new IrInt16Constant(DC(AstInt16Constant, node));
  case AstType::UInt16Constant:
    return new IrUInt16Constant(DC(AstUInt16Constant, node));
  case AstType::Int32Constant:
    return new IrInt32Constant(DC(AstInt32Constant, node));
  case AstType::UInt32Constant:
    return new IrUInt32Constant(DC(AstUInt32Constant, node));
  case AstType::Int64Constant:
    return new IrInt64Constant(DC(AstInt64Constant, node));
  case AstType::UInt64Constant:
    return new IrUInt64Constant(DC(AstUInt64Constant, node));
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

static std::string toIrNameImpl(const std::string &name,
                                const std::string prefix) {
  LOG_ASSERT(name.length() > 0, "name {} length {} <= 0", name, name.length());
  std::string tmp(name);
  for (int i = 0; i < (int)tmp.length(); i++) {
    if (tmp[i] == '_') {
      tmp[i] = '.';
    }
  }
  return prefix + tmp;
}

static std::string fromIrNameImpl(const std::string &name,
                                  const std::string prefix) {
  LOG_ASSERT(name.length() > prefix.length(),
             "name {} length {} <= prefix {} length {}", name, name.length(),
             prefix, prefix.length());
  std::string tmp(
      name.substr(prefix.length(), name.length() - prefix.length()));
  for (int i = 0; i < (int)tmp.length(); i++) {
    if (tmp[i] == '.') {
      tmp[i] = '_';
    }
  }
  return tmp;
}

#define SHP_IR std::string("shp.ir.")

std::string Ir::toIrName(const std::string &name) {
  return toIrNameImpl(name, SHP_IR);
}

std::string Ir::fromIrName(const std::string &name) {
  return fromIrNameImpl(name, SHP_IR);
}

#undef SHP_IR

IrExpression::IrExpression(const std::string &name) : Ir(name) {}

IrStatement::IrStatement(const std::string &name) : Ir(name) {}

IrDeclaration::IrDeclaration(const std::string &name) : IrStatement(name) {}

/* list */
IrExpressionList::IrExpressionList()
    : detail::IrList<IrExpression>(nameGenerator.generate("ExprList")) {}

IrType IrExpressionList::type() const { return IrType::ExpressionList; }

std::string IrExpressionList::stringify() const { return "IrExpressionList"; }

IrStatementList::IrStatementList()
    : detail::IrList<IrStatement>(nameGenerator.generate("StmtList")) {}

IrType IrStatementList::type() const { return IrType::StatementList; }

std::string IrStatementList::stringify() const { return "IrStatementList"; }

IrDeclarationList::IrDeclarationList()
    : detail::IrList<IrDeclaration>(nameGenerator.generate("DeclList")) {}

IrType IrDeclarationList::type() const { return IrType::DeclarationList; }

std::string IrDeclarationList::stringify() const { return "IrDeclarationList"; }

/* translate unit */
IrTranslateUnit::IrTranslateUnit(AstTranslateUnit *node)
    : detail::IrList<IrDeclaration>(nameGenerator.generate("TUnit")),
      node_(node) {
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
      llvm::Function *f = llvm::dyn_cast<llvm::Function>(
          DC(IrFunctionDeclaration, ir)->codeGen(context));
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
    : IrExpression(nameGenerator.generate("Id")), node_(node) {}

IrType IrIdentifierConstant::type() const { return IrType::IdentifierConstant; }

llvm::Value *IrIdentifierConstant::codeGen(IrContext *context) {
  LOG_ASSERT(context, "context is null");
  LOG_ASSERT(node_, "node_ is null");
  llvm::Value *v = context->symtable()[Ir::toIrName(node_->value())];
  llvm::Value *vec{v};
  return vec;
}

std::string IrIdentifierConstant::toString() const {
  return fmt::format("[ @IrIdentifierConstant node_:{} ]", node_->toString());
}

/* i8 constant */
IrInt8Constant::IrInt8Constant(AstInt8Constant *node)
    : IrExpression(nameGenerator.generate("IrInt8Constant")), node_(node) {}

IrType IrInt8Constant::type() const { return IrType::Int8Constant; }

llvm::Value *IrInt8Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(8, (uint64_t)node_->value(), true));
}

std::string IrInt8Constant::toString() const {
  return fmt::format("[ @IrInt8Constant node_:{} ]", node_->toString());
}

/* u8 constant */
IrUInt8Constant::IrUInt8Constant(AstUInt8Constant *node)
    : IrExpression(nameGenerator.generate("IrUInt8Constant")), node_(node) {}

IrType IrUInt8Constant::type() const { return IrType::UInt8Constant; }

llvm::Value *IrUInt8Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(8, (uint64_t)node_->value(), false));
}

std::string IrUInt8Constant::toString() const {
  return fmt::format("[ @IrUInt8Constant node_:{} ]", node_->toString());
}

/* i16 constant */
IrInt16Constant::IrInt16Constant(AstInt16Constant *node)
    : IrExpression(nameGenerator.generate("IrInt16Constant")), node_(node) {}

IrType IrInt16Constant::type() const { return IrType::Int16Constant; }

llvm::Value *IrInt16Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(16, (uint64_t)node_->value(), true));
}

std::string IrInt16Constant::toString() const {
  return fmt::format("[ @IrInt16Constant node_:{} ]", node_->toString());
}

/* u16 constant */
IrUInt16Constant::IrUInt16Constant(AstUInt16Constant *node)
    : IrExpression(nameGenerator.generate("IrUInt16Constant")), node_(node) {}

IrType IrUInt16Constant::type() const { return IrType::UInt16Constant; }

llvm::Value *IrUInt16Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(16, (uint64_t)node_->value(), false));
}

std::string IrUInt16Constant::toString() const {
  return fmt::format("[ @IrUInt16Constant node_:{} ]", node_->toString());
}

/* i32 constant */
IrInt32Constant::IrInt32Constant(AstInt32Constant *node)
    : IrExpression(nameGenerator.generate("IrInt32Constant")), node_(node) {}

IrType IrInt32Constant::type() const { return IrType::Int32Constant; }

llvm::Value *IrInt32Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(32, (uint64_t)node_->value(), true));
}

std::string IrInt32Constant::toString() const {
  return fmt::format("[ @IrInt32Constant node_:{} ]", node_->toString());
}

/* u32 constant */
IrUInt32Constant::IrUInt32Constant(AstUInt32Constant *node)
    : IrExpression(nameGenerator.generate("IrUInt32Constant")), node_(node) {}

IrType IrUInt32Constant::type() const { return IrType::UInt32Constant; }

llvm::Value *IrUInt32Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(32, (uint64_t)node_->value(), false));
}

std::string IrUInt32Constant::toString() const {
  return fmt::format("[ @IrUInt32Constant node_:{} ]", node_->toString());
}

/* i64 constant */
IrInt64Constant::IrInt64Constant(AstInt64Constant *node)
    : IrExpression(nameGenerator.generate("IrInt64Constant")), node_(node) {}

IrType IrInt64Constant::type() const { return IrType::Int64Constant; }

llvm::Value *IrInt64Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(64, (uint64_t)node_->value(), true));
}

std::string IrInt64Constant::toString() const {
  return fmt::format("[ @IrInt64Constant node_:{} ]", node_->toString());
}

/* u64 constant */
IrUInt64Constant::IrUInt64Constant(AstUInt64Constant *node)
    : IrExpression(nameGenerator.generate("IrUInt64Constant")), node_(node) {}

IrType IrUInt64Constant::type() const { return IrType::UInt64Constant; }

llvm::Value *IrUInt64Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(64, (uint64_t)node_->value(), false));
}

std::string IrUInt64Constant::toString() const {
  return fmt::format("[ @IrUInt64Constant node_:{} ]", node_->toString());
}

/* f32 constant */
IrFloat32Constant::IrFloat32Constant(AstFloat32Constant *node)
    : IrExpression(nameGenerator.generate("IrFloat32Constant")), node_(node) {}

IrType IrFloat32Constant::type() const { return IrType::Float32Constant; }

llvm::Value *IrFloat32Constant::codeGen(IrContext *context) {
  return llvm::ConstantFP::get(context->context(),
                               llvm::APFloat((float)node_->value()));
}

std::string IrFloat32Constant::toString() const {
  return fmt::format("[ @IrFloat32Constant node_:{} ]", node_->toString());
}

/* f64 constant */
IrFloat64Constant::IrFloat64Constant(AstFloat64Constant *node)
    : IrExpression(nameGenerator.generate("IrFloat64Constant")), node_(node) {}

IrType IrFloat64Constant::type() const { return IrType::Float64Constant; }

llvm::Value *IrFloat64Constant::codeGen(IrContext *context) {
  return llvm::ConstantFP::get(context->context(),
                               llvm::APFloat((double)node_->value()));
}

std::string IrFloat64Constant::toString() const {
  return fmt::format("[ @IrFloat64Constant node_:{} ]", node_->toString());
}

/* string constant */
IrStringConstant::IrStringConstant(AstStringConstant *node)
    : IrExpression(nameGenerator.generate("IrStringConstant")), node_(node) {}

IrType IrStringConstant::type() const { return IrType::StringConstant; }

llvm::Value *IrStringConstant::codeGen(IrContext *context) { return nullptr; }

std::string IrStringConstant::toString() const {
  return fmt::format("[ @IrStringConstant node_:{} ]", node_->toString());
}

/* boolean constant */
IrBooleanConstant::IrBooleanConstant(AstBooleanConstant *node)
    : IrExpression(nameGenerator.generate("IrBooleanConstant")), node_(node) {}

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
    : IrExpression(nameGenerator.generate("IrCallExpression")), node_(node) {}

IrType IrCallExpression::type() const { return IrType::CallExpression; }

llvm::Value *IrCallExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrCallExpression::toString() const {
  return fmt::format("[ @IrCallExpression node_:{} ]", node_->toString());
}

/* unary expression */
IrUnaryExpression::IrUnaryExpression(AstUnaryExpression *node)
    : IrExpression(nameGenerator.generate("IrUnaryExpression")), node_(node) {}

IrType IrUnaryExpression::type() const { return IrType::UnaryExpression; }

llvm::Value *IrUnaryExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrUnaryExpression::toString() const {
  return fmt::format("[ @IrUnaryExpression node_:{} ]", node_->toString());
}

/* binary expression */
IrBinaryExpression::IrBinaryExpression(AstBinaryExpression *node)
    : IrExpression(nameGenerator.generate("IrBinaryExpression")), node_(node),
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
      return context->builder().CreateFAdd(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFAdd(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateAdd(l, r);
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
      return context->builder().CreateFSub(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFSub(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateSub(l, r);
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
      return context->builder().CreateFMul(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFMul(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateMul(l, r);
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
      return context->builder().CreateFDiv(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFDiv(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateSDiv(l, r);
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
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateFRem(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFRem(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateSRem(l, r);
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
  case T_BIT_LSHIFT: {
    LOG_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "l->getType->getTypeID {} not integer",
               l->getType()->getTypeID());
    LOG_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "r->getType->getTypeID {} not integer",
               r->getType()->getTypeID());
    return context->builder().CreateShl(l, r);
  } break;
  case T_BIT_RSHIFT: {
    LOG_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "l->getType->getTypeID {} not integer",
               l->getType()->getTypeID());
    LOG_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "r->getType->getTypeID {} not integer",
               r->getType()->getTypeID());
    return context->builder().CreateLShr(l, r);
  } break;
  case T_BIT_ARSHIFT: {
    LOG_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "l->getType->getTypeID {} not integer",
               l->getType()->getTypeID());
    LOG_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "r->getType->getTypeID {} not integer",
               r->getType()->getTypeID());
    return context->builder().CreateAShr(l, r);
  } break;
  case T_EQ: {
    LOG_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
               "l->getType->getTypeID {} != r->getType->getTypeID {}",
               l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateFCmpOEQ(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context->builder().CreateICmpEQ(l, r);
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_NEQ: {
    LOG_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
               "l->getType->getTypeID {} != r->getType->getTypeID {}",
               l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateFCmpONE(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context->builder().CreateICmpNE(l, r);
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_LE: {
    LOG_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
               "l->getType->getTypeID {} != r->getType->getTypeID {}",
               l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateFCmpOLE(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context->builder().CreateICmpSLE(l, r);
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_LT: {
    LOG_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
               "l->getType->getTypeID {} != r->getType->getTypeID {}",
               l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateFCmpOLT(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context->builder().CreateICmpSLT(l, r);
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_GE: {
    LOG_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
               "l->getType->getTypeID {} != r->getType->getTypeID {}",
               l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateFCmpOGE(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context->builder().CreateICmpSGE(l, r);
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_GT: {
    LOG_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
               "l->getType->getTypeID {} != r->getType->getTypeID {}",
               l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context->builder().CreateFCmpOGT(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context->builder().CreateICmpSGT(l, r);
    } break;
    default:
      LOG_ASSERT(false, "l->getType->getTypeID {} invalid",
                 l->getType()->getTypeID());
    }
  } break;
  case T_BIT_AND: {
    LOG_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "l->getType->getTypeID {} not integer",
               l->getType()->getTypeID());
    LOG_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "r->getType->getTypeID {} not integer",
               r->getType()->getTypeID());
    return context->builder().CreateAnd(l, r, "andtmp");
  } break;
  case T_BIT_OR: {
    LOG_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "l->getType->getTypeID {} not integer",
               l->getType()->getTypeID());
    LOG_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "r->getType->getTypeID {} not integer",
               r->getType()->getTypeID());
    return context->builder().CreateOr(l, r, "ortmp");
  } break;
  case T_BIT_XOR: {
    LOG_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "l->getType->getTypeID {} not integer",
               l->getType()->getTypeID());
    LOG_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "r->getType->getTypeID {} not integer",
               r->getType()->getTypeID());
    return context->builder().CreateXor(l, r, "xortmp");
  } break;
  case T_LOGIC_AND: {
    LOG_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "l->getType->getTypeID {} not integer",
               l->getType()->getTypeID());
    LOG_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "r->getType->getTypeID {} not integer",
               r->getType()->getTypeID());
    llvm::ConstantInt *lc = llvm::dyn_cast<llvm::ConstantInt>(l);
    llvm::ConstantInt *rc = llvm::dyn_cast<llvm::ConstantInt>(r);
    LOG_ASSERT(lc->getBitWidth() == 1, "lc->getBitWidth {} != 1",
               lc->getBitWidth());
    LOG_ASSERT(rc->getBitWidth() == 1, "rc->getBitWidth {} != 1",
               rc->getBitWidth());
    return context->builder().CreateAnd(l, r, "logic_andtmp");
  } break;
  case T_LOGIC_OR: {
    LOG_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "l->getType->getTypeID {} not integer",
               l->getType()->getTypeID());
    LOG_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
               "r->getType->getTypeID {} not integer",
               r->getType()->getTypeID());
    llvm::ConstantInt *lc = llvm::dyn_cast<llvm::ConstantInt>(l);
    llvm::ConstantInt *rc = llvm::dyn_cast<llvm::ConstantInt>(r);
    LOG_ASSERT(lc->getBitWidth() == 1, "lc->getBitWidth {} != 1",
               lc->getBitWidth());
    LOG_ASSERT(rc->getBitWidth() == 1, "rc->getBitWidth {} != 1",
               rc->getBitWidth());
    return context->builder().CreateOr(l, r, "logic_ortmp");
  } break;
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
    : IrExpression(nameGenerator.generate("IrConditionalExpression")),
      node_(node) {}

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
    : IrExpression(nameGenerator.generate("IrAssignmentExpression")),
      node_(node) {}

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
    : IrExpression(nameGenerator.generate("IrSequelExpression")), node_(node) {}

IrType IrSequelExpression::type() const { return IrType::SequelExpression; }

llvm::Value *IrSequelExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrSequelExpression::toString() const {
  return fmt::format("[ @IrSequelExpression node_:{} ]", node_->toString());
}

/* expression statement */
IrExpressionStatement::IrExpressionStatement(AstExpressionStatement *node)
    : IrStatement(nameGenerator.generate("IrExpressionStatement")), node_(node),
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
  case IrType::Int8Constant:
    return DC(IrInt8Constant, expr_)->codeGen(context);
  case IrType::UInt8Constant:
    return DC(IrUInt8Constant, expr_)->codeGen(context);
  case IrType::Int16Constant:
    return DC(IrInt16Constant, expr_)->codeGen(context);
  case IrType::UInt16Constant:
    return DC(IrUInt16Constant, expr_)->codeGen(context);
  case IrType::Int32Constant:
    return DC(IrInt32Constant, expr_)->codeGen(context);
  case IrType::UInt32Constant:
    return DC(IrUInt32Constant, expr_)->codeGen(context);
  case IrType::Int64Constant:
    return DC(IrInt64Constant, expr_)->codeGen(context);
  case IrType::UInt64Constant:
    return DC(IrUInt64Constant, expr_)->codeGen(context);
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
    : IrStatement(nameGenerator.generate("IrCompoundStatement")), node_(node),
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
    : IrStatement(nameGenerator.generate("IrIfStatement")), node_(node),
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
  LOG_ASSERT(condV, "condV is null");
  condV = context->builder().CreateICmpNE(
      condV,
      llvm::ConstantInt::get(context->context(),
                             llvm::APInt(1, (uint64_t)0, false)),
      irNameGenerator.generate("ifcond"));
  llvm::Function *f = context->builder().GetInsertBlock()->getParent();
  llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(
      context->context(), irNameGenerator.generate("then"), f);
  llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(
      context->context(), irNameGenerator.generate("else"));
  llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(
      context->context(), irNameGenerator.generate("ifcont"));
  context->builder().CreateCondBr(condV, thenBlock, elseBlock);
  context->builder().SetInsertPoint(thenBlock);
  llvm::Value *thenV = thens_->codeGen(context);
  LOG_ASSERT(thenV, "thenV is null");
  context->builder().CreateBr(mergeBlock);
  thenBlock = context->builder().GetInsertBlock();
  f->getBasicBlockList().push_back(elseBlock);
  context->builder().SetInsertPoint(elseBlock);
  llvm::Value *elseV = elses_->codeGen(context);
  LOG_ASSERT(elseV, "elseV is null");
  context->builder().CreateBr(mergeBlock);
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
    : IrStatement(nameGenerator.generate("IrWhileStatement")), node_(node) {}

IrType IrWhileStatement::type() const { return IrType::WhileStatement; }

llvm::Value *IrWhileStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrWhileStatement::toString() const {
  return fmt::format("[ @IrWhileStatement node_:{} ]", node_->toString());
}

/* for statement */
IrForStatement::IrForStatement(AstForStatement *node)
    : IrStatement(nameGenerator.generate("IrForStatement")), node_(node),
      start_(DC(IrStatement, createIr(node->start()))),
      step_(DC(IrStatement, createIr(node->step()))),
      end_(DC(IrStatement, createIr(node->end()))),
      statement_(DC(IrStatement, createIr(node->statement()))) {}

IrForStatement::~IrForStatement() {
  delete start_;
  start_ = nullptr;
  delete step_;
  step_ = nullptr;
  delete end_;
  end_ = nullptr;
  delete statement_;
  statement_ = nullptr;
}

IrType IrForStatement::type() const { return IrType::ForStatement; }

llvm::Value *IrForStatement::codeGen(IrContext *context) {
  llvm::Value *startV = start_->codeGen(context);
  LOG_ASSERT(startV, "startV is null");
  llvm::Function *f = context->builder().GetInsertBlock()->getParent();
  llvm::BasicBlock *preheaderBB = context->builder().GetInsertBlock();
  llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(
      context->context(), irNameGenerator.generate("loop"), f);
  context->builder().CreateBr(loopBB);
  context->builder().SetInsertPoint(loopBB);

  llvm::PHINode *variable =
      context->builder().CreatePHI(llvm::Type::getDoubleTy(context->context()),
                                   2, irNameGenerator.generate("loop.phi"));
  variable->addIncoming(startV, preheaderBB);
  llvm::Value *bodyV = statement_->codeGen(context);
  LOG_ASSERT(bodyV, "bodyV is null");
  llvm::Value *stepV = nullptr;
  if (step_) {
    stepV = step_->codeGen(context);
    LOG_ASSERT(stepV, "stepV is null");
  } else {
    stepV = llvm::ConstantInt::get(context->context(),
                                   llvm::APInt(1, (uint64_t)1, false));
  }
  llvm::Value *nextV = context->builder().CreateAdd(
      variable, stepV, irNameGenerator.generate("nextvar"));
  llvm::Value *endCond = end_->codeGen(context);
  LOG_ASSERT(endCond, "endCond is null");
  endCond = context->builder().CreateICmpNE(
      endCond, llvm::ConstantInt::get(context->context(),
                                      llvm::APInt(1, (uint64_t)0, false)));
  llvm::BasicBlock *loopEndBB = context->builder().GetInsertBlock();
  llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(
      context->context(), irNameGenerator.generate("afterloop"), f);
  context->builder().CreateCondBr(endCond, loopBB, afterBB);
  context->builder().SetInsertPoint(afterBB);
  variable->addIncoming(nextV, loopEndBB);
  return llvm::Constant::getNullValue(
      llvm::Type::getDoubleTy(context->context()));
}

std::string IrForStatement::toString() const {
  return fmt::format("[ @IrForStatement node_:{} ]", node_->toString());
}

/* variable declaration */
IrVariableDeclaration::IrVariableDeclaration(AstVariableDeclaration *node)
    : IrDeclaration(nameGenerator.generate("IrVariableDeclaration")),
      node_(node) {}

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
    : IrDeclaration(nameGenerator.generate("IrFunctionDeclaration")),
      node_(node),
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

llvm::Value *IrFunctionDeclaration::codeGen(IrContext *context) {
  llvm::Function *f = context->module()->getFunction(
      Ir::toIrName(node_->signature()->identifier()));
  if (!f) {
    f = llvm::dyn_cast<llvm::Function>(signature_->codeGen(context));
  }
  if (!f) {
    return nullptr;
  }
  LOG_ASSERT(f->empty(), "Function {} cannot be redefined!",
             node_->signature()->identifier());
  llvm::BasicBlock *bb =
      llvm::BasicBlock::Create(context->context(), "shp.ir.func.entry", f);
  context->builder().SetInsertPoint(bb);
  context->symtable().clear();
  for (auto &a : f->args()) {
    context->symtable().insert(a.getName(), &a);
  }
  llvm::Value *ret = statement_->codeGen(context);
  if (ret) {
    context->builder().CreateRet(ret);
    llvm::verifyFunction(*f);
    return llvm::dyn_cast<llvm::Value>(f);
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
    : IrDeclaration(nameGenerator.generate("IrFunctionSignatureDeclaration")),
      node_(node) {}

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
      args ? args->size() : 0, llvm::Type::getDoubleTy(context->context()));
  // result, parameters
  llvm::FunctionType *ft = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(context->context()), doubleArgs, false);
  llvm::Function *f = llvm::Function::Create(
      ft, llvm::Function::ExternalLinkage, Ir::toIrName(node_->identifier()),
      context->module());
  // set function arg names
  int i = 0;
  for (auto &a : f->args()) {
    LOG_ASSERT(args, "args is null");
    LOG_ASSERT(args->get(i), "args->get({}) is null", i);
    AstFunctionArgumentDeclaration *ast =
        DC(AstFunctionArgumentDeclaration, args->get(i++));
    a.setName(Ir::toIrName(ast->value()));
  }
  return llvm::dyn_cast<llvm::Value>(f);
}

#undef DC
