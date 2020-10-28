// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrBuilder.h"
#include "Ast.h"
#include "Label.h"
#include "Symbol.h"
#include "iface/LLVMModular.h"
#include "iface/LLVMTypable.h"
#include "iface/LLVMValuable.h"
#include "iface/Scoped.h"
#include "infra/Log.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include <system_error>
#include <unordered_map>
#include <utility>

namespace detail {

namespace ir_builder {

struct Context : public VisitorContext,
                 public LLVMValuable,
                 public LLVMTypable,
                 public LLVMModular,
                 public Scoped {
  Context() : llvmContext(), llvmIRBuilder(llvmContext) {}

  llvm::LLVMContext llvmContext;
  llvm::IRBuilder<> llvmIRBuilder;
};

struct Integer : public Visitor {
  Integer() : Visitor("IrBuilder::Visitor::Integer") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_Integer *node = static_cast<A_Integer *>(ast);
    switch (node->bit()) {
    case 32: {
      llvm::APInt ap = node->isSigned() ? llvm::APInt(32, node->asInt32(), true)
                                        : llvm::APInt(node->asUInt32(), false);
      llvm::ConstantInt *ci = llvm::ConstantInt::get(ctx->llvmContext, ap);
      node->llvmValue() = ci;
      ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(ci);
      break;
    }
    case 64: {
      llvm::APInt ap = node->isSigned()
                           ? llvm::APInt(64, node->asInt64(), true)
                           : llvm::APInt(64, node->asUInt64(), false);
      llvm::ConstantInt *ci = llvm::ConstantInt::get(ctx->llvmContext, ap);
      node->llvmValue() = ci;
      ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(ci);
      break;
    }
    default:
      LOG_ASSERT(false, "invalid integer node {}:{}", node->name(),
                 node->location());
    }
  }
};

struct Float : public Visitor {
  Float() : Visitor("IrBuilder::Visitor::Float") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_Float *node = static_cast<A_Float *>(ast);
    switch (node->bit()) {
    case 32: {
      llvm::APFloat ap = llvm::APFloat(node->asFloat());
      llvm::ConstantFP *cf = llvm::ConstantFP::get(ctx->llvmContext, ap);
      node->llvmValue() = cf;
      ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(cf);
      break;
    }
    case 64: {
      llvm::APFloat ap = llvm::APFloat(node->asDouble());
      llvm::ConstantFP *cf = llvm::ConstantFP::get(ctx->llvmContext, ap);
      node->llvmValue() = cf;
      ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(cf);
      break;
    }
    default:
      LOG_ASSERT(false, "invalid float node {}:{}", node->name(),
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
    ctx->scope() = node->scope();
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    // update scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct Block : public Visitor {
  Block() : Visitor("IrBuilder::Visitor::Block") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_Block *node = static_cast<A_Block *>(ast);
    // update scope
    ctx->scope() = node->scope();
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    // update scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct PlainType : public Visitor {
  PlainType() : Visitor("IrBuilder::Visitor::PlainType") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_PlainType *node = static_cast<A_PlainType *>(ast);

    TypeSymbol *tsym = ctx->scope()->ts_resolve(node->name());
    LOG_ASSERT(tsym, "type symbol {}:{} cannot resolve in scope {}:{}",
               node->name(), node->location(), ctx->scope()->name(),
               ctx->scope()->location());
    LOG_ASSERT(tsym->kind()._to_integral() ==
                   (+TypeSymbolKind::Plain)._to_integral(),
               "tsym {}:{} kind {} != TypeSymbolKind::Plain {}", tsym->name(),
               tsym->location(), tsym->kind()._to_string(),
               (+TypeSymbolKind::Plain)._to_string());

    Ts_Plain *tp = static_cast<Ts_Plain *>(tsym);
    if (tp == TypeSymbol::ts_byte() || tp == TypeSymbol::ts_ubyte()) {
      llvm::IntegerType *it = llvm::Type::getInt8Ty(ctx->llvmContext);
      node->llvmType() = it;
      ctx->llvmType() = llvm::dyn_cast<llvm::Type>(it);
    } else if (tp == TypeSymbol::ts_short() || tp == TypeSymbol::ts_ushort()) {
      llvm::IntegerType *it = llvm::Type::getInt16Ty(ctx->llvmContext);
      node->llvmType() = it;
      ctx->llvmType() = llvm::dyn_cast<llvm::Type>(it);
    } else if (tp == TypeSymbol::ts_int() || tp == TypeSymbol::ts_uint()) {
      llvm::IntegerType *it = llvm::Type::getInt32Ty(ctx->llvmContext);
      node->llvmType() = it;
      ctx->llvmType() = llvm::dyn_cast<llvm::Type>(it);
    } else if (tp == TypeSymbol::ts_long() || tp == TypeSymbol::ts_ulong()) {
      llvm::IntegerType *it = llvm::Type::getInt64Ty(ctx->llvmContext);
      node->llvmType() = it;
      ctx->llvmType() = llvm::dyn_cast<llvm::Type>(it);
    } else if (tp == TypeSymbol::ts_float()) {
      llvm::Type *ty = llvm::Type::getFloatTy(ctx->llvmContext);
      node->llvmType() = ty;
      ctx->llvmType() = llvm::dyn_cast<llvm::Type>(ty);
    } else if (tp == TypeSymbol::ts_double()) {
      llvm::Type *ty = llvm::Type::getDoubleTy(ctx->llvmContext);
      node->llvmType() = ty;
      ctx->llvmType() = llvm::dyn_cast<llvm::Type>(ty);
    } else if (tp == TypeSymbol::ts_char()) {
      llvm::IntegerType *it = llvm::Type::getInt8Ty(ctx->llvmContext);
      node->llvmType() = it;
      ctx->llvmType() = llvm::dyn_cast<llvm::Type>(it);
    } else if (tp == TypeSymbol::ts_boolean()) {
      llvm::IntegerType *it = llvm::Type::getInt1Ty(ctx->llvmContext);
      node->llvmType() = it;
      ctx->llvmType() = llvm::dyn_cast<llvm::Type>(it);
    } else if (tp == TypeSymbol::ts_void()) {
      llvm::Type *ty = llvm::Type::getVoidTy(ctx->llvmContext);
      node->llvmType() = ty;
      ctx->llvmType() = llvm::dyn_cast<llvm::Type>(ty);
    } else {
      LOG_ASSERT(false, "invalid plain type {}:{}", tp->name(), tp->location());
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
    ctx->scope() = dynamic_cast<Scope *>(varId->symbol());
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    // update scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct VarDef : public Visitor {
  llvm::Value *idValue;
  llvm::Type *typeValue;
  llvm::Value *exprValue;

  VarDef()
      : Visitor("IrBuilder::Visitor::VarDef"), idValue(nullptr),
        typeValue(nullptr), exprValue(nullptr) {}
  virtual void visitAfter(Ast *ast, Ast *child, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_VarDef *node = static_cast<A_VarDef *>(ast);

    if (static_cast<void *>(node->type) == static_cast<void *>(child)) {
      typeValue = ctx->llvmType();
    } else if (static_cast<void *>(node->expr) == static_cast<void *>(child)) {
      exprValue = ctx->llvmValue();
    }
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_VarDef *node = static_cast<A_VarDef *>(ast);

    llvm::Type *gty = typeValue;
    llvm::Constant *ginit = llvm::dyn_cast<llvm::Constant>(exprValue);
    llvm::GlobalVariable *gvar = new llvm::GlobalVariable(
        *ctx->llvmModule(), gty, false, llvm::GlobalValue::CommonLinkage, ginit,
        Label::globalVariable(node).str(), nullptr,
        llvm::GlobalValue::NotThreadLocal, 0, false);
    ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(gvar);
  }
};

struct CompileUnit : public Visitor {
  CompileUnit() : Visitor("IrBuilder::Visitor::CompileUnit") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    A_CompileUnit *node = static_cast<A_CompileUnit *>(ast);
    // update scope
    LOG_ASSERT(!ctx->scope(),
               "context scope must be null before compile unit:{}",
               ctx->scope()->name());
    ctx->scope() = node->scope();
    // init llvmModule
    llvm::Module *m = new llvm::Module(node->name().str(), ctx->llvmContext);
    node->llvmModule() = m;
    ctx->llvmModule() = m;
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    Context *ctx = static_cast<Context *>(context);
    // update scope
    ctx->scope() = ctx->scope()->owner();
    LOG_ASSERT(!ctx->scope(), "global scope's owner scope must be null:{}",
               ctx->scope()->name());
  }
};

} // namespace ir_builder

} // namespace detail

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new detail::ir_builder::x();                                  \
    binder_.bind((+AstKind::x)._to_integral(), v);                             \
    visitors_.push_back(v);                                                    \
  } while (0)

IrBuilder::IrBuilder(const Cowstr &fileName)
    : Phase("IrBuilder"), fileName_(fileName),
      context_(new detail::ir_builder::Context()), binder_(context_) {
  BIND(Integer);
  BIND(Float);
  BIND(Block);
  BIND(CompileUnit);
  BIND(FuncDef);
  BIND(Loop);
  BIND(PlainType);
  BIND(VarDef);
}

IrBuilder::~IrBuilder() {
  delete context_;
  context_ = nullptr;
  for (int i = 0; i < (int)visitors_.size(); i++) {
    delete visitors_[i];
    visitors_[i] = nullptr;
  }
  visitors_.clear();
}

void IrBuilder::run(Ast *ast) {
  Visitor::traverse(&binder_, ast);
  detail::ir_builder::Context *ctx =
      static_cast<detail::ir_builder::Context *>(context_);

  std::error_code ec;
  llvm::raw_fd_ostream fos(fileName_.str(), ec);
  LOG_ASSERT(!ec, "Error! failed to open file {}", fileName_);
  ctx->llvmModule()->print(fos, nullptr);
  fos.flush();
  fos.close();
}

Cowstr &IrBuilder::fileName() { return fileName_; }

const Cowstr &IrBuilder::fileName() const { return fileName_; }
