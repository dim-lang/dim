// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrBuilder.h"
#include "Ast.h"
#include "Cowstr.h"
#include "Label.h"
#include "Log.h"
#include "Symbol.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Ir/Type.h"
#include "llvm/Support/Casting.h"
#include <unordered_map>
#include <utility>

namespace detail {

namespace ir_builder {

struct Context : public VisitorContext {
  Context()
      : llvmContext(), llvmIRBuilder(llvmContext), llvmModule(nullptr),
        llvmValue(nullptr), scope(nullptr) {}

  virtual ~Context() {
    if (llvmIRBuilder) {
      delete llvmIRBuilder;
      llvmIRBuilder = nullptr;
    }
    if (llvmModule) {
      delete llvmModule;
      llvmModule = nullptr;
    }
  }

  llvm::LLVMContext llvmContext;
  llvm::IRBuilder<> llvmIRBuilder;
  llvm::Module *llvmModule;
  llvm::Value *llvmValue;

  Scope *scope;
};

struct Integer : public Visitor {
  Integer() : Visitor("IrBuilder::Visitor::Integer") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_Integer *node = static_cast<A_Integer *>(ast);
    switch (node->bitKind()) {
    case A_Integer::BitKind::SIGNED: {
      llvm::APInt signedAP = llvm::APInt(32, node->asInt32(), true);
      llvm::ConstantInt *signedInt =
          llvm::ConstantInt::get(ctx->llvmContext, signedAP);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(signedInt);
      break;
    }
    case A_Integer::BitKind::UNSIGNED: {
      llvm::APInt unsignedAP = llvm::APInt(32, node->asUInt32(), false);
      llvm::ConstantInt *unsignedInt =
          llvm::ConstantInt::get(ctx->llvmContext, unsignedAP);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(unsignedInt);
      break;
    }
    case A_Integer::BitKind::LONG: {
      llvm::APInt longAP = llvm::APInt(64, node->asInt64(), true);
      llvm::ConstantInt *longInt =
          llvm::ConstantInt::get(ctx->llvmContext, longAP);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(longInt);
      break;
    }
    case A_Integer::BitKind::ULONG: {
      ctx->llvmValue = llvm::ConstantInt::get(
          ctx->llvmContext, llvm::APInt(64, node->asUInt64(), false));
      break;
    }
    default:
      LOG_ASSERT(false, "invalid integer node {}:{}", node->name(),
                 node->location());
    }
  }
};

struct Loop : public Visitor {
  Loop() : Visitor("IrBuilder::Visitor::Loop") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_Loop *node = static_cast<A_Loop *>(ast);
    // update scope
    ctx->scope = node->localScope;
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    // update scope
    ctx->scope = ctx->scope->owner();
  }
};

struct Block : public Visitor {
  Block() : Visitor("IrBuilder::Visitor::Block") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_Block *node = static_cast<A_Block *>(ast);
    // update scope
    ctx->scope = node->localScope;
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    // update scope
    ctx->scope = ctx->scope->owner();
  }
};

struct PlainType : public Visitor {
  PlainType() : Visitor("IrBuilder::Visitor::PlainType") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_PlainType *node = static_cast<A_PlainType *>(ast);

    TypeSymbol *tsym = ctx->scope->ts_resolve(node->name());
    LOG_ASSERT(tsym, "type symbol {}:{} cannot resolve in scope {}:{}",
               node->name(), node->location(), ctx->scope->name(),
               ctx->scope->location());
    LOG_ASSERT(tsym->kind()._to_integral() ==
                   (+TypeSymbolKind::Plain)._to_integral(),
               "tsym {}:{} kind {} != TypeSymbolKind::Plain {}", tsym->name(),
               tsym->location(), tsym->kind()._to_string(),
               (+TypeSymbolKind::Plain)._to_string());

    Ts_Plain *ty = static_cast<Ts_Plain *>(tsym);
    if (ty == TypeSymbol::ts_byte() || ty == TypeSymbol::ts_ubyte()) {
      llvm::IntegerType *byteTy = Type::getInt8Ty(ctx->llvmContext);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(byteTy);
    } else if (ty == TypeSymbol::ts_short() || ty == TypeSymbol::ts_ushort()) {
      llvm::IntegerType *shortTy = Type::getInt16Ty(ctx->llvmContext);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(shortTy);
    } else if (ty == TypeSymbol::ts_int() || ty == TypeSymbol::ts_uint()) {
      llvm::IntegerType *intTy = Type::getInt32Ty(ctx->llvmContext);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(intTy);
    } else if (ty == TypeSymbol::ts_long() || ty == TypeSymbol::ts_ulong()) {
      llvm::IntegerType *longTy = Type::getInt64Ty(ctx->llvmContext);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(longTy);
    } else if (ty == TypeSymbol::ts_float()) {
      llvm::Type *floatTy = Type::getFloatTy(ctx->llvmContext);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(floatTy);
    } else if (ty == TypeSymbol::ts_double()) {
      llvm::Type *doubleTy = Type::getDoubleTy(ctx->llvmContext);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(doubleTy);
    } else if (ty == TypeSymbol::ts_char()) {
      llvm::IntegerType *charTy = Type::getInt8Ty(ctx->llvmContext);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(charTy);
    } else if (ty == TypeSymbol::ts_boolean()) {
      llvm::IntegerType *booleanTy = Type::getInt1Ty(ctx->llvmContext);
      ctx->llvmValue = llvm::dyn_cast<llvm::Value>(booleanTy);
    } else if (ty == TypeSymbol::ts_void()) {
      ctx->llvmValue = nullptr;
    } else {
      LOG_ASSERT(false, "invalid plain type {}:{}", ty->name(), ty->location());
    }
  }
};

struct FuncDef : public Visitor {
  FuncDef() : Visitor("IrBuilder::Visitor::FuncDef") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_FuncDef *node = static_cast<A_FuncDef *>(ast);
    A_FuncSign *sign = static_cast<A_FuncSign *>(node->funcSign);
    A_VarId *varId = static_cast<A_VarId *>(sign->id);
    // update scope
    ctx->scope = dynamic_cast<Scope *>(varId->symbol);
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    // update scope
    ctx->scope = ctx->scope->owner();
  }
};

struct VarDef : public Visitor {
  llvm::Value *idValue;
  llvm::Value *typeValue;
  llvm::Value *exprValue;

  VarDef()
      : Visitor("IrBuilder::Visitor::VarDef"), idValue(nullptr),
        typeValue(nullptr), exprValue(nullptr) {}
  virtual void visitAfter(Ast *ast, Ast *child, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_VarDef *node = static_cast<A_VarDef *>(ast);
    A_VarId *varId = static_cast<A_VarId *>(node->id);
    A_PlainType *varType = static_cast<A_PlainType *>(node->type);
    Ast *varInit = node->expr;

    if (static_cast<void *>(varType) == static_cast<void *>(child)) {
      typeValue = ctx->llvmValue;
    } else if (static_cast<void *>(varInit) == static_cast<void *>(child)) {
      exprValue = ctx->llvmValue;
    }
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_VarDef *node = static_cast<A_VarDef *>(ast);

    A_VarId *varId = static_cast<A_VarId *>(node->id);
    A_PlainType *varType = static_cast<A_PlainType *>(node->type);
    Ast *varInit = node->expr;

    llvm::Type *gty = llvm::dyn_cast<llvm::Type>(typeValue);
    llvm::Constant *ginit = llvm::dyn_cast<llvm::Constant>(exprValue);
    llvm::GlobalVariable *gvar = new llvm::GlobalVariable(
        *ctx->llvmModule, gty, false, llvm::GlobalValue::CommonLinkage, ginit,
        Label::globalVariable(node).str(), nullptr,
        llvm::GlobalValue::NotThreadLocal, 0, false);
    ctx->llvmValue = llvm::dyn_cast<llvm::Value>(gvar);
  }
};

struct CompileUnit : public Visitor {
  CompileUnit() : Visitor("IrBuilder::Visitor::CompileUnit") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_CompileUnit *node = static_cast<A_CompileUnit *>(ast);
    // update scope
    LOG_ASSERT(!ctx->scope, "context scope must be null before compile unit:{}",
               ctx->scope->name());
    ctx->scope = node->globalScope;
    // init llvmModule
    ctx->llvmModule = new llvm::Module(node->name().str(), ctx->llvmContext);
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    // update scope
    ctx->scope = ctx->scope->owner();
    LOG_ASSERT(!ctx->scope, "global scope's owner scope must be null:{}",
               ctx->scope->name());
  }
};

} // namespace ir_builder

} // namespace detail

IrBuilder::IrBuilder(const Cowstr &fileName)
    : fileName_(fileName), context_(new detail::ir_builder::Context()),
      binder_(context_) {}

IrBuilder::~IrBuilder() {
  delete context_;
  context_ = nullptr;
  for (int i = 0; i < (int)visitors_.size(); i++) {
    delete visitors_[i];
    visitors_[i] = nullptr;
  }
  visitors_.clear();
}

void IrBuilder::run(Ast *ast) {}

Cowstr &IrBuilder::fileName() { return fileName_; }

const Cowstr &IrBuilder::fileName() const { return fileName_; }
