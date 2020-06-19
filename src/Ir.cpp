// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "Dump.h"
#include "NameGenerator.h"
#include "Parser.tab.hpp"
#include "container/LinkedHashMap.hpp"
#include "fmt/core.h"
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
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

static NameGenerator nameGenerator;
static NameGenerator irNameGenerator;

#define DC(x, y) dynamic_cast<x *>(y)

/* ir context */
IrContext::IrContext(const std::string &moduleName)
    : moduleName_(moduleName), context_(), builder_(context_), module_(nullptr),
      symtable_() {
  module_ =
      new llvm::Module(std::string("shepherd_module_") + moduleName, context_);
  fpm_ = new llvm::legacy::FunctionPassManager(module_);
  fpm_->add(llvm::createInstructionCombiningPass());
  fpm_->add(llvm::createReassociatePass());
  fpm_->add(llvm::createGVNPass());
  fpm_->add(llvm::createCFGSimplificationPass());
  fpm_->add(llvm::createPromoteMemoryToRegisterPass());
  fpm_->doInitialization();
}

IrContext::~IrContext() {
  delete module_;
  module_ = nullptr;
  delete fpm_;
  fpm_ = nullptr;
}

const std::string &IrContext::moduleName() const { return moduleName_; }

llvm::LLVMContext &IrContext::context() { return context_; }

const llvm::LLVMContext &IrContext::context() const { return context_; }

llvm::IRBuilder<> &IrContext::builder() { return builder_; }

const llvm::IRBuilder<> &IrContext::builder() const { return builder_; }

llvm::Module *&IrContext::module() { return module_; }

const llvm::Module *IrContext::module() const { return module_; }

llvm::legacy::FunctionPassManager *&IrContext::functionPassManager() {
  return fpm_;
}

const llvm::legacy::FunctionPassManager *
IrContext::functionPassManager() const {
  return fpm_;
}

LinkedHashMap<std::string, llvm::Value *> &IrContext::symtable() {
  return symtable_;
}

const LinkedHashMap<std::string, llvm::Value *> &IrContext::symtable() const {
  return symtable_;
}

static Ir *createIrByAst(IrContext *context, Ast *node) {
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
  case AstType::ExpressionStatement:
    return new IrExpressionStatement(context, DC(AstExpressionStatement, node));
  case AstType::IfStatement:
    return new IrIfStatement(context, DC(AstIfStatement, node));
  case AstType::WhileStatement:
    return new IrWhileStatement(context, DC(AstWhileStatement, node));
  case AstType::ForStatement:
    return new IrForStatement(context, DC(AstForStatement, node));
  case AstType::CompoundStatement:
    return new IrCompoundStatement(context, DC(AstCompoundStatement, node));
  case AstType::ReturnStatement:
    return new IrReturnStatement(context, DC(AstReturnStatement, node));
  case AstType::VariableDefinition:
    return new IrVariableDefinition(context, DC(AstVariableDefinition, node));
  case AstType::FunctionDefinition:
    return new IrFunctionDefinition(context, DC(AstFunctionDefinition, node));
  case AstType::FunctionSignatureDefinition:
    return new IrFunctionSignatureDefinition(
        context, DC(AstFunctionSignatureDefinition, node));
  default:
    EX_ASSERT(false, "invalid ast node: {}, source:{}", node->toString(),
              dumpSource(context->moduleName(), node->position()));
  }
  return nullptr;
}

/* interface */
Ir::Ir(IrContext *context, const std::string &name)
    : context_(context), name_(name) {
  EX_ASSERT(context_, "context_ is null");
}

std::string Ir::name() const { return name_; }

static std::string toIrNameImpl(const std::string &name,
                                const std::string prefix) {
  EX_ASSERT(name.length() > 0, "name {} length {} <= 0", name, name.length());
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
  EX_ASSERT(name.length() > prefix.length(),
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

IrExpression::IrExpression(IrContext *context, const std::string &name)
    : Ir(context, name) {}

IrConstant::IrConstant(IrContext *context, const std::string &name)
    : IrExpression(context, name) {}

IrStatement::IrStatement(IrContext *context, const std::string &name)
    : Ir(context, name) {}

IrDefinition::IrDefinition(IrContext *context, const std::string &name)
    : IrStatement(context, name) {}

/* list */
IrExpressionList::IrExpressionList(IrContext *context)
    : detail::IrList<IrExpression>(context,
                                   nameGenerator.generate("ExprList")) {}

IrType IrExpressionList::type() const { return IrType::ExpressionList; }

std::string IrExpressionList::stringify() const { return "IrExpressionList"; }

IrStatementList::IrStatementList(IrContext *context)
    : detail::IrList<IrStatement>(context, nameGenerator.generate("StmtList")) {
}

IrType IrStatementList::type() const { return IrType::StatementList; }

std::string IrStatementList::stringify() const { return "IrStatementList"; }

IrDefinitionList::IrDefinitionList(IrContext *context)
    : detail::IrList<IrDefinition>(context,
                                   nameGenerator.generate("DeclList")) {}

IrType IrDefinitionList::type() const { return IrType::DefinitionList; }

std::string IrDefinitionList::stringify() const { return "IrDefinitionList"; }

/* translate unit */
IrTranslateUnit::IrTranslateUnit(IrContext *context, AstTranslateUnit *node)
    : detail::IrList<IrDefinition>(context, nameGenerator.generate("TUnit")),
      node_(node) {
  for (int i = 0; i < node_->size(); i++) {
    AstDefinition *ast = node_->get(i);
    switch (ast->type()) {
    case AstType::VariableDefinition: {
      IrVariableDefinition *vd =
          new IrVariableDefinition(context, DC(AstVariableDefinition, ast));
      add(vd);
    } break;
    case AstType::FunctionDefinition: {
      IrFunctionDefinition *fd =
          new IrFunctionDefinition(context, DC(AstFunctionDefinition, ast));
      add(fd);
    } break;
    default:
      EX_ASSERT(false, "invalid ast:{}", ast->toString());
    }
  }
}

IrType IrTranslateUnit::type() const { return IrType::TranslateUnit; }

std::string IrTranslateUnit::stringify() const { return "IrTranslateUnit"; }

/* identifier constant */
IrIdentifierConstant::IrIdentifierConstant(IrContext *context,
                                           AstIdentifierConstant *node)
    : IrConstant(context, nameGenerator.generate("Id")), node_(node),
      ssaVersion_(0) {
  EX_ASSERT(node_, "node_ is null");
}

IrType IrIdentifierConstant::type() const { return IrType::IdentifierConstant; }

llvm::Value *IrIdentifierConstant::codeGen() {
  EX_ASSERT(node_, "node_ is null");
  return context_->symtable()[Ir::toIrName(node_->value())];
}

std::string IrIdentifierConstant::toString() const {
  return fmt::format("[@IrIdentifierConstant node_:{}]", node_->toString());
}

/* i8 constant */
IrInt8Constant::IrInt8Constant(IrContext *context, AstInt8Constant *node)
    : IrConstant(context, nameGenerator.generate("IrInt8Constant")),
      node_(node) {}

IrType IrInt8Constant::type() const { return IrType::Int8Constant; }

llvm::Value *IrInt8Constant::codeGen() {
  return llvm::ConstantInt::get(context_->context(),
                                llvm::APInt(8, (uint64_t)node_->value(), true));
}

std::string IrInt8Constant::toString() const {
  return fmt::format("[@IrInt8Constant node_:{}]", node_->toString());
}

/* u8 constant */
IrUInt8Constant::IrUInt8Constant(IrContext *context, AstUInt8Constant *node)
    : IrConstant(context, nameGenerator.generate("IrUInt8Constant")),
      node_(node) {}

IrType IrUInt8Constant::type() const { return IrType::UInt8Constant; }

llvm::Value *IrUInt8Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->context(), llvm::APInt(8, (uint64_t)node_->value(), false));
}

std::string IrUInt8Constant::toString() const {
  return fmt::format("[@IrUInt8Constant node_:{}]", node_->toString());
}

/* i16 constant */
IrInt16Constant::IrInt16Constant(IrContext *context, AstInt16Constant *node)
    : IrConstant(context, nameGenerator.generate("IrInt16Constant")),
      node_(node) {}

IrType IrInt16Constant::type() const { return IrType::Int16Constant; }

llvm::Value *IrInt16Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->context(), llvm::APInt(16, (uint64_t)node_->value(), true));
}

std::string IrInt16Constant::toString() const {
  return fmt::format("[@IrInt16Constant node_:{}]", node_->toString());
}

/* u16 constant */
IrUInt16Constant::IrUInt16Constant(IrContext *context, AstUInt16Constant *node)
    : IrConstant(context, nameGenerator.generate("IrUInt16Constant")),
      node_(node) {}

IrType IrUInt16Constant::type() const { return IrType::UInt16Constant; }

llvm::Value *IrUInt16Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->context(), llvm::APInt(16, (uint64_t)node_->value(), false));
}

std::string IrUInt16Constant::toString() const {
  return fmt::format("[@IrUInt16Constant node_:{}]", node_->toString());
}

/* i32 constant */
IrInt32Constant::IrInt32Constant(IrContext *context, AstInt32Constant *node)
    : IrConstant(context, nameGenerator.generate("IrInt32Constant")),
      node_(node) {}

IrType IrInt32Constant::type() const { return IrType::Int32Constant; }

llvm::Value *IrInt32Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->context(), llvm::APInt(32, (uint64_t)node_->value(), true));
}

std::string IrInt32Constant::toString() const {
  return fmt::format("[@IrInt32Constant node_:{}]", node_->toString());
}

/* u32 constant */
IrUInt32Constant::IrUInt32Constant(IrContext *context, AstUInt32Constant *node)
    : IrConstant(context, nameGenerator.generate("IrUInt32Constant")),
      node_(node) {}

IrType IrUInt32Constant::type() const { return IrType::UInt32Constant; }

llvm::Value *IrUInt32Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->context(), llvm::APInt(32, (uint64_t)node_->value(), false));
}

std::string IrUInt32Constant::toString() const {
  return fmt::format("[@IrUInt32Constant node_:{}]", node_->toString());
}

/* i64 constant */
IrInt64Constant::IrInt64Constant(IrContext *context, AstInt64Constant *node)
    : IrConstant(context, nameGenerator.generate("IrInt64Constant")),
      node_(node) {}

IrType IrInt64Constant::type() const { return IrType::Int64Constant; }

llvm::Value *IrInt64Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->context(), llvm::APInt(64, (uint64_t)node_->value(), true));
}

std::string IrInt64Constant::toString() const {
  return fmt::format("[@IrInt64Constant node_:{}]", node_->toString());
}

/* u64 constant */
IrUInt64Constant::IrUInt64Constant(IrContext *context, AstUInt64Constant *node)
    : IrConstant(context, nameGenerator.generate("IrUInt64Constant")),
      node_(node) {}

IrType IrUInt64Constant::type() const { return IrType::UInt64Constant; }

llvm::Value *IrUInt64Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->context(), llvm::APInt(64, (uint64_t)node_->value(), false));
}

std::string IrUInt64Constant::toString() const {
  return fmt::format("[@IrUInt64Constant node_:{}]", node_->toString());
}

/* f32 constant */
IrFloat32Constant::IrFloat32Constant(IrContext *context,
                                     AstFloat32Constant *node)
    : IrConstant(context, nameGenerator.generate("IrFloat32Constant")),
      node_(node) {}

IrType IrFloat32Constant::type() const { return IrType::Float32Constant; }

llvm::Value *IrFloat32Constant::codeGen() {
  return llvm::ConstantFP::get(context_->context(),
                               llvm::APFloat((float)node_->value()));
}

std::string IrFloat32Constant::toString() const {
  return fmt::format("[@IrFloat32Constant node_:{}]", node_->toString());
}

/* f64 constant */
IrFloat64Constant::IrFloat64Constant(IrContext *context,
                                     AstFloat64Constant *node)
    : IrConstant(context, nameGenerator.generate("IrFloat64Constant")),
      node_(node) {}

IrType IrFloat64Constant::type() const { return IrType::Float64Constant; }

llvm::Value *IrFloat64Constant::codeGen() {
  return llvm::ConstantFP::get(context_->context(),
                               llvm::APFloat((double)node_->value()));
}

std::string IrFloat64Constant::toString() const {
  return fmt::format("[@IrFloat64Constant node_:{}]", node_->toString());
}

/* string constant */
IrStringConstant::IrStringConstant(IrContext *context, AstStringConstant *node)
    : IrConstant(context, nameGenerator.generate("IrStringConstant")),
      node_(node) {}

IrType IrStringConstant::type() const { return IrType::StringConstant; }

llvm::Value *IrStringConstant::codeGen() { return nullptr; }

std::string IrStringConstant::toString() const {
  return fmt::format("[@IrStringConstant node_:{}]", node_->toString());
}

/* boolean constant */
IrBooleanConstant::IrBooleanConstant(IrContext *context,
                                     AstBooleanConstant *node)
    : IrConstant(context, nameGenerator.generate("IrBooleanConstant")),
      node_(node) {}

IrType IrBooleanConstant::type() const { return IrType::BooleanConstant; }

llvm::Value *IrBooleanConstant::codeGen() {
  return llvm::ConstantInt::get(
      context_->context(),
      llvm::APInt(1, node_->value() ? (uint64_t)1U : (uint64_t)0U, false));
}

std::string IrBooleanConstant::toString() const {
  return fmt::format("[@IrBooleanConstant node_:{}]", node_->toString());
}

/* call expression */
IrCallExpression::IrCallExpression(IrContext *context, AstCallExpression *node)
    : IrExpression(context, nameGenerator.generate("IrCallExpression")),
      node_(node) {}

IrType IrCallExpression::type() const { return IrType::CallExpression; }

llvm::Value *IrCallExpression::codeGen() { return nullptr; }

std::string IrCallExpression::toString() const {
  return fmt::format("[@IrCallExpression node_:{}]", node_->toString());
}

/* unary expression */
IrUnaryExpression::IrUnaryExpression(IrContext *context,
                                     AstUnaryExpression *node)
    : IrExpression(context, nameGenerator.generate("IrUnaryExpression")),
      node_(node) {}

IrType IrUnaryExpression::type() const { return IrType::UnaryExpression; }

llvm::Value *IrUnaryExpression::codeGen() { return nullptr; }

std::string IrUnaryExpression::toString() const {
  return fmt::format("[@IrUnaryExpression node_:{}]", node_->toString());
}

/* binary expression */
IrBinaryExpression::IrBinaryExpression(IrContext *context,
                                       AstBinaryExpression *node)
    : IrExpression(context, nameGenerator.generate("IrBinaryExpression")),
      node_(node),
      left_(DC(IrExpression, createIrByAst(context, node->left()))),
      right_(DC(IrExpression, createIrByAst(context, node->right()))) {
  EX_ASSERT(left_, "left_ is null");
  EX_ASSERT(right_, "right_ is null");
}

IrBinaryExpression::~IrBinaryExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

IrType IrBinaryExpression::type() const { return IrType::BinaryExpression; }

llvm::Value *IrBinaryExpression::codeGen() {
  llvm::Value *l = left_->codeGen();
  llvm::Value *r = right_->codeGen();
  EX_ASSERT(l, "l is null");
  EX_ASSERT(r, "r is null");
  switch (node_->token()) {
  case T_ADD: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFAdd(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context_->builder().CreateFAdd(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context_->builder().CreateAdd(l, r);
      } break;
      default:
        EX_ASSERT(false, "r->getType->getTypeID {} invalid",
                  r->getType()->getTypeID());
      }
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_SUB: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFSub(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context_->builder().CreateFSub(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context_->builder().CreateSub(l, r);
      } break;
      default:
        EX_ASSERT(false, "r->getType->getTypeID {} invalid",
                  r->getType()->getTypeID());
      }
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_MUL: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFMul(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context_->builder().CreateFMul(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context_->builder().CreateMul(l, r);
      } break;
      default:
        EX_ASSERT(false, "r->getType->getTypeID {} invalid",
                  r->getType()->getTypeID());
      }
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_DIV: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFDiv(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context_->builder().CreateFDiv(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context_->builder().CreateSDiv(l, r);
      } break;
      default:
        EX_ASSERT(false, "r->getType->getTypeID {} invalid",
                  r->getType()->getTypeID());
      }
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_MOD: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFRem(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context_->builder().CreateFRem(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context_->builder().CreateSRem(l, r);
      } break;
      default:
        EX_ASSERT(false, "r->getType->getTypeID {} invalid",
                  r->getType()->getTypeID());
      }
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_BIT_LSHIFT: {
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    return context_->builder().CreateShl(l, r);
  } break;
  case T_BIT_RSHIFT: {
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    return context_->builder().CreateLShr(l, r);
  } break;
  case T_BIT_ARSHIFT: {
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    return context_->builder().CreateAShr(l, r);
  } break;
  case T_EQ: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFCmpOEQ(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context_->builder().CreateICmpEQ(l, r);
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_NEQ: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFCmpONE(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context_->builder().CreateICmpNE(l, r);
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_LE: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFCmpOLE(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context_->builder().CreateICmpSLE(l, r);
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_LT: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFCmpOLT(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context_->builder().CreateICmpSLT(l, r);
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_GE: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFCmpOGE(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context_->builder().CreateICmpSGE(l, r);
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_GT: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID: {
      return context_->builder().CreateFCmpOGT(l, r);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      return context_->builder().CreateICmpSGT(l, r);
    } break;
    default:
      EX_ASSERT(false, "l->getType->getTypeID {} invalid",
                l->getType()->getTypeID());
    }
  } break;
  case T_BIT_AND: {
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    return context_->builder().CreateAnd(l, r, "andtmp");
  } break;
  case T_BIT_OR: {
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    return context_->builder().CreateOr(l, r, "ortmp");
  } break;
  case T_BIT_XOR: {
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    return context_->builder().CreateXor(l, r, "xortmp");
  } break;
  case T_LOGIC_AND: {
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    llvm::ConstantInt *lc = llvm::dyn_cast<llvm::ConstantInt>(l);
    llvm::ConstantInt *rc = llvm::dyn_cast<llvm::ConstantInt>(r);
    EX_ASSERT(lc->getBitWidth() == 1, "lc->getBitWidth {} != 1",
              lc->getBitWidth());
    EX_ASSERT(rc->getBitWidth() == 1, "rc->getBitWidth {} != 1",
              rc->getBitWidth());
    return context_->builder().CreateAnd(l, r, "logic_andtmp");
  } break;
  case T_LOGIC_OR: {
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    llvm::ConstantInt *lc = llvm::dyn_cast<llvm::ConstantInt>(l);
    llvm::ConstantInt *rc = llvm::dyn_cast<llvm::ConstantInt>(r);
    EX_ASSERT(lc->getBitWidth() == 1, "lc->getBitWidth {} != 1",
              lc->getBitWidth());
    EX_ASSERT(rc->getBitWidth() == 1, "rc->getBitWidth {} != 1",
              rc->getBitWidth());
    return context_->builder().CreateOr(l, r, "logic_ortmp");
  } break;
  default:
    EX_ASSERT(false, "token {} invalid", node_->token());
  }
  return nullptr;
}

std::string IrBinaryExpression::toString() const {
  return fmt::format("[@IrBinaryExpression node_:{}]", node_->toString());
}

/* conditional expression */
IrConditionalExpression::IrConditionalExpression(IrContext *context,
                                                 AstConditionalExpression *node)
    : IrExpression(context, nameGenerator.generate("IrConditionalExpression")),
      node_(node) {}

IrType IrConditionalExpression::type() const {
  return IrType::ConditionalExpression;
}

llvm::Value *IrConditionalExpression::codeGen() { return nullptr; }

std::string IrConditionalExpression::toString() const {
  return fmt::format("[@IrConditionalExpression node_:{}]", node_->toString());
}

/* assignment expression */
IrAssignmentExpression::IrAssignmentExpression(IrContext *context,
                                               AstAssignmentExpression *node)
    : IrExpression(context, nameGenerator.generate("IrAssignmentExpression")),
      node_(node),
      variable_(DC(IrExpression, createIrByAst(context, node->variable()))),
      value_(DC(IrExpression, createIrByAst(context, node->value()))) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(variable_, "variable_ is null");
  EX_ASSERT(value_, "value_ is null");
}

IrAssignmentExpression::~IrAssignmentExpression() {
  delete variable_;
  variable_ = nullptr;
  delete value_;
  value_ = nullptr;
}

IrType IrAssignmentExpression::type() const {
  return IrType::AssignmentExpression;
}

llvm::Value *IrAssignmentExpression::codeGen() { return nullptr; }

std::string IrAssignmentExpression::toString() const {
  return fmt::format("[@IrAssignmentExpression node_:{}]", node_->toString());
}

/* sequel expression */
IrSequelExpression::IrSequelExpression(IrContext *context,
                                       AstSequelExpression *node)
    : IrExpression(context, nameGenerator.generate("IrSequelExpression")),
      node_(node), expressionList_(new IrExpressionList(context)) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->expressionList(), "node_->expressionList is null");
  for (int i = 0; i < node->expressionList()->size(); i++) {
    AstExpression *ast = node->expressionList()->get(i);
    EX_ASSERT(ast, "the {} ast is null", i);
    expressionList_->add(DC(IrExpression, createIrByAst(context_, ast)));
  }
}

IrSequelExpression::~IrSequelExpression() {
  for (int i = 0; i < expressionList_->size(); i++) {
    IrExpression *ir = expressionList_->get(i);
    delete ir;
  }
  delete expressionList_;
  expressionList_ = nullptr;
}

IrType IrSequelExpression::type() const { return IrType::SequelExpression; }

IrExpressionList *IrSequelExpression::expressionList() {
  return expressionList_;
}

llvm::Value *IrSequelExpression::codeGen() {
  EX_ASSERT(false, "never reach here");
  return nullptr;
}

std::string IrSequelExpression::toString() const {
  return fmt::format("[@IrSequelExpression node_:{}]", node_->toString());
}

IrVoidExpression::IrVoidExpression(IrContext *context, AstVoidExpression *node)
    : IrExpression(context, nameGenerator.generate("IrVoidExpression")),
      node_(node) {
  EX_ASSERT(node_, "node_ is null");
}
std::string IrVoidExpression::toString() const {
  return fmt::format("[@IrVoidExpression node_:{}]", node_->toString());
}

IrType IrVoidExpression::type() const { return IrType::VoidExpression; }

llvm::Value *IrVoidExpression::codeGen() { return nullptr; }

/* expression statement */
IrExpressionStatement::IrExpressionStatement(IrContext *context,
                                             AstExpressionStatement *node)
    : IrStatement(context, nameGenerator.generate("IrExpressionStatement")),
      node_(node), expression_(DC(IrExpression,
                                  createIrByAst(context, node->expression()))) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(expression_, "expression_ is null");
}

IrExpressionStatement::~IrExpressionStatement() {
  delete expression_;
  expression_ = nullptr;
}

IrType IrExpressionStatement::type() const {
  return IrType::ExpressionStatement;
}

llvm::Value *IrExpressionStatement::codeGen() {
  EX_ASSERT(expression_, "expression_ is null");
  return expression_->codeGen();
}

std::string IrExpressionStatement::toString() const {
  return fmt::format("[@IrExpressionStatement node_:{}]", node_->toString());
}

/* compound statement */
IrCompoundStatement::IrCompoundStatement(IrContext *context,
                                         AstCompoundStatement *node)
    : IrStatement(context, nameGenerator.generate("IrCompoundStatement")),
      node_(node), statementList_(new IrStatementList(context)) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->statementList(), "node_->statementList is null");
  for (int i = 0; i < node_->statementList()->size(); i++) {
    Ast *ast = node_->statementList()->get(i);
    EX_ASSERT(ast, "the {} ast is null", i);
    if (ast->type() == (+AstType::EmptyStatement)) {
      continue;
    }
    IrStatement *ir = DC(IrStatement, createIrByAst(context_, ast));
    EX_ASSERT(ir, "ir is null, ast: {}", ast->toString());
    statementList_->add(ir);
  }
}

IrCompoundStatement::~IrCompoundStatement() {
  for (int i = 0; i < statementList_->size(); i++) {
    IrStatement *ir = statementList_->get(i);
    delete ir;
  }
  delete statementList_;
  statementList_ = nullptr;
}

IrType IrCompoundStatement::type() const { return IrType::CompoundStatement; }

llvm::Value *IrCompoundStatement::codeGen() {
  if (statementList_->size() <= 0) {
    return nullptr;
  }
  for (int i = 0; i < statementList_->size(); i++) {
    IrStatement *ir = statementList_->get(0);
    EX_ASSERT(ir, "ir is null");
    switch (ir->type()) {
    case IrType::ExpressionStatement: {
    } break;
    case IrType::CompoundStatement: {
    } break;
    case IrType::IfStatement: {
    } break;
    case IrType::WhileStatement: {
    } break;
    case IrType::ForStatement: {
    } break;
    case IrType::ReturnStatement: {
    } break;
    default:
      EX_ASSERT(false, "invalid ir: {}", ir->toString());
    }
  }
  IrStatement *ir = statementList_->get(statementList_->size() - 1);
  return ir->codeGen();
}

std::string IrCompoundStatement::toString() const {
  return fmt::format("[@IrCompoundStatement node_:{}]", node_->toString());
}

/* if statement */
IrIfStatement::IrIfStatement(IrContext *context, AstIfStatement *node)
    : IrStatement(context, nameGenerator.generate("IrIfStatement")),
      node_(node),
      condition_(DC(IrExpression, createIrByAst(context, node->condition()))),
      thens_(DC(IrStatement, createIrByAst(context, node->thens()))),
      elses_(DC(IrStatement, createIrByAst(context, node->elses()))) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->condition(), "node_->condition is null");
  EX_ASSERT(node_->thens(), "node_->thens is null");
  EX_ASSERT(node_->elses(), "node_->elses is null");
}

IrIfStatement::~IrIfStatement() {
  delete condition_;
  condition_ = nullptr;
  delete thens_;
  thens_ = nullptr;
  delete elses_;
  elses_ = nullptr;
}

IrType IrIfStatement::type() const { return IrType::IfStatement; }

llvm::Value *IrIfStatement::codeGen() {
  llvm::Value *condV = condition_->codeGen();
  EX_ASSERT(condV, "condV is null");
  condV = context_->builder().CreateICmpNE(
      condV,
      llvm::ConstantInt::get(context_->context(),
                             llvm::APInt(1, (uint64_t)0, false)),
      irNameGenerator.generate("ifcond"));
  llvm::Function *f = context_->builder().GetInsertBlock()->getParent();
  llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(
      context_->context(), irNameGenerator.generate("then"), f);
  llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(
      context_->context(), irNameGenerator.generate("else"));
  llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(
      context_->context(), irNameGenerator.generate("ifcont"));
  context_->builder().CreateCondBr(condV, thenBlock, elseBlock);
  context_->builder().SetInsertPoint(thenBlock);
  llvm::Value *thenV = thens_->codeGen();
  EX_ASSERT(thenV, "thenV is null");
  context_->builder().CreateBr(mergeBlock);
  thenBlock = context_->builder().GetInsertBlock();
  f->getBasicBlockList().push_back(elseBlock);
  context_->builder().SetInsertPoint(elseBlock);
  llvm::Value *elseV = elses_->codeGen();
  EX_ASSERT(elseV, "elseV is null");
  context_->builder().CreateBr(mergeBlock);
  elseBlock = context_->builder().GetInsertBlock();
  f->getBasicBlockList().push_back(mergeBlock);
  context_->builder().SetInsertPoint(mergeBlock);
  llvm::PHINode *pn = context_->builder().CreatePHI(
      llvm::Type::getDoubleTy(context_->context()), 2, "iftmp");
  pn->addIncoming(thenV, thenBlock);
  pn->addIncoming(elseV, elseBlock);
  return pn;
}

std::string IrIfStatement::toString() const {
  return fmt::format("[@IrIfStatement node_:{}]", node_->toString());
}

/* while statement */
IrWhileStatement::IrWhileStatement(IrContext *context, AstWhileStatement *node)
    : IrStatement(context, nameGenerator.generate("IrWhileStatement")),
      node_(node) {
  EX_ASSERT(node_, "node_ is null");
}

IrType IrWhileStatement::type() const { return IrType::WhileStatement; }

llvm::Value *IrWhileStatement::codeGen() { return nullptr; }

std::string IrWhileStatement::toString() const {
  return fmt::format("[@IrWhileStatement node_:{}]", node_->toString());
}

/* for statement */
IrForStatement::IrForStatement(IrContext *context, AstForStatement *node)
    : IrStatement(context, nameGenerator.generate("IrForStatement")),
      node_(node),
      start_(DC(IrStatement, createIrByAst(context, node->start()))),
      step_(DC(IrStatement, createIrByAst(context, node->step()))),
      end_(DC(IrStatement, createIrByAst(context, node->end()))),
      statement_(DC(IrStatement, createIrByAst(context, node->statement()))) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->start(), "node_->start is null");
  EX_ASSERT(node_->step(), "node_->step is null");
  EX_ASSERT(node_->end(), "node_->end is null");
  EX_ASSERT(node_->statement(), "node_->statement is null");
}

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

llvm::Value *IrForStatement::codeGen() {
  llvm::Value *startV = start_->codeGen();
  EX_ASSERT(startV, "startV is null");
  llvm::Function *f = context_->builder().GetInsertBlock()->getParent();
  llvm::BasicBlock *preheaderBB = context_->builder().GetInsertBlock();
  llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(
      context_->context(), irNameGenerator.generate("loop"), f);
  context_->builder().CreateBr(loopBB);
  context_->builder().SetInsertPoint(loopBB);

  llvm::PHINode *variable = context_->builder().CreatePHI(
      llvm::Type::getDoubleTy(context_->context()), 2,
      irNameGenerator.generate("loop.phi"));
  variable->addIncoming(startV, preheaderBB);
  llvm::Value *bodyV = statement_->codeGen();
  EX_ASSERT(bodyV, "bodyV is null");
  llvm::Value *stepV = nullptr;
  if (step_) {
    stepV = step_->codeGen();
    EX_ASSERT(stepV, "stepV is null");
  } else {
    stepV = llvm::ConstantInt::get(context_->context(),
                                   llvm::APInt(1, (uint64_t)1, false));
  }
  llvm::Value *nextV = context_->builder().CreateAdd(
      variable, stepV, irNameGenerator.generate("nextvar"));
  llvm::Value *endCond = end_->codeGen();
  EX_ASSERT(endCond, "endCond is null");
  endCond = context_->builder().CreateICmpNE(
      endCond, llvm::ConstantInt::get(context_->context(),
                                      llvm::APInt(1, (uint64_t)0, false)));
  llvm::BasicBlock *loopEndBB = context_->builder().GetInsertBlock();
  llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(
      context_->context(), irNameGenerator.generate("afterloop"), f);
  context_->builder().CreateCondBr(endCond, loopBB, afterBB);
  context_->builder().SetInsertPoint(afterBB);
  variable->addIncoming(nextV, loopEndBB);
  return llvm::Constant::getNullValue(
      llvm::Type::getDoubleTy(context_->context()));
}

std::string IrForStatement::toString() const {
  return fmt::format("[@IrForStatement node_:{}]", node_->toString());
}

/* return statement */
IrReturnStatement::IrReturnStatement(IrContext *context,
                                     AstReturnStatement *node)
    : IrStatement(context, nameGenerator.generate("IrReturnStatement")),
      node_(node), expression_(DC(IrExpression,
                                  createIrByAst(context, node->expression()))) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->expression(), "node_->expression is null");
}

IrReturnStatement::~IrReturnStatement() {
  delete expression_;
  expression_ = nullptr;
}

std::string IrReturnStatement::toString() const {
  return fmt::format("[@IrReturnStatement node_:{}]", node_->toString());
}

IrType IrReturnStatement::type() const { return IrType::ReturnStatement; }

llvm::Value *IrReturnStatement::codeGen() {
  EX_ASSERT(expression_, "expression_ is null");
  switch (expression_->type()) {
  case IrType::VoidExpression:
    return context_->builder().CreateRetVoid();
  case IrType::SequelExpression: {
    IrExpressionList *expressionList =
        DC(IrSequelExpression, expression_)->expressionList();
    EX_ASSERT(expressionList->size() > 0, "expressionList->size {} > 0",
              expressionList->size());
    llvm::Value *exprV =
        expressionList->get(expressionList->size() - 1)->codeGen();
    return context_->builder().CreateRet(exprV);
  } break;
  default:
    EX_ASSERT(false, "invalid expression_:{}", expression_->toString());
  }
  return nullptr;
}

/* variable definition */
IrVariableDefinition::IrVariableDefinition(IrContext *context,
                                           AstVariableDefinition *node)
    : IrDefinition(context, nameGenerator.generate("IrVariableDefinition")),
      node_(node) {
  EX_ASSERT(node_, "node_ is null");
}

IrType IrVariableDefinition::type() const { return IrType::VariableDefinition; }

llvm::Value *IrVariableDefinition::codeGen() { return nullptr; }

std::string IrVariableDefinition::toString() const {
  return fmt::format("[@IrVariableDefinition node_:{}]", node_->toString());
}

/* function definition */
IrFunctionDefinition::IrFunctionDefinition(IrContext *context,
                                           AstFunctionDefinition *node)
    : IrDefinition(context, nameGenerator.generate("IrFunctionDefinition")),
      node_(node), signature_(DC(IrFunctionSignatureDefinition,
                                 createIrByAst(context, node->signature()))),
      statement_(DC(IrStatement, createIrByAst(context, node->statement()))) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->signature(), "node_->signature is null");
  EX_ASSERT(node_->statement(), "node_->statement is null");
}

IrType IrFunctionDefinition::type() const { return IrType::FunctionDefinition; }

llvm::Value *IrFunctionDefinition::codeGen() {
  llvm::Function *f = context_->module()->getFunction(
      Ir::toIrName(node_->signature()->identifier()));
  if (!f) {
    f = llvm::dyn_cast<llvm::Function>(signature_->codeGen());
  }
  if (!f) {
    return nullptr;
  }
  EX_ASSERT(f->empty(), "Function {} cannot be redefined!",
            node_->signature()->identifier());
  llvm::BasicBlock *bb =
      llvm::BasicBlock::Create(context_->context(), "shp.ir.func.entry", f);
  context_->builder().SetInsertPoint(bb);
  context_->symtable().clear();
  for (auto &a : f->args()) {
    context_->symtable().insert(a.getName(), &a);
  }
  llvm::Value *ret = statement_->codeGen();
  if (ret) {
    context_->builder().CreateRet(ret);
    llvm::verifyFunction(*f);
    return llvm::dyn_cast<llvm::Value>(f);
  }
  f->eraseFromParent();
  return nullptr;
}

std::string IrFunctionDefinition::toString() const {
  return fmt::format("[@IrFunctionDefinition node_:{}]", node_->toString());
}

/* function signature definition */
IrFunctionSignatureDefinition::IrFunctionSignatureDefinition(
    IrContext *context, AstFunctionSignatureDefinition *node)
    : IrDefinition(context,
                   nameGenerator.generate("IrFunctionSignatureDefinition")),
      node_(node) {
  EX_ASSERT(node_, "node_ is null");
}

std::string IrFunctionSignatureDefinition::toString() const {
  return fmt::format("[@IrFunctionSignatureDefinition node_:{}]",
                     node_->toString());
}

IrType IrFunctionSignatureDefinition::type() const {
  return IrType::FunctionSignatureDefinition;
}

llvm::Value *IrFunctionSignatureDefinition::codeGen() {
  AstDefinitionList *args = node_->argumentList();
  std::vector<llvm::Type *> doubleArgs(
      args->size(), llvm::Type::getDoubleTy(context_->context()));
  llvm::FunctionType *ft = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(context_->context()), doubleArgs, false);
  llvm::Function *f = llvm::Function::Create(
      ft, llvm::Function::ExternalLinkage, Ir::toIrName(node_->identifier()),
      context_->module());
  // set function arg names
  int i = 0;
  for (auto &a : f->args()) {
    EX_ASSERT(args, "args is null");
    EX_ASSERT(args->get(i), "args->get({}) is null", i);
    AstFunctionArgumentDefinition *ast =
        DC(AstFunctionArgumentDefinition, args->get(i++));
    a.setName(Ir::toIrName(ast->identifier()));
  }
  return llvm::dyn_cast<llvm::Value>(f);
}
