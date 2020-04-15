// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "Counter.h"
#include "Log.h"
#include "NameGenerator.h"
#include "Parser.tab.hpp"
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
    : context_(), builder_(context_), module_(), symtable_() {}

IrContext::~IrContext() {}

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

/* translate unit */
IrTranslateUnit::IrTranslateUnit(AstTranslateUnit *node)
    : Ir<AstTranslateUnit>(node, nameGen("IrTranslateUnit")) {}

int IrTranslateUnit::type() const { return IR_TRANSLATE_UNIT; }

llvm::Value *IrTranslateUnit::codeGen(IrContext *context) {
  if (!node_->empty()) {
    for (int i = 0; i < node_->size(); i++) {
      Ast *ast = node_->get(i);
      switch (ast->type()) {
      default:
        LOG_INFO("default ast:{}", ast->toString());
      }
    }
  }
}

std::string IrTranslateUnit::stringify() const { return "IrTranslateUnit"; }

/* expression */
IrExpression::IrExpression(AstExpression *node)
    : Ir<AstExpression>(node, nameGen("IrExpression")) {}

int IrExpression::type() const { return IR_EXPRESSION; }

llvm::Value *IrExpression::codeGen(IrContext *context) {
  switch (node_->type()) {
  case IR_IDENTIFIER_CONSTANT:
    return IrIdentifierConstant(DC(AstIdentifierConstant, node_))
        .codeGen(context);
  case IR_F32_CONSTANT:
    return IrF32Constant(DC(AstF32Constant, node_)).codeGen(context);
  case IR_F64_CONSTANT:
    return IrF64Constant(DC(AstF64Constant, node_)).codeGen(context);
  default:
    LOG_ASSERT(false, "node {} invalid", node_->toString());
  }
  return nullptr;
}

std::string IrExpression::stringify() const { return "IrExpression"; }

/* identifier constant */
IrIdentifierConstant::IrIdentifierConstant(AstIdentifierConstant *node)
    : Ir<AstIdentifierConstant>(node, nameGen("IrIdentifierConstant")) {}

int IrIdentifierConstant::type() const { return IR_IDENTIFIER_CONSTANT; }

llvm::Value *IrIdentifierConstant::codeGen(IrContext *context) {
  LOG_ASSERT(context, "context is null");
  LOG_ASSERT(node_, "node_ is null");
  LOG_ASSERT(context->symtable().find(node_->value()) ==
                 context->symtable().end(),
             "symbol {} not found in context", node_->value());
  return context->symtable()[node_->value()];
}

std::string IrIdentifierConstant::stringify() const {
  return "IrIdentifierConstant";
}

/* i8 constant */
IrI8Constant::IrI8Constant(AstI8Constant *node)
    : Ir<AstI8Constant>(node, nameGen("IrI8Constant")) {}

int IrI8Constant::type() const { return IR_I8_CONSTANT; }

llvm::Value *IrI8Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(8, node_->value(), true));
}

std::string IrI8Constant::stringify() const { return "IrI8Constant"; }

/* u8 constant */
IrU8Constant::IrU8Constant(AstU8Constant *node)
    : Ir<AstU8Constant>(node, nameGen("IrU8Constant")) {}

int IrU8Constant::type() const { return IR_U8_CONSTANT; }

llvm::Value *IrU8Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(8, node_->value(), false));
}

std::string IrU8Constant::stringify() const { return "IrU8Constant"; }

/* i16 constant */
IrI16Constant::IrI16Constant(AstI16Constant *node)
    : Ir<AstI16Constant>(node, nameGen("IrI16Constant")) {}

int IrI16Constant::type() const { return IR_I16_CONSTANT; }

llvm::Value *IrI16Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(16, node_->value(), true));
}

std::string IrI16Constant::stringify() const { return "IrI16Constant"; }

/* u16 constant */
IrU16Constant::IrU16Constant(AstU16Constant *node)
    : Ir<AstU16Constant>(node, nameGen("IrU16Constant")) {}

int IrU16Constant::type() const { return IR_U16_CONSTANT; }

llvm::Value *IrU16Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(16, node_->value(), false));
}

std::string IrU16Constant::stringify() const { return "IrU16Constant"; }

/* i32 constant */
IrI32Constant::IrI32Constant(AstI32Constant *node)
    : Ir<AstI32Constant>(node, nameGen("IrI32Constant")) {}

int IrI32Constant::type() const { return IR_I32_CONSTANT; }

llvm::Value *IrI32Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(32, node_->value(), true));
}

std::string IrI32Constant::stringify() const { return "IrI32Constant"; }

/* u32 constant */
IrU32Constant::IrU32Constant(AstU32Constant *node)
    : Ir<AstU32Constant>(node, nameGen("IrU32Constant")) {}

int IrU32Constant::type() const { return IR_U32_CONSTANT; }

llvm::Value *IrU32Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(32, node_->value(), false));
}

std::string IrU32Constant::stringify() const { return "IrU32Constant"; }

/* i64 constant */
IrI64Constant::IrI64Constant(AstI64Constant *node)
    : Ir<AstI64Constant>(node, nameGen("IrI64Constant")) {}

int IrI64Constant::type() const { return IR_I64_CONSTANT; }

llvm::Value *IrI64Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(64, node_->value(), true));
}

std::string IrI64Constant::stringify() const { return "IrI64Constant"; }

/* u64 constant */
IrU64Constant::IrU64Constant(AstU64Constant *node)
    : Ir<AstU64Constant>(node, nameGen("IrU64Constant")) {}

int IrU64Constant::type() const { return IR_U64_CONSTANT; }

llvm::Value *IrU64Constant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(64, node_->value(), false));
}

std::string IrU64Constant::stringify() const { return "IrU64Constant"; }

/* f32 constant */
IrF32Constant::IrF32Constant(AstF32Constant *node)
    : Ir<AstF32Constant>(node, nameGen("IrF32Constant")) {}

int IrF32Constant::type() const { return IR_F32_CONSTANT; }

llvm::Value *IrF32Constant::codeGen(IrContext *context) {
  return llvm::ConstantFP::get(context->context(),
                               llvm::APFloat((float)node_->value()));
}

std::string IrF32Constant::stringify() const { return "IrF32Constant"; }

/* f64 constant */
IrF64Constant::IrF64Constant(AstF64Constant *node)
    : Ir<AstF64Constant>(node, nameGen("IrF64Constant")) {}

int IrF64Constant::type() const { return IR_F64_CONSTANT; }

llvm::Value *IrF64Constant::codeGen(IrContext *context) {
  return llvm::ConstantFP::get(context->context(),
                               llvm::APFloat((double)node_->value()));
}

std::string IrF64Constant::stringify() const { return "IrF64Constant"; }

/* string constant */
IrStringConstant::IrStringConstant(AstStringConstant *node)
    : Ir<AstStringConstant>(node, nameGen("IrStringConstant")) {}

int IrStringConstant::type() const { return IR_STRING_CONSTANT; }

llvm::Value *IrStringConstant::codeGen(IrContext *context) { return nullptr; }

std::string IrStringConstant::stringify() const { return "IrStringConstant"; }

/* boolean constant */
IrBooleanConstant::IrBooleanConstant(AstBooleanConstant *node)
    : Ir<AstBooleanConstant>(node, nameGen("IrBooleanConstant")) {}

int IrBooleanConstant::type() const { return IR_BOOLEAN_CONSTANT; }

llvm::Value *IrBooleanConstant::codeGen(IrContext *context) {
  return llvm::ConstantInt::get(
      context->context(), llvm::APInt(1, node_->value() ? 1U : 0U, false));
}

std::string IrBooleanConstant::stringify() const { return "IrBooleanConstant"; }

/* call expression */
IrCallExpression::IrCallExpression(AstCallExpression *node)
    : Ir<AstCallExpression>(node, nameGen("IrCallExpression")) {}

int IrCallExpression::type() const { return IR_CALL_EXPRESSION; }

llvm::Value *IrCallExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrCallExpression::stringify() const { return "IrCallExpression"; }

/* unary expression */
IrUnaryExpression::IrUnaryExpression(AstUnaryExpression *node)
    : Ir<AstUnaryExpression>(node, nameGen("IrUnaryExpression")) {}

int IrUnaryExpression::type() const { return IR_UNARY_EXPRESSION; }

llvm::Value *IrUnaryExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrUnaryExpression::stringify() const { return "IrUnaryExpression"; }

/* binary expression */
IrBinaryExpression::IrBinaryExpression(AstBinaryExpression *node,
                                       IrExpression *left, IrExpression *right)
    : Ir<AstBinaryExpression>(node, nameGen("IrBinaryExpression")), left_(left),
      right_(right) {}

IrBinaryExpression::~IrBinaryExpression() {
  delete left_;
  left_ = nullptr;
  delete right_;
  right_ = nullptr;
}

int IrBinaryExpression::type() const { return IR_BINARY_EXPRESSION; }

static llvm::Value *binOp(llvm::Value *l, llvm::Value *r, int token) {
  switch (token) {
  case T_ADD:
  case T_SUB:
  default:
    LOG_ASSERT(false, "token {} invalid", token);
  }
  return nullptr;
}

#define IR_FADD(x, y) context->builder().CreateFAdd(x, y, "addtmp")
#define IR_ADD(x, y) context->builder().CreateAdd(x, y, "addtmp")
#define IR_FSUB(x, y) context->builder().CreateFSub(x, y, "subtmp")
#define IR_SUB(x, y) context->builder().CreateSub(x, y, "subtmp")
#define IR_FMUL(x, y) context->builder().CreateFMul(x, y, "multmp")
#define IR_MUL(x, y) context->builder().CreateMul(x, y, "multmp")
#define IR_FDIV(x, y) context->builder().CreateFMul(x, y, "divtmp")
#define IR_DIV(x, y) context->builder().CreateMul(x, y, "divtmp")
#define ADDTMP "addtmp"
#define SUBTMP "subtmp"
#define MULTMP "multmp"
#define DIVTMP "divtmp"

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
      return context->builder().CreateFAdd(l, r, ADDTMP);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFAdd(l, r, ADDTMP);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateAdd(l, r, ADDTMP);
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
      return context->builder().CreateFSub(l, r, SUBTMP);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFSub(l, r, SUBTMP);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateSub(l, r, SUBTMP);
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
      return context->builder().CreateFMul(l, r, MULTMP);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFMul(l, r, MULTMP);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateMul(l, r, MULTMP);
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
      return context->builder().CreateFDiv(l, r, DIVTMP);
    } break;
    case llvm::Type::TypeID::IntegerTyID: {
      switch (r->getType()->getTypeID()) {
      case llvm::Type::TypeID::FloatTyID:
      case llvm::Type::TypeID::DoubleTyID: {
        return context->builder().CreateFDiv(l, r, DIVTMP);
      } break;
      case llvm::Type::TypeID::IntegerTyID: {
        return context->builder().CreateSDiv(l, r, DIVTMP);
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
        llvm::Value *dr = context->builder().CreateSDiv(l, r, DIVTMP);
        // second do sub
        return context->builder().CreateSub(l, dr, SUBTMP);
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
    return nullptr;
  case T_BIT_RSHIFT:
    return nullptr;
  case T_BIT_ARSHIFT:
    return nullptr;
  case T_EQ:
    return nullptr;
  case T_NEQ:
    return nullptr;
  case T_LE:
    return nullptr;
  case T_LT:
    return nullptr;
  case T_GE:
    return nullptr;
  case T_GT:
    return nullptr;
  case T_BIT_AND:
    return nullptr;
  case T_BIT_OR:
    return nullptr;
  case T_BIT_XOR:
    return nullptr;
  case T_LOGIC_AND:
    return nullptr;
  case T_LOGIC_OR:
    return nullptr;
  default:
    LOG_ASSERT(false, "token {} invalid", node_->token());
  }
  return nullptr;
}

std::string IrBinaryExpression::stringify() const {
  return "IrBinaryExpression";
}

/* conditional expression */
IrConditionalExpression::IrConditionalExpression(AstConditionalExpression *node)
    : Ir<AstConditionalExpression>(node, nameGen("IrConditionalExpression")) {}

int IrConditionalExpression::type() const { return IR_CONDITIONAL_EXPRESSION; }

llvm::Value *IrConditionalExpression::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrConditionalExpression::stringify() const {
  return "IrConditionalExpression";
}

/* assignment expression */
IrAssignmentExpression::IrAssignmentExpression(AstAssignmentExpression *node)
    : Ir<AstAssignmentExpression>(node, nameGen("IrAssignmentExpression")) {}

int IrAssignmentExpression::type() const { return IR_ASSIGNMENT_EXPRESSION; }

llvm::Value *IrAssignmentExpression::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrAssignmentExpression::stringify() const {
  return "IrAssignmentExpression";
}

/* sequel expression */
IrSequelExpression::IrSequelExpression(AstSequelExpression *node)
    : Ir<AstSequelExpression>(node, nameGen("IrSequelExpression")) {}

int IrSequelExpression::type() const { return IR_SEQUEL_EXPERSSION; }

llvm::Value *IrSequelExpression::codeGen(IrContext *context) { return nullptr; }

std::string IrSequelExpression::stringify() const {
  return "IrSequelExpression";
}

/* expression statement */
IrExpressionStatement::IrExpressionStatement(AstExpressionStatement *node)
    : Ir<AstExpressionStatement>(node, nameGen("IrExpressionStatement")) {}

int IrExpressionStatement::type() const { return IR_EXPRESSION_STATEMENT; }

llvm::Value *IrExpressionStatement::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrExpressionStatement::stringify() const {
  return "IrExpressionStatement";
}

/* compound statement */
IrCompoundStatement::IrCompoundStatement(AstCompoundStatement *node)
    : Ir<AstCompoundStatement>(node, nameGen("IrCompoundStatement")) {}

int IrCompoundStatement::type() const { return IR_COMPOUND_STATEMENT; }

llvm::Value *IrCompoundStatement::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrCompoundStatement::stringify() const {
  return "IrCompoundStatement";
}

/* if statement */
IrIfStatement::IrIfStatement(AstIfStatement *node)
    : Ir<AstIfStatement>(node, nameGen("IrIfStatement")) {}

int IrIfStatement::type() const { return IR_IF_STATEMENT; }

llvm::Value *IrIfStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrIfStatement::stringify() const { return "IrIfStatement"; }

/* while statement */
IrWhileStatement::IrWhileStatement(AstWhileStatement *node)
    : Ir<AstWhileStatement>(node, nameGen("IrWhileStatement")) {}

int IrWhileStatement::type() const { return IR_WHILE_STATEMENT; }

llvm::Value *IrWhileStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrWhileStatement::stringify() const { return "IrWhileStatement"; }

/* for statement */
IrForStatement::IrForStatement(AstForStatement *node)
    : Ir<AstForStatement>(node, nameGen("IrForStatement")) {}

int IrForStatement::type() const { return IR_FOR_STATEMENT; }

llvm::Value *IrForStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrForStatement::stringify() const { return "IrForStatement"; }

/* continue statement */
IrContinueStatement::IrContinueStatement(AstContinueStatement *node)
    : Ir<AstContinueStatement>(node, nameGen("IrContinueStatement")) {}

int IrContinueStatement::type() const { return IR_CONTINUE_STATEMENT; }

llvm::Value *IrContinueStatement::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrContinueStatement::stringify() const {
  return "IrContinueStatement";
}

/* break statement */
IrBreakStatement::IrBreakStatement(AstBreakStatement *node)
    : Ir<AstBreakStatement>(node, nameGen("IrBreakStatement")) {}

int IrBreakStatement::type() const { return IR_BREAK_STATEMENT; }

llvm::Value *IrBreakStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrBreakStatement::stringify() const { return "IrBreakStatement"; }

/* return statement */
IrReturnStatement::IrReturnStatement(AstReturnStatement *node)
    : Ir<AstReturnStatement>(node, nameGen("IrReturnStatement")) {}

int IrReturnStatement::type() const { return IR_RETURN_STATEMENT; }

llvm::Value *IrReturnStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrReturnStatement::stringify() const { return "IrReturnStatement"; }

/* empty statement */
IrEmptyStatement::IrEmptyStatement(AstEmptyStatement *node)
    : Ir<AstEmptyStatement>(node, nameGen("IrEmptyStatement")) {}

int IrEmptyStatement::type() const { return IR_EMPTY_STATEMENT; }

llvm::Value *IrEmptyStatement::codeGen(IrContext *context) { return nullptr; }

std::string IrEmptyStatement::stringify() const { return "IrEmptyStatement"; }

/* variable declaration */
IrVariableDeclaration::IrVariableDeclaration(AstVariableDeclaration *node)
    : Ir<AstVariableDeclaration>(node, nameGen("IrVariableDeclaration")) {}

int IrVariableDeclaration::type() const { return IR_VARIABLE_DECLARATION; }

llvm::Value *IrVariableDeclaration::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrVariableDeclaration::stringify() const {
  return "IrVariableDeclaration";
}

/* variable assignment declaration */
IrVariableAssignmentDeclaration::IrVariableAssignmentDeclaration(
    AstVariableAssignmentDeclaration *node)
    : Ir<AstVariableAssignmentDeclaration>(
          node, nameGen("IrVariableAssignmentDeclaration")) {}

int IrVariableAssignmentDeclaration::type() const {
  return IR_VARIABLE_ASSIGNMENT_DECLARATION;
}

llvm::Value *IrVariableAssignmentDeclaration::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrVariableAssignmentDeclaration::stringify() const {
  return "IrVariableAssignmentDeclaration";
}

/* function declaration */
IrFunctionDeclaration::IrFunctionDeclaration(AstFunctionDeclaration *node)
    : Ir<AstFunctionDeclaration>(node, nameGen("IrFunctionDeclaration")) {}

int IrFunctionDeclaration::type() const { return IR_FUNCTION_DECLARATION; }

llvm::Value *IrFunctionDeclaration::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrFunctionDeclaration::stringify() const {
  return "IrFunctionDeclaration";
}

/* function argument declaration */
IrFunctionArgumentDeclaration::IrFunctionArgumentDeclaration(
    AstFunctionArgumentDeclaration *node)
    : Ir<AstFunctionArgumentDeclaration>(
          node, nameGen("IrFunctionArgumentDeclaration")) {}

int IrFunctionArgumentDeclaration::type() const {
  return IR_FUNCTION_ARGUMENT_DECLARATION;
}

llvm::Value *IrFunctionArgumentDeclaration::codeGen(IrContext *context) {
  return nullptr;
}

std::string IrFunctionArgumentDeclaration::stringify() const {
  return "IrFunctionArgumentDeclaration";
}

#undef DC
