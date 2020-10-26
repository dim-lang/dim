// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrBuilder.h"
#include "Ast.h"
#include "Log.h"
#include "Symbol.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

namespace detail {

namespace ir_builder {

struct Context : public VisitorContext {
  Context()
      : llvmContext(nullptr), llvmModule(nullptr), llvmIRBuilder(nullptr),
        llvmValue(nullptr) {}

  virtual ~Context() {
    if (llvmIRBuilder) {
      delete llvmIRBuilder;
      llvmIRBuilder = nullptr;
    }
    if (llvmModule) {
      delete llvmModule;
      llvmModule = nullptr;
    }
    if (llvmContext) {
      delete llvmContext;
      llvmContext = nullptr;
    }
  }

  llvm::LLVMContext *llvmContext;
  llvm::Module *llvmModule;
  llvm::IRBuilder<> *llvmIRBuilder;
  llvm::Value *llvmValue;
};

struct Integer : public Visitor {
  Integer() : Visitor("IrBuilder::Integer::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_Integer *node = static_cast<A_Integer>(ast);
    switch (node->bitKind()) {
    case A_Integer::BitKind::SIGNED: {
      ctx->value = llvm::ConstantInt::get(
          ctx->llvmContext, llvm::APInt(32, node->asInt32(), true));
      break;
    }
    case A_Integer::BitKind::UNSIGNED: {
      ctx->value = llvm::ConstantInt::get(
          ctx->llvmContext, llvm::APInt(32, node->asUInt32(), false));
      break;
    }
    case A_Integer::BitKind::LONG: {
      ctx->value = llvm::ConstantInt::get(
          ctx->llvmContext, llvm::APInt(64, node->asInt64(), true));
      break;
    }
    case A_Integer::BitKind::ULONG: {
      ctx->value = llvm::ConstantInt::get(
          ctx->llvmContext, llvm::APInt(64, node->asUInt64(), false));
      break;
    }
    default:
      LOG_ASSERT(false, "invalid BitKind for node {}:{}", node->name(),
                 node->location().str());
    }
  }
  virtual void postVisit(Ast *ast, VisitorContext *context) {}
};

struct CompileUnit : public Visitor {
  CompileUnit() : Visitor("IrBuilder::CompileUnit::Visitor") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    detail::ir_builder::Context *ctx =
        static_cast<detail::ir_builder::Context *>(context_);
    A_CompileUnit *node = static_cast<A_CompileUnit *>(ast);

    ctx->llvmContext = new llvm::LLVMContext();
    ctx->llvmModule = new llvm::Module(node->name().str(), *ctx->llvmContext);
    ctx->llvmIRBuilder = new llvm::IRBuilder<>(*ctx->llvmContext);
  }
};

} // namespace ir_builder

} // namespace detail

IrBuilder::IrBuilder()
    : context_(new detail::ir_builder::Context()), binder_(context_) {}

IrBuilder::~IrBuilder() {
  delete context_;
  context_ = nullptr;
}

void IrBuilder::run(Ast *ast) {}
