// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "Counter.h"
#include "fmt/format.h"

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
IrTranslateUnit::IrTranslateUnit(AstTranslateUnit *node) : node_(node) {}

IrTranslateUnit::~IrTranslateUnit() { node_ = nullptr; }

int IrTranslateUnit::type() const { return A_TRANSLATE_UNIT; }

std::string IrTranslateUnit::toString() const {
  return fmt::format("[ @IrTranslateUnit node_:{} ]", node_->name());
}

std::string IrTranslateUnit::name() const {
  return GEN_NAME("IrTranslateUnit");
}

llvm::Value *IrTranslateUnit::codegen(IrContext *context) {
  if (!node_->empty()) {
    for (int i = 0; i < node_->size(); i++) {
      Ast *ast = node_->get(i);
      switch (ast->type()) {}
    }
  }
}

#undef GEN_NAME
