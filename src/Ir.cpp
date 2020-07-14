// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "Ast.h"
#include "Dump.h"
#include "Exception.h"
#include "IrFactory.h"
#include "IrUtil.h"
#include "Log.h"
#include "NameGenerator.h"
#include "Parser.tab.hpp"
#include "Symbol.h"
#include "TokenName.h"
#include "container/LinkedHashMap.hpp"
#include "fmt/format.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
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

#define DC(x, y) dynamic_cast<x *>(y)

/* interface */
Ir::Ir(IrContext *context, const std::string &name)
    : context_(context), name_(name) {
  EX_ASSERT(context_, "context_ must not null");
}

std::string Ir::name() const { return name_; }

void Ir::buildSymbol() {}

void Ir::checkSymbol() const {}

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
                                   IrUtil::namegen("expression.list")) {}

IrType IrExpressionList::type() const { return IrType::ExpressionList; }

std::string IrExpressionList::stringify() const { return "IrExpressionList"; }

IrStatementList::IrStatementList(IrContext *context)
    : detail::IrList<IrStatement>(context, IrUtil::namegen("statement.list")) {}

IrType IrStatementList::type() const { return IrType::StatementList; }

std::string IrStatementList::stringify() const { return "IrStatementList"; }

IrDefinitionList::IrDefinitionList(IrContext *context)
    : detail::IrList<IrDefinition>(context,
                                   IrUtil::namegen("definition.list")) {}

IrType IrDefinitionList::type() const { return IrType::DefinitionList; }

std::string IrDefinitionList::stringify() const { return "IrDefinitionList"; }

/* translate unit */
IrTranslateUnit::IrTranslateUnit(IrContext *context, AstTranslateUnit *node)
    : detail::IrList<IrDefinition>(context, IrUtil::namegen("translate.unit")),
      node_(node) {
  EX_ASSERT(node_, "node_ is null");
  for (int i = 0; i < node_->size(); i++) {
    AstDefinition *ast = node_->get(i);
    add(IrFactory::unit(context_, ast));
  }
}

void IrTranslateUnit::buildSymbol() {
  for (int i = 0; i < size(); i++) {
    IrDefinition *ir = get(i);
    EX_ASSERT(ir, "ir is null");
    ir->buildSymbol();
  }
}

void IrTranslateUnit::checkSymbol() const {
  for (int i = 0; i < size(); i++) {
    IrDefinition *ir = get(i);
    EX_ASSERT(ir, "ir is null");
    ir->checkSymbol();
  }
}

IrType IrTranslateUnit::type() const { return IrType::TranslateUnit; }

std::string IrTranslateUnit::stringify() const { return "IrTranslateUnit"; }

/* identifier constant */
IrIdentifierConstant::IrIdentifierConstant(IrContext *context,
                                           AstIdentifierConstant *node)
    : IrConstant(context, IrUtil::namegen("identifier.constant")), node_(node),
      ssaVersion_(0) {
  EX_ASSERT(node_, "node_ is null");
}

IrType IrIdentifierConstant::type() const { return IrType::IdentifierConstant; }

llvm::Value *IrIdentifierConstant::codeGen() {
  EX_ASSERT(node_, "node_ is null");
  ScopeNode *varNode = context_->symbolTable->current->resolve(node_->value());
  EX_ASSERT(varNode, "varNode must not null");
  EX_ASSERT(*varNode != ScopeNode::invalid(), "varNode {} must be valid",
            varNode->toString());
  EX_ASSERT(varNode->value, "varNode.value {} must not null",
            varNode->toString());
  return varNode->value;
}

std::string IrIdentifierConstant::toString() const {
  return fmt::format("[@IrIdentifierConstant node_:{}]", node_->toString());
}

/* i8 constant */
IrInt8Constant::IrInt8Constant(IrContext *context, AstInt8Constant *node)
    : IrConstant(context, IrUtil::namegen("int8.constant")), node_(node) {}

IrType IrInt8Constant::type() const { return IrType::Int8Constant; }

llvm::Value *IrInt8Constant::codeGen() {
  return llvm::ConstantInt::get(context_->llvmContext,
                                llvm::APInt(8, (uint64_t)node_->value(), true));
}

std::string IrInt8Constant::toString() const {
  return fmt::format("[@IrInt8Constant node_:{}]", node_->toString());
}

/* u8 constant */
IrUInt8Constant::IrUInt8Constant(IrContext *context, AstUInt8Constant *node)
    : IrConstant(context, IrUtil::namegen("uint8.constant")), node_(node) {}

IrType IrUInt8Constant::type() const { return IrType::UInt8Constant; }

llvm::Value *IrUInt8Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->llvmContext, llvm::APInt(8, (uint64_t)node_->value(), false));
}

std::string IrUInt8Constant::toString() const {
  return fmt::format("[@IrUInt8Constant node_:{}]", node_->toString());
}

/* i16 constant */
IrInt16Constant::IrInt16Constant(IrContext *context, AstInt16Constant *node)
    : IrConstant(context, IrUtil::namegen("int16.constant")), node_(node) {}

IrType IrInt16Constant::type() const { return IrType::Int16Constant; }

llvm::Value *IrInt16Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->llvmContext, llvm::APInt(16, (uint64_t)node_->value(), true));
}

std::string IrInt16Constant::toString() const {
  return fmt::format("[@IrInt16Constant node_:{}]", node_->toString());
}

/* u16 constant */
IrUInt16Constant::IrUInt16Constant(IrContext *context, AstUInt16Constant *node)
    : IrConstant(context, IrUtil::namegen("uint16.constant")), node_(node) {}

IrType IrUInt16Constant::type() const { return IrType::UInt16Constant; }

llvm::Value *IrUInt16Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->llvmContext, llvm::APInt(16, (uint64_t)node_->value(), false));
}

std::string IrUInt16Constant::toString() const {
  return fmt::format("[@IrUInt16Constant node_:{}]", node_->toString());
}

/* i32 constant */
IrInt32Constant::IrInt32Constant(IrContext *context, AstInt32Constant *node)
    : IrConstant(context, IrUtil::namegen("int32.constant")), node_(node) {}

IrType IrInt32Constant::type() const { return IrType::Int32Constant; }

llvm::Value *IrInt32Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->llvmContext, llvm::APInt(32, (uint64_t)node_->value(), true));
}

std::string IrInt32Constant::toString() const {
  return fmt::format("[@IrInt32Constant node_:{}]", node_->toString());
}

/* u32 constant */
IrUInt32Constant::IrUInt32Constant(IrContext *context, AstUInt32Constant *node)
    : IrConstant(context, IrUtil::namegen("uint32.constant")), node_(node) {}

IrType IrUInt32Constant::type() const { return IrType::UInt32Constant; }

llvm::Value *IrUInt32Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->llvmContext, llvm::APInt(32, (uint64_t)node_->value(), false));
}

std::string IrUInt32Constant::toString() const {
  return fmt::format("[@IrUInt32Constant node_:{}]", node_->toString());
}

/* i64 constant */
IrInt64Constant::IrInt64Constant(IrContext *context, AstInt64Constant *node)
    : IrConstant(context, IrUtil::namegen("int64.constant")), node_(node) {}

IrType IrInt64Constant::type() const { return IrType::Int64Constant; }

llvm::Value *IrInt64Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->llvmContext, llvm::APInt(64, (uint64_t)node_->value(), true));
}

std::string IrInt64Constant::toString() const {
  return fmt::format("[@IrInt64Constant node_:{}]", node_->toString());
}

/* u64 constant */
IrUInt64Constant::IrUInt64Constant(IrContext *context, AstUInt64Constant *node)
    : IrConstant(context, IrUtil::namegen("uint64.constant")), node_(node) {}

IrType IrUInt64Constant::type() const { return IrType::UInt64Constant; }

llvm::Value *IrUInt64Constant::codeGen() {
  return llvm::ConstantInt::get(
      context_->llvmContext, llvm::APInt(64, (uint64_t)node_->value(), false));
}

std::string IrUInt64Constant::toString() const {
  return fmt::format("[@IrUInt64Constant node_:{}]", node_->toString());
}

/* f32 constant */
IrFloat32Constant::IrFloat32Constant(IrContext *context,
                                     AstFloat32Constant *node)
    : IrConstant(context, IrUtil::namegen("float32.constant")), node_(node) {}

IrType IrFloat32Constant::type() const { return IrType::Float32Constant; }

llvm::Value *IrFloat32Constant::codeGen() {
  return llvm::ConstantFP::get(context_->llvmContext,
                               llvm::APFloat((float)node_->value()));
}

std::string IrFloat32Constant::toString() const {
  return fmt::format("[@IrFloat32Constant node_:{}]", node_->toString());
}

/* f64 constant */
IrFloat64Constant::IrFloat64Constant(IrContext *context,
                                     AstFloat64Constant *node)
    : IrConstant(context, IrUtil::namegen("float64.constant")), node_(node) {}

IrType IrFloat64Constant::type() const { return IrType::Float64Constant; }

llvm::Value *IrFloat64Constant::codeGen() {
  return llvm::ConstantFP::get(context_->llvmContext,
                               llvm::APFloat((double)node_->value()));
}

std::string IrFloat64Constant::toString() const {
  return fmt::format("[@IrFloat64Constant node_:{}]", node_->toString());
}

/* string constant */
IrStringConstant::IrStringConstant(IrContext *context, AstStringConstant *node)
    : IrConstant(context, IrUtil::namegen("string.constant")), node_(node) {}

IrType IrStringConstant::type() const { return IrType::StringConstant; }

llvm::Value *IrStringConstant::codeGen() { return nullptr; }

std::string IrStringConstant::toString() const {
  return fmt::format("[@IrStringConstant node_:{}]", node_->toString());
}

/* boolean constant */
IrBooleanConstant::IrBooleanConstant(IrContext *context,
                                     AstBooleanConstant *node)
    : IrConstant(context, IrUtil::namegen("boolean.constant")), node_(node) {}

IrType IrBooleanConstant::type() const { return IrType::BooleanConstant; }

llvm::Value *IrBooleanConstant::codeGen() {
  return node_->value() ? context_->llvmBuilder.getTrue()
                        : context_->llvmBuilder.getFalse();
#if 0
  return llvm::ConstantInt::get(
      context_->llvmContext,
      llvm::APInt(1, node_->value() ? (uint64_t)1U : (uint64_t)0U, false));
#endif
}

std::string IrBooleanConstant::toString() const {
  return fmt::format("[@IrBooleanConstant node_:{}]", node_->toString());
}

/* call expression */
IrCallExpression::IrCallExpression(IrContext *context, AstCallExpression *node)
    : IrExpression(context, IrUtil::namegen("call.expression")), node_(node) {}

IrType IrCallExpression::type() const { return IrType::CallExpression; }

llvm::Value *IrCallExpression::codeGen() { return nullptr; }

std::string IrCallExpression::toString() const {
  return fmt::format("[@IrCallExpression node_:{}]", node_->toString());
}

/* unary expression */
IrUnaryExpression::IrUnaryExpression(IrContext *context,
                                     AstUnaryExpression *node)
    : IrExpression(context, IrUtil::namegen("unary.expression")), node_(node),
      expression_(nullptr) {
  EX_ASSERT(node_, "node_ must not null");
  EX_ASSERT(node_->expression(), "node_->expression must not null");
  expression_ = IrFactory::expr(context_, node_->expression());
}

IrUnaryExpression::~IrUnaryExpression() {
  delete expression_;
  expression_ = nullptr;
}

IrType IrUnaryExpression::type() const { return IrType::UnaryExpression; }

llvm::Value *IrUnaryExpression::codeGen() {
  llvm::Value *e = expression_->codeGen();
  EX_ASSERT(e, "e must not be null");
  switch (node_->token()) {
  case T_ADD: {
    return e;
  } break;
  case T_SUB: {
    switch (e->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFNeg(e);
    case llvm::Type::TypeID::IntegerTyID:
      return context_->llvmBuilder.CreateNeg(e);
    default:
      break;
    }
  } break;
  case T_BIT_NOT: {
    return context_->llvmBuilder.CreateNot(e);
  } break;
  case T_LOGIC_NOT: {
    EX_ASSERT(
        e->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "e->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID {}",
        e->getType()->getTypeID(), llvm::Type::TypeID::IntegerTyID);

    EX_ASSERT(llvm::dyn_cast<llvm::IntegerType>(e->getType())->getBitWidth() ==
                  1U,
              "e->getType->getBitWidth {} == 1",
              llvm::dyn_cast<llvm::IntegerType>(e->getType())->getBitWidth());
    return context_->llvmBuilder.CreateNot(e);
  } break;
  default:
    EX_ASSERT(false, "invalid node_->token {}", tokenName(node_->token()));
  }
  EX_ASSERT(false, "invalid e->getType->getTypeID {}, expression_:{}",
            e->getType()->getTypeID(), expression_->toString());
  return nullptr;
}

std::string IrUnaryExpression::toString() const {
  return fmt::format("[@IrUnaryExpression node_:{}]", node_->toString());
}

/* binary expression */
IrBinaryExpression::IrBinaryExpression(IrContext *context,
                                       AstBinaryExpression *node)
    : IrExpression(context, IrUtil::namegen("binary.expression")), node_(node),
      left_(nullptr), right_(nullptr) {
  EX_ASSERT(node_->left(), "node_->left is null");
  EX_ASSERT(node_->right(), "node_->right is null");
  left_ = IrFactory::expr(context_, node->left());
  right_ = IrFactory::expr(context_, node->right());
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
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFAdd(l, r);
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID:
        return context_->llvmBuilder.CreateFAdd(l, r);
      case llvm::Type::TypeID::IntegerTyID:
        return context_->llvmBuilder.CreateAdd(l, r);
      default:
        break;
      }
    } break;
    default:
      break;
    }
  } break;
  case T_SUB: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFSub(l, r);
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID:
        return context_->llvmBuilder.CreateFSub(l, r);
      case llvm::Type::TypeID::IntegerTyID:
        return context_->llvmBuilder.CreateSub(l, r);
      default:
        break;
      }
    } break;
    default:
      break;
    }
  } break;
  case T_MUL: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFMul(l, r);
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID:
        return context_->llvmBuilder.CreateFMul(l, r);
      case llvm::Type::TypeID::IntegerTyID:
        return context_->llvmBuilder.CreateMul(l, r);
      default:
        break;
      }
    } break;
    default:
      break;
    }
  } break;
  case T_DIV: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFDiv(l, r);
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context_->llvmBuilder.CreateFDiv(l, r);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context_->llvmBuilder.CreateSDiv(l, r);
      } break;
      default:
        break;
      }
    } break;
    default:
      break;
    }
  } break;
  case T_MOD: {
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFRem(l, r);
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID:
        return context_->llvmBuilder.CreateFRem(l, r);
      case llvm::Type::TypeID::IntegerTyID:
        return context_->llvmBuilder.CreateSRem(l, r);
      default:
        break;
      }
    } break;
    default:
      break;
    }
  } break;
  case T_BIT_LSHIFT:
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    return context_->llvmBuilder.CreateShl(l, r);
  case T_BIT_RSHIFT:
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    return context_->llvmBuilder.CreateLShr(l, r);
  case T_BIT_ARSHIFT:
    EX_ASSERT(l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "l->getType->getTypeID {} not integer",
              l->getType()->getTypeID());
    EX_ASSERT(r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
              "r->getType->getTypeID {} not integer",
              r->getType()->getTypeID());
    return context_->llvmBuilder.CreateAShr(l, r);
  case T_EQ: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFCmpOEQ(l, r);
    case llvm::Type::TypeID::IntegerTyID:
      return context_->llvmBuilder.CreateICmpEQ(l, r);
    default:
      break;
    }
  } break;
  case T_NEQ: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFCmpONE(l, r);
    case llvm::Type::TypeID::IntegerTyID:
      return context_->llvmBuilder.CreateICmpNE(l, r);
    default:
      break;
    }
  } break;
  case T_LE: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFCmpOLE(l, r);
    case llvm::Type::TypeID::IntegerTyID:
      return context_->llvmBuilder.CreateICmpSLE(l, r);
    default:
      break;
    }
  } break;
  case T_LT: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFCmpOLT(l, r);
    case llvm::Type::TypeID::IntegerTyID:
      return context_->llvmBuilder.CreateICmpSLT(l, r);
    default:
      break;
    }
  } break;
  case T_GE: {
    EX_ASSERT(l->getType()->getTypeID() == r->getType()->getTypeID(),
              "l->getType->getTypeID {} != r->getType->getTypeID {}",
              l->getType()->getTypeID(), r->getType()->getTypeID());
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFCmpOGE(l, r);
    case llvm::Type::TypeID::IntegerTyID:
      return context_->llvmBuilder.CreateICmpSGE(l, r);
    default:
      break;
    }
  } break;
  case T_GT: {
    EX_ASSERT(
        l->getType()->getTypeID() == r->getType()->getTypeID(),
        "l->getType->getTypeID {} {} == r->getType->getTypeID {} {}",
        l->getType()->getTypeID(), IrUtil::dumpLLVM<llvm::Type>(l->getType()),
        r->getType()->getTypeID(), IrUtil::dumpLLVM<llvm::Type>(r->getType()));
    switch (l->getType()->getTypeID()) {
    case llvm::Type::TypeID::FloatTyID:
    case llvm::Type::TypeID::DoubleTyID:
      return context_->llvmBuilder.CreateFCmpOGT(l, r);
    case llvm::Type::TypeID::IntegerTyID:
      return context_->llvmBuilder.CreateICmpSGT(l, r);
    default:
      break;
    }
  } break;
  case T_BIT_AND:
    EX_ASSERT(
        l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "l->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID",
        l->getType()->getTypeID());
    EX_ASSERT(
        r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "r->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID",
        r->getType()->getTypeID());
    return context_->llvmBuilder.CreateAnd(l, r);
  case T_BIT_OR:
    EX_ASSERT(
        l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "l->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID",
        l->getType()->getTypeID());
    EX_ASSERT(
        r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "r->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID",
        r->getType()->getTypeID());
    return context_->llvmBuilder.CreateOr(l, r);
  case T_BIT_XOR:
    EX_ASSERT(
        l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "l->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID",
        l->getType()->getTypeID());
    EX_ASSERT(
        r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "r->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID",
        r->getType()->getTypeID());
    return context_->llvmBuilder.CreateXor(l, r);
  case T_LOGIC_AND:
    EX_ASSERT(
        l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "l->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID",
        l->getType()->getTypeID());
    EX_ASSERT(
        r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "r->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID",
        r->getType()->getTypeID());
    EX_ASSERT(llvm::dyn_cast<llvm::ConstantInt>(l)->getBitWidth() == 1,
              "l->getBitWidth {} != 1",
              llvm::dyn_cast<llvm::ConstantInt>(l)->getBitWidth());
    EX_ASSERT(llvm::dyn_cast<llvm::ConstantInt>(r)->getBitWidth() == 1,
              "r->getBitWidth {} != 1",
              llvm::dyn_cast<llvm::ConstantInt>(r)->getBitWidth());
    return context_->llvmBuilder.CreateAnd(l, r);
  case T_LOGIC_OR:
    EX_ASSERT(
        l->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "l->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID",
        l->getType()->getTypeID());
    EX_ASSERT(
        r->getType()->getTypeID() == llvm::Type::TypeID::IntegerTyID,
        "r->getType->getTypeID {} must be llvm::Type::TypeID::IntegerTyID",
        r->getType()->getTypeID());
    EX_ASSERT(llvm::dyn_cast<llvm::ConstantInt>(l)->getBitWidth() == 1,
              "l->getBitWidth {} != 1",
              llvm::dyn_cast<llvm::ConstantInt>(l)->getBitWidth());
    EX_ASSERT(llvm::dyn_cast<llvm::ConstantInt>(r)->getBitWidth() == 1,
              "r->getBitWidth {} != 1",
              llvm::dyn_cast<llvm::ConstantInt>(r)->getBitWidth());
    return context_->llvmBuilder.CreateOr(l, r);
  default:
    EX_ASSERT(false, "token {} invalid", node_->token());
  }
  EX_ASSERT(false,
            "token: {}, l->getType->getTypeID {} {} or r->getType->getTypeID "
            "{} {} invalid, l "
            "source:{} value:{} ir:{}, r source:{} value:{} ir:{}",
            tokenName(node_->token()), l->getType()->getTypeID(),
            IrUtil::dumpLLVM<llvm::Type>(l->getType()),
            r->getType()->getTypeID(),
            IrUtil::dumpLLVM<llvm::Type>(r->getType()),
            dumpSource(context_->sourceName, node_->left()->position()),
            node_->left()->toString(), IrUtil::dumpLLVM<llvm::Value>(l),
            dumpSource(context_->sourceName, node_->right()->position()),
            node_->right()->toString(), IrUtil::dumpLLVM<llvm::Value>(r));
  return nullptr;
}

std::string IrBinaryExpression::toString() const {
  return fmt::format("[@IrBinaryExpression node_:{}]", node_->toString());
}

/* conditional expression */
IrConditionalExpression::IrConditionalExpression(IrContext *context,
                                                 AstConditionalExpression *node)
    : IrExpression(context, IrUtil::namegen("conditional.expression")),
      node_(node), condition_(nullptr), thens_(nullptr), elses_(nullptr) {
  EX_ASSERT(node_, "node_ must not null");
  EX_ASSERT(node_->condition(), "node_->condition must not null");
  EX_ASSERT(node_->thens(), "node_->thens must not null");
  EX_ASSERT(node_->elses(), "node_->elses must not null");
  condition_ = IrFactory::expr(context_, node_->condition());
  thens_ = IrFactory::expr(context_, node_->thens());
  elses_ = IrFactory::expr(context_, node_->elses());
}

IrConditionalExpression::~IrConditionalExpression() {
  delete condition_;
  condition_ = nullptr;
  delete thens_;
  thens_ = nullptr;
  delete elses_;
  elses_ = nullptr;
}

IrType IrConditionalExpression::type() const {
  return IrType::ConditionalExpression;
}

llvm::Value *IrConditionalExpression::codeGen() {
  llvm::Value *condV = condition_->codeGen();
  EX_ASSERT(condV, "condV is null");
  condV = context_->llvmBuilder.CreateICmpNE(
      condV,
      llvm::ConstantInt::get(context_->llvmContext,
                             llvm::APInt(1, (uint64_t)0, false)),
      name() + "ifcond");
  llvm::Function *func = context_->llvmBuilder.GetInsertBlock()->getParent();
  llvm::BasicBlock *thenBlock =
      llvm::BasicBlock::Create(context_->llvmContext, name() + "then", func);
  llvm::BasicBlock *elseBlock =
      llvm::BasicBlock::Create(context_->llvmContext, name() + "else");
  llvm::BasicBlock *mergeBlock =
      llvm::BasicBlock::Create(context_->llvmContext, name() + "ifcont");
  context_->llvmBuilder.CreateCondBr(condV, thenBlock, elseBlock);
  context_->llvmBuilder.SetInsertPoint(thenBlock);
  llvm::Value *thenV = thens_->codeGen();
  EX_ASSERT(thenV, "thenV is null");
  context_->llvmBuilder.CreateBr(mergeBlock);
  thenBlock = context_->llvmBuilder.GetInsertBlock();
  func->getBasicBlockList().push_back(elseBlock);
  context_->llvmBuilder.SetInsertPoint(elseBlock);
  llvm::Value *elseV = elses_->codeGen();
  EX_ASSERT(elseV, "elseV is null");
  context_->llvmBuilder.CreateBr(mergeBlock);
  elseBlock = context_->llvmBuilder.GetInsertBlock();
  func->getBasicBlockList().push_back(mergeBlock);
  context_->llvmBuilder.SetInsertPoint(mergeBlock);
  llvm::PHINode *pn = context_->llvmBuilder.CreatePHI(
      llvm::Type::getDoubleTy(context_->llvmContext), 2, "iftmp");
  pn->addIncoming(thenV, thenBlock);
  pn->addIncoming(elseV, elseBlock);
  return pn;
}

std::string IrConditionalExpression::toString() const {
  return fmt::format("[@IrConditionalExpression node_:{}]", node_->toString());
}

/* assignment expression */
IrAssignmentExpression::IrAssignmentExpression(IrContext *context,
                                               AstAssignmentExpression *node)
    : IrExpression(context, IrUtil::namegen("assignment.expression")),
      node_(node), value_(nullptr) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->variable(), "node_->variable is null");
  EX_ASSERT(node_->value(), "node_->value is null");
  value_ = IrFactory::expr(context_, node->value());
}

IrAssignmentExpression::~IrAssignmentExpression() {
  delete value_;
  value_ = nullptr;
}

IrType IrAssignmentExpression::type() const {
  return IrType::AssignmentExpression;
}

llvm::Value *IrAssignmentExpression::codeGen() {
  EX_ASSERT(node_->variable()->type() == (+AstType::IdentifierConstant),
            "node_->variable type {} must be identifier",
            node_->variable()->type()._to_string());
  AstIdentifierConstant *varId = DC(AstIdentifierConstant, node_->variable());
  ScopeNode *varNode = context_->symbolTable->current->resolve(varId->value());
  EX_ASSERT(varNode, "varNode must not null");
  EX_ASSERT(*varNode != ScopeNode::invalid(), "varNode {} must be valid",
            varNode->toString());
  EX_ASSERT(varNode->value, "varNode.value {} must not null",
            varNode->toString());
  llvm::Value *v = value_->codeGen();
  context_->llvmBuilder.CreateStore(v, varNode->value);
  return v;
}

std::string IrAssignmentExpression::toString() const {
  return fmt::format("[@IrAssignmentExpression node_:{}]", node_->toString());
}

/* sequel expression */
IrSequelExpression::IrSequelExpression(IrContext *context,
                                       AstSequelExpression *node)
    : IrExpression(context, IrUtil::namegen("sequel.expression")), node_(node),
      expressionList_(nullptr) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->expressionList(), "node_->expressionList is null");
  expressionList_ = new IrExpressionList(context_);
  for (int i = 0; i < node_->expressionList()->size(); i++) {
    expressionList_->add(
        IrFactory::expr(context_, node_->expressionList()->get(i)));
  }
}

IrSequelExpression::~IrSequelExpression() {
  delete expressionList_;
  expressionList_ = nullptr;
}

IrType IrSequelExpression::type() const { return IrType::SequelExpression; }

IrExpressionList *IrSequelExpression::expressionList() {
  return expressionList_;
}

llvm::Value *IrSequelExpression::codeGen() {
  return expressionList_->codeGen();
}

std::string IrSequelExpression::toString() const {
  return fmt::format("[@IrSequelExpression node_:{}]", node_->toString());
}

IrVoidExpression::IrVoidExpression(IrContext *context, AstVoidExpression *node)
    : IrExpression(context, IrUtil::namegen("void.expression")), node_(node) {
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
    : IrStatement(context, IrUtil::namegen("expression.statement")),
      node_(node), expression_(nullptr) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->expression(), "node_->expression is null");
  expression_ = IrFactory::expr(context_, node->expression());
}

void IrExpressionStatement::buildSymbol() { expression_->buildSymbol(); }

void IrExpressionStatement::checkSymbol() const {}

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
    : IrStatement(context, IrUtil::namegen("compound.statement")), node_(node),
      statementList_(nullptr) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->statementList(), "node_->statementList is null");
  statementList_ = new IrStatementList(context);
  for (int i = 0; i < node_->statementList()->size(); i++) {
    Ast *ast = node_->statementList()->get(i);
    EX_ASSERT(ast, "the {} ast is null", i);
    if (ast->type() == (+AstType::EmptyStatement)) {
      continue;
    }
    IrStatement *ir = nullptr;
    switch (ast->type()) {
    case AstType::IfStatement:
      ir = new IrIfStatement(context_, DC(AstIfStatement, ast));
      break;
    case AstType::CompoundStatement:
      ir = new IrCompoundStatement(context_, DC(AstCompoundStatement, ast));
      break;
    case AstType::ExpressionStatement:
      ir = new IrExpressionStatement(context_, DC(AstExpressionStatement, ast));
      break;
    case AstType::ForStatement:
      ir = new IrForStatement(context_, DC(AstForStatement, ast));
      break;
    case AstType::WhileStatement:
      ir = new IrWhileStatement(context_, DC(AstWhileStatement, ast));
      break;
    case AstType::ReturnStatement:
      ir = new IrReturnStatement(context_, DC(AstReturnStatement, ast));
      break;
    case AstType::VariableDefinition:
      ir = new IrLocalVariableDefinition(context_,
                                         DC(AstVariableDefinition, ast));
      break;
    case AstType::EmptyStatement:
    case AstType::FunctionDefinition:
    case AstType::ContinueStatement:
    case AstType::BreakStatement:
      break;
    default:
      EX_ASSERT(false, "invalid ast:{}", ast->toString());
    }
    if (ir) {
      statementList_->add(ir);
    }
  }
}

void IrCompoundStatement::buildSymbol() {
  LocalScope *loc =
      new LocalScope(node_->name(), context_->symbolTable->current);
  context_->symbolTable->current->define(
      new ScopeNode(loc, ScopeType::ty_local(), node_));
  context_->symbolTable->push(loc);
  for (int i = 0; i < statementList_->size(); i++) {
    statementList_->get(i)->buildSymbol();
  }
  context_->symbolTable->pop();
}

void IrCompoundStatement::checkSymbol() const {}

IrCompoundStatement::~IrCompoundStatement() {
  delete statementList_;
  statementList_ = nullptr;
}

IrType IrCompoundStatement::type() const { return IrType::CompoundStatement; }

llvm::Value *IrCompoundStatement::codeGen() {
  ScopeNode *localScope =
      context_->symbolTable->current->resolve(node_->name());
  EX_ASSERT(localScope, "localScope must not null");
  context_->symbolTable->push(DC(LocalScope, localScope->symbol));
  llvm::Value *ret = statementList_->codeGen();
  context_->symbolTable->pop();
  return ret;
}

std::string IrCompoundStatement::toString() const {
  return fmt::format("[@IrCompoundStatement node_:{}]", node_->toString());
}

/* if statement */
IrIfStatement::IrIfStatement(IrContext *context, AstIfStatement *node)
    : IrStatement(context, IrUtil::namegen("if.statement")), node_(node),
      condition_(nullptr), thens_(nullptr), elses_(nullptr) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->condition(), "node_->condition is null");
  EX_ASSERT(node_->thens(), "node_->thens is null");
  EX_ASSERT(node_->elses(), "node_->elses is null");
  condition_ = IrFactory::expr(context_, node->condition());
  thens_ = IrFactory::stmt(context_, node->thens());
  elses_ = IrFactory::stmt(context_, node->elses());
}

void IrIfStatement::buildSymbol() {
  condition_->buildSymbol();
  thens_->buildSymbol();
  elses_->buildSymbol();
}

void IrIfStatement::checkSymbol() const {
  condition_->checkSymbol();
  thens_->checkSymbol();
  elses_->checkSymbol();
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
  condV = context_->llvmBuilder.CreateICmpNE(
      condV,
      llvm::ConstantInt::get(context_->llvmContext,
                             llvm::APInt(1, (uint64_t)0, false)),
      name() + "ifcond");
  llvm::Function *func = context_->llvmBuilder.GetInsertBlock()->getParent();
  llvm::BasicBlock *thenBlock =
      llvm::BasicBlock::Create(context_->llvmContext, name() + "then", func);
  llvm::BasicBlock *elseBlock =
      llvm::BasicBlock::Create(context_->llvmContext, name() + "else");
  llvm::BasicBlock *mergeBlock =
      llvm::BasicBlock::Create(context_->llvmContext, name() + "ifcont");
  context_->llvmBuilder.CreateCondBr(condV, thenBlock, elseBlock);
  context_->llvmBuilder.SetInsertPoint(thenBlock);
  llvm::Value *thenV = thens_->codeGen();
  EX_ASSERT(thenV, "thenV is null");
  context_->llvmBuilder.CreateBr(mergeBlock);
  thenBlock = context_->llvmBuilder.GetInsertBlock();
  func->getBasicBlockList().push_back(elseBlock);
  context_->llvmBuilder.SetInsertPoint(elseBlock);
  llvm::Value *elseV = elses_->codeGen();
  EX_ASSERT(elseV, "elseV is null");
  context_->llvmBuilder.CreateBr(mergeBlock);
  elseBlock = context_->llvmBuilder.GetInsertBlock();
  func->getBasicBlockList().push_back(mergeBlock);
  context_->llvmBuilder.SetInsertPoint(mergeBlock);
  llvm::PHINode *pn = context_->llvmBuilder.CreatePHI(
      llvm::Type::getDoubleTy(context_->llvmContext), 2, "iftmp");
  pn->addIncoming(thenV, thenBlock);
  pn->addIncoming(elseV, elseBlock);
  return pn;
}

std::string IrIfStatement::toString() const {
  return fmt::format("[@IrIfStatement node_:{}]", node_->toString());
}

/* while statement */
IrWhileStatement::IrWhileStatement(IrContext *context, AstWhileStatement *node)
    : IrStatement(context, IrUtil::namegen("while.statement")), node_(node),
      condition_(nullptr), statement_(nullptr) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->condition(), "node_->condition is null");
  EX_ASSERT(node_->statement(), "node_->statement is null");
  condition_ = IrFactory::expr(context_, node_->condition());
  statement_ = IrFactory::stmt(context_, node_->statement());
}

void IrWhileStatement::buildSymbol() {
  condition_->buildSymbol();
  statement_->buildSymbol();
}

void IrWhileStatement::checkSymbol() const {}

IrType IrWhileStatement::type() const { return IrType::WhileStatement; }

llvm::Value *IrWhileStatement::codeGen() { return nullptr; }

std::string IrWhileStatement::toString() const {
  return fmt::format("[@IrWhileStatement node_:{}]", node_->toString());
}

/* for statement */
IrForStatement::IrForStatement(IrContext *context, AstForStatement *node)
    : IrStatement(context, IrUtil::namegen("for.statement")), node_(node),
      start_(nullptr), step_(nullptr), end_(nullptr), statement_(nullptr) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->start(), "node_->start is null");
  EX_ASSERT(node_->step(), "node_->step is null");
  EX_ASSERT(node_->end(), "node_->end is null");
  EX_ASSERT(node_->statement(), "node_->statement is null");
  start_ = IrFactory::stmt(context_, node->start());
  step_ = IrFactory::stmt(context_, node->step());
  end_ = IrFactory::expr(context_, node->end());
  statement_ = IrFactory::stmt(context_, node->statement());
}

void IrForStatement::buildSymbol() {
  LocalScope *loc =
      new LocalScope(node_->name(), context_->symbolTable->current);
  context_->symbolTable->push(loc);
  start_->buildSymbol();
  step_->buildSymbol();
  end_->buildSymbol();
  statement_->buildSymbol();
  context_->symbolTable->pop();
}

void IrForStatement::checkSymbol() const {}

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
  llvm::Function *func = context_->llvmBuilder.GetInsertBlock()->getParent();
  llvm::BasicBlock *preheaderBB = context_->llvmBuilder.GetInsertBlock();
  llvm::BasicBlock *loopBB =
      llvm::BasicBlock::Create(context_->llvmContext, name() + "loop", func);
  context_->llvmBuilder.CreateBr(loopBB);
  context_->llvmBuilder.SetInsertPoint(loopBB);

  llvm::PHINode *variable = context_->llvmBuilder.CreatePHI(
      llvm::Type::getDoubleTy(context_->llvmContext), 2, name() + "loop.phi");
  variable->addIncoming(startV, preheaderBB);
  llvm::Value *bodyV = statement_->codeGen();
  EX_ASSERT(bodyV, "bodyV is null");
  llvm::Value *stepV = nullptr;
  if (step_) {
    stepV = step_->codeGen();
    EX_ASSERT(stepV, "stepV is null");
  } else {
    stepV = llvm::ConstantInt::get(context_->llvmContext,
                                   llvm::APInt(1, (uint64_t)1, false));
  }
  llvm::Value *nextV =
      context_->llvmBuilder.CreateAdd(variable, stepV, name() + "nextvar");
  llvm::Value *endCond = end_->codeGen();
  EX_ASSERT(endCond, "endCond is null");
  endCond = context_->llvmBuilder.CreateICmpNE(
      endCond, llvm::ConstantInt::get(context_->llvmContext,
                                      llvm::APInt(1, (uint64_t)0, false)));
  llvm::BasicBlock *loopEndBB = context_->llvmBuilder.GetInsertBlock();
  llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(
      context_->llvmContext, name() + "afterloop", func);
  context_->llvmBuilder.CreateCondBr(endCond, loopBB, afterBB);
  context_->llvmBuilder.SetInsertPoint(afterBB);
  variable->addIncoming(nextV, loopEndBB);
  return llvm::Constant::getNullValue(
      llvm::Type::getDoubleTy(context_->llvmContext));
}

std::string IrForStatement::toString() const {
  return fmt::format("[@IrForStatement node_:{}]", node_->toString());
}

/* return statement */
IrReturnStatement::IrReturnStatement(IrContext *context,
                                     AstReturnStatement *node)
    : IrStatement(context, IrUtil::namegen("return.statement")), node_(node),
      expression_(nullptr) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->expression(), "node_->expression is null");
  expression_ = IrFactory::expr(context_, node->expression());
}

void IrReturnStatement::buildSymbol() { expression_->buildSymbol(); }

void IrReturnStatement::checkSymbol() const {}

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
    return context_->llvmBuilder.CreateRetVoid();
  case IrType::SequelExpression: {
    IrExpressionList *expressionList =
        DC(IrSequelExpression, expression_)->expressionList();
    EX_ASSERT(expressionList->size() > 0, "expressionList->size {} > 0",
              expressionList->size());
    llvm::Value *exprV = expressionList->codeGen();
    return context_->llvmBuilder.CreateRet(exprV);
  } break;
  default:
    EX_ASSERT(false, "invalid expression_:{}", expression_->toString());
  }
  return nullptr;
}

/* variable definition */
static void variableDefinitionBuildSymbol(AstVariableDefinition *node,
                                          SymbolTable *symbolTable) {
  for (int i = 0; i < node->definitionList()->size(); i++) {
    AstVariableInitialDefinition *ast =
        DC(AstVariableInitialDefinition, node->definitionList()->get(i));
    VariableSymbol *varSym =
        new VariableSymbol(ast->identifier(), symbolTable->current);
    BuiltinType *varTy = nullptr;
    switch (ast->expression()->type()) {
    case AstType::Int8Constant:
      varTy = BuiltinType::ty_int8();
    case AstType::UInt8Constant:
      varTy = BuiltinType::ty_uint8();
    case AstType::Int16Constant:
      varTy = BuiltinType::ty_int16();
    case AstType::UInt16Constant:
      varTy = BuiltinType::ty_uint16();
    case AstType::Int32Constant:
      varTy = BuiltinType::ty_int32();
    case AstType::UInt32Constant:
      varTy = BuiltinType::ty_uint32();
    case AstType::Int64Constant:
      varTy = BuiltinType::ty_int64();
    case AstType::UInt64Constant:
      varTy = BuiltinType::ty_uint64();
    case AstType::Float32Constant:
      varTy = BuiltinType::ty_float32();
    case AstType::Float64Constant:
      varTy = BuiltinType::ty_float64();
    case AstType::BooleanConstant:
      varTy = BuiltinType::ty_boolean();
    case AstType::StringConstant:
      varTy = BuiltinType::ty_string();
    default:
      LOG_WARN("warning default builtin type:{}", ast->toString());
      varTy = BuiltinType::ty_void();
    }
    symbolTable->current->define(new ScopeNode(varSym, varTy, ast));
  }
}

/* global variable */
IrGlobalVariableDefinition::IrGlobalVariableDefinition(
    IrContext *context, AstVariableDefinition *node)
    : IrDefinition(context, IrUtil::namegen("global.variable.definition")),
      node_(node), expressionList_(nullptr) {
  EX_ASSERT(node_, "node_ must not null");
  EX_ASSERT(node_->definitionList(), "node_->definitionList must not null");
  expressionList_ = new IrExpressionList(context_);
  for (int i = 0; i < node_->definitionList()->size(); i++) {
    AstVariableInitialDefinition *ast =
        DC(AstVariableInitialDefinition, node_->definitionList()->get(i));
    expressionList_->add(IrFactory::expr(context_, ast->expression()));
  }
}

IrGlobalVariableDefinition::~IrGlobalVariableDefinition() {
  delete expressionList_;
  expressionList_ = nullptr;
}

std::string IrGlobalVariableDefinition::toString() const {
  return fmt::format("[@IrGlobalVariableDefinition node_:{}]",
                     node_->toString());
}

IrType IrGlobalVariableDefinition::type() const {
  return IrType::GlobalVariableDefinition;
}

static Symbol *variableParentScope(VariableSymbol *varSym) {
  Symbol *scope = DC(Symbol, varSym->enclosingScope());
  while (scope->type() != (+SymType::Function) &&
         scope->type() != (+SymType::Global)) {
    scope = scope->enclosingScope();
  }
  return scope;
}

llvm::Value *IrGlobalVariableDefinition::codeGen() {
  EX_ASSERT(node_->definitionList(), "node_->definitionList is null");
  llvm::Value *ret = nullptr;
  for (int i = 0; i < node_->definitionList()->size(); i++) {
    AstVariableInitialDefinition *ast =
        DC(AstVariableInitialDefinition, node_->definitionList()->get(i));
    ScopeNode *varNode =
        context_->symbolTable->current->resolve(ast->identifier());
    EX_ASSERT(*varNode != ScopeNode::invalid(), "varNode {} must be valid",
              varNode->toString());
    EX_ASSERT(!varNode->value, "varNode.value {} must be null",
              varNode->toString());
    VariableSymbol *varSym = DC(VariableSymbol, varNode->symbol);
    Symbol *enclose = variableParentScope(varSym);
    EX_ASSERT(enclose->type() == (+SymType::Global),
              "enclosingScope type {} is not global",
              enclose->type()._to_string());
    llvm::Constant *initial =
        llvm::dyn_cast<llvm::Constant>(expressionList_->get(i)->codeGen());
    std::string varIrName = IrUtil::toLLVMName(ast->identifier());
    llvm::GlobalVariable *gv = new llvm::GlobalVariable(
        *context_->llvmModule, llvm::Type::getDoubleTy(context_->llvmContext),
        false, llvm::GlobalValue::LinkageTypes::CommonLinkage, initial,
        varIrName);
    ret = varNode->value = llvm::dyn_cast<llvm::Value>(gv);
  }
  return ret;
}

void IrGlobalVariableDefinition::buildSymbol() {
  variableDefinitionBuildSymbol(node_, context_->symbolTable);
}

void IrGlobalVariableDefinition::checkSymbol() const {}

/* local variable in function */
IrLocalVariableDefinition::IrLocalVariableDefinition(
    IrContext *context, AstVariableDefinition *node)
    : IrDefinition(context, IrUtil::namegen("local.variable.definition")),
      node_(node) {}

std::string IrLocalVariableDefinition::toString() const {
  return fmt::format("[@IrLocalVariableDefinition node_:{}]",
                     node_->toString());
}

IrType IrLocalVariableDefinition::type() const {
  return IrType::LocalVariableDefinition;
}

llvm::Value *IrLocalVariableDefinition::codeGen() {
  EX_ASSERT(node_->definitionList(), "node_->definitionList is null");
  llvm::Value *ret = nullptr;
  FunctionSymbol *funcSym = nullptr;
  llvm::Function *func = nullptr;
  for (int i = 0; i < node_->definitionList()->size(); i++) {
    AstVariableInitialDefinition *ast =
        DC(AstVariableInitialDefinition, node_->definitionList()->get(i));
    ScopeNode *varNode =
        context_->symbolTable->current->resolve(ast->identifier());
    EX_ASSERT(varNode, "varNode must not null");
    EX_ASSERT(*varNode != ScopeNode::invalid(), "varNode {} must be valid",
              varNode->toString());
    EX_ASSERT(!varNode->value, "varNode.value {} must be null",
              varNode->toString());
    VariableSymbol *varSym = DC(VariableSymbol, varNode->symbol);
    Symbol *enclose = variableParentScope(varSym);
    EX_ASSERT(enclose->type() == (+SymType::Function),
              "enclosingScope type {} is not function",
              enclose->type()._to_string());
    if (funcSym) {
      EX_ASSERT((void *)funcSym == (void *)enclose, "funcSym {} == enclose {}",
                (void *)funcSym, (void *)enclose);
    }
    funcSym = DC(FunctionSymbol, enclose);
    ScopeNode *funcNode =
        context_->symbolTable->current->resolve(funcSym->name());
    EX_ASSERT(funcNode, "funcNode must not null");
    EX_ASSERT(*funcNode != ScopeNode::invalid(), "funcNode {} must be valid",
              funcNode->toString());
    EX_ASSERT(funcNode->value, "funcNode.value {} must not null",
              funcNode->toString());
    if (func) {
      EX_ASSERT((void *)func == (void *)funcNode->value,
                "func {} == funcNode.value {} {}", (void *)func,
                (void *)funcNode->value, funcNode->toString());
    } else {
      func = llvm::dyn_cast<llvm::Function>(funcNode->value);
    }
  }
  EX_ASSERT(funcSym, "funcSym is null");
  EX_ASSERT(func, "func is null");
  llvm::IRBuilder<> varBuilder(&func->getEntryBlock(),
                               func->getEntryBlock().begin());
  for (int i = 0; i < node_->definitionList()->size(); i++) {
    AstVariableInitialDefinition *ast =
        DC(AstVariableInitialDefinition, node_->definitionList()->get(i));
    ScopeNode *varNode =
        context_->symbolTable->current->resolve(ast->identifier());
    EX_ASSERT(varNode, "varNode must not null");
    std::string varIrName = IrUtil::toLLVMName(ast->identifier());
    llvm::AllocaInst *varAlloca = varBuilder.CreateAlloca(
        llvm::Type::getDoubleTy(context_->llvmContext), 0, varIrName);
    EX_ASSERT(!varNode->value, "varNode.value {} must not null",
              varNode->toString());
    varNode->value = ret = llvm::dyn_cast<llvm::Value>(varAlloca);
  }
  return ret;
}

void IrLocalVariableDefinition::buildSymbol() {
  variableDefinitionBuildSymbol(node_, context_->symbolTable);
}

void IrLocalVariableDefinition::checkSymbol() const {}

/* function definition */
IrFunctionDefinition::IrFunctionDefinition(IrContext *context,
                                           AstFunctionDefinition *node)
    : IrDefinition(context, IrUtil::namegen("function.definition")),
      node_(node), signature_(nullptr), statement_(nullptr) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->signature(), "node_->signature is null");
  EX_ASSERT(node_->statement(), "node_->statement is null");
  signature_ = new IrFunctionSignatureDefinition(context_, node->signature());
  statement_ = IrFactory::stmt(context_, node_->statement());
}

void IrFunctionDefinition::buildSymbol() {
  AstFunctionSignatureDefinition *sign = node_->signature();
  FunctionSymbol *funcSym =
      new FunctionSymbol(sign->identifier(), context_->symbolTable->current);
  std::vector<Type *> funcArgTypeList;
  for (int i = 0; i < sign->argumentList()->size(); i++) {
    AstFunctionArgumentDefinition *funcArg =
        DC(AstFunctionArgumentDefinition, sign->argumentList()->get(i));
    EX_ASSERT(funcArg, "funcArg is null");
    funcArgTypeList.push_back(BuiltinType::ty_void());
  }
  FunctionType *funcTy =
      new FunctionType(funcArgTypeList, BuiltinType::ty_void());
  context_->symbolTable->current->define(new ScopeNode(funcSym, funcTy, node_));
  context_->symbolTable->push(funcSym);
  for (int i = 0; i < sign->argumentList()->size(); i++) {
    AstFunctionArgumentDefinition *funcArg =
        DC(AstFunctionArgumentDefinition, sign->argumentList()->get(i));
    EX_ASSERT(funcArg, "funcArg is null");
    FunctionArgumentSymbol *funcArgSym = new FunctionArgumentSymbol(
        funcArg->identifier(), context_->symbolTable->current);
    context_->symbolTable->current->define(
        new ScopeNode(funcArgSym, BuiltinType::ty_void(), funcArg));
  }
  statement_->buildSymbol();
  context_->symbolTable->pop();
}

void IrFunctionDefinition::checkSymbol() const {
  signature_->checkSymbol();
  statement_->checkSymbol();
}

IrType IrFunctionDefinition::type() const { return IrType::FunctionDefinition; }

llvm::Value *IrFunctionDefinition::codeGen() {
  std::string funcIrName = IrUtil::toLLVMName(node_->signature()->identifier());

  llvm::Function *func = context_->llvmModule->getFunction(funcIrName);
  if (!func) {
    func = llvm::dyn_cast<llvm::Function>(signature_->codeGen());
  }
  if (!func) {
    return nullptr;
  }
  EX_ASSERT(func->empty(), "Function {} cannot be redefined!",
            node_->signature()->identifier());
  llvm::BasicBlock *bb = llvm::BasicBlock::Create(context_->llvmContext,
                                                  funcIrName + ".entry", func);
  context_->llvmBuilder.SetInsertPoint(bb);

  ScopeNode *funcNode =
      context_->symbolTable->current->resolve(node_->signature()->identifier());
  EX_ASSERT(funcNode, "funcNode must not null");
  EX_ASSERT(funcNode->value == func, "funcNode->value {} == func {}",
            (void *)funcNode->value, (void *)func);
  context_->symbolTable->push(DC(FunctionSymbol, funcNode->symbol));

  // check function argument symbol exist
  for (auto &a : func->args()) {
    ScopeNode *argNode = context_->symbolTable->current->resolve(
        IrUtil::fromLLVMName(a.getName()));
    EX_ASSERT(argNode, "argNode must not null");
    EX_ASSERT(*argNode != ScopeNode::invalid(), "argNode {} must be valid",
              argNode->toString());
    argNode->value = &a;
  }

  llvm::Value *ret = statement_->codeGen();

  if (ret) {
    /* context_->llvmBuilder.CreateRet(ret); */
    llvm::verifyFunction(*func);
    context_->symbolTable->pop();
    return llvm::dyn_cast<llvm::Value>(func);
  }

  func->eraseFromParent();
  context_->symbolTable->pop();
  return nullptr;
}

std::string IrFunctionDefinition::toString() const {
  return fmt::format("[@IrFunctionDefinition node_:{}]", node_->toString());
}

/* function signature definition */
IrFunctionSignatureDefinition::IrFunctionSignatureDefinition(
    IrContext *context, AstFunctionSignatureDefinition *node)
    : IrDefinition(context, IrUtil::namegen("function.signature.definition")),
      node_(node) {
  EX_ASSERT(node_, "node_ is null");
  EX_ASSERT(node_->argumentList(), "node_->argumentList is null");
}

std::string IrFunctionSignatureDefinition::toString() const {
  return fmt::format("[@IrFunctionSignatureDefinition node_:{}]",
                     node_->toString());
}

IrType IrFunctionSignatureDefinition::type() const {
  return IrType::FunctionSignatureDefinition;
}

llvm::Value *IrFunctionSignatureDefinition::codeGen() {
  std::string funcIrName = IrUtil::toLLVMName(node_->identifier());

  AstDefinitionList *args = node_->argumentList();
  std::vector<llvm::Type *> doubleArgs(
      args->size(), llvm::Type::getDoubleTy(context_->llvmContext));
  llvm::FunctionType *funcTy = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(context_->llvmContext), doubleArgs, false);
  llvm::Function *func =
      llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage,
                             funcIrName, context_->llvmModule);
  // set function arg names
  int i = 0;
  for (auto &a : func->args()) {
    EX_ASSERT(args, "args is null");
    EX_ASSERT(args->get(i), "args->get({}) is null", i);
    AstFunctionArgumentDefinition *ast =
        DC(AstFunctionArgumentDefinition, args->get(i++));
    std::string argIrName = IrUtil::toLLVMName(ast->identifier());
    a.setName(argIrName);
  }
  ScopeNode *funcNode =
      context_->symbolTable->current->resolve(node_->identifier());
  EX_ASSERT(!funcNode->value, "funcNode->value {} must be null",
            (void *)funcNode->value);
  llvm::Value *ret = llvm::dyn_cast<llvm::Value>(func);
  funcNode->value = ret;
  return ret;
}
