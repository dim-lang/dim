// Copyright 2019- <coli-lang>
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

const td::map<std::string, llvm::Value *> &IrContext::symtable() const {
  return symtable_;
}

/* translate unit */
IrTranslateUnit::IrTranslateUnit(AstTranslateUnit *node)
    : Ir<AstTranslateUnit *>(node), name_(GEN_NAME("IrTranslateUnit")) {}

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

/* identifier constant */
IrIdentifierConstant::IrIdentifierConstant(AstIdentifierConstant *node)
    : Ir<AstIdentifierConstant *>(node),
      name_(GEN_NAME("IrIdentifierConstant")) {}

int IrIdentifierConstant::type() const { return A_IDENTIFIER_CONSTANT; }

llvm::Value *IrIdentifierConstant::codegen(IrContext *context) {}

/* i8 constant */
IrI8Constant::IrI8Constant(AstI8Constant *node)
    : Ir<AstI8Constant *>(node), name_(GEN_NAME("IrI8Constant")) {}

int IrI8Constant::type() const { return A_I8_CONSTANT; }

llvm::Value *IrI8Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(8, node_->value(), true));
}

/* u8 constant */
IrU8Constant::IrU8Constant(AstU8Constant *node)
    : Ir<AstU8Constant *>(node), name_(GEN_NAME("IrU8Constant")) {}

int IrU8Constant::type() const { return A_U8_CONSTANT; }

llvm::Value *IrU8Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(8, node_->value(), false));
}

/* i16 constant */
IrI16Constant::IrI16Constant(AstI16Constant *node)
    : Ir<IrI16Constant *>(node), name_(GEN_NAME("IrI16Constant")) {}

int IrI16Constant::type() const { return A_I16_CONSTANT; }

llvm::Value *IrI16Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(16, node_->value(), true));
}

/* u16 constant */
IrU16Constant::IrU16Constant(AstU16Constant *node)
    : Ir<AstU16Constant *>(node), name_(GEN_NAME("IrU16Constant")) {}

int IrU16Constant::type() const { return A_U16_CONSTANT; }

llvm::Value *IrU16Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(16, node_->value(), false));
}

/* i32 constant */
IrI32Constant::IrI32Constant(AstI32Constant *node)
    : Ir<AstI32Constant *>(node), name_(GEN_NAME("IrI32Constant")) {}

int IrI32Constant::type() const { return A_I32_CONSTANT; }

llvm::Value *IrI32Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(32, node_->value(), true));
}

/* u32 constant */
IrU32Constant::IrU32Constant(AstU32Constant *node)
    : Ir<AstU32Constant *>(node), name_(GEN_NAME("IrU32Constant")) {}

int IrU32Constant::type() const { return A_U32_CONSTANT; }

llvm::Value *IrU32Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(32, node_->value(), false));
}

/* i64 constant */
IrI64Constant::IrI64Constant(AstI64Constant *node)
    : Ir<AstI64Constant *>(node), name_(GEN_NAME("IrI64Constant")) {}

int IrI64Constant::type() const { return A_I64_CONSTANT; }

llvm::Value *IrI64Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(64, node_->value(), true));
}

/* u64 constant */
IrU64Constant::IrU64Constant(AstU64Constant *node)
    : Ir<AstU64Constant *>(node), name_(GEN_NAME("IrU64Constant")) {}

int IrU64Constant::type() const { return A_U64_CONSTANT; }

llvm::Value *IrU64Constant::codegen(IrContext *context) {
  return llvm::ConstantInt::get(context->context(),
                                llvm::APInt(64, node_->value(), false));
}

/* f32 constant */
IrF32Constant::IrF32Constant(AstF32Constant *node)
    : Ir<AstF32Constant *>(node), name_(GEN_NAME("IrF32Constant")) {}

int IrF32Constant::type() const { return A_F32_CONSTANT; }

llvm::Value *IrF32Constant::codegen(IrContext *context) {
  return llvm::ConstantFP::get(context->context(),
                               llvm::APFloat((float)node_->value()));
}

/* f64 constant */
IrF64Constant::IrF64Constant(AstF64Constant *node)
    : Ir<AstF64Constant *>(node), name_(GEN_NAME("IrF64Constant")) {}

int IrF64Constant::type() const { return A_F64_CONSTANT; }

llvm::Value *IrF64Constant::codegen(IrContext *context) {
  return llvm::ConstantFP::get(context->context(),
                               llvm::APFloat((double)node_->value()));
}

#undef GEN_NAME
