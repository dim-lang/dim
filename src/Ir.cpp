// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "Counter.h"
#include "Log.h"

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
    : node_(node), name_(GEN_NAME("IrTranslateUnit")) {}

IrTranslateUnit::~IrTranslateUnit() { node_ = nullptr; }

int IrTranslateUnit::type() const { return A_TRANSLATE_UNIT; }

std::string IrTranslateUnit::toString() const {
  return fmt::format("[ @IrTranslateUnit node_:{} ]", node_->name());
}

std::string IrTranslateUnit::name() const { return name_; }

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
    : node_(node), name_(GEN_NAME("IrIdentifierConstant")) {}

IrIdentifierConstant::~IrIdentifierConstant() { node_ = nullptr; }

int IrIdentifierConstant::type() const { return A_IDENTIFIER_CONSTANT; }

std::string IrIdentifierConstant::toString() const {
  return fmt::format("[ @IrIdentifierConstant node_:{} ]", node_->name());
}

std::string IrIdentifierConstant::name() const { return name_; }

llvm::Value *IrIdentifierConstant::codegen(IrContext *context) {}

/* i8 constant */
IrI8Constant::IrI8Constant(AstI8Constant *node)
    : node_(node), name_(GEN_NAME("IrI8Constant")) {}

IrI8Constant::~IrI8Constant() { node_ = nullptr; }

int IrI8Constant::type() const { return A_I8_CONSTANT; }

std::string IrI8Constant::toString() const {
  return fmt::format("[ @IrI8Constant node_:{} ]", node_->name());
}

std::string IrI8Constant::name() const { return name_; }

llvm::Value *IrI8Constant::codegen(IrContext *context) {}

#undef GEN_NAME
