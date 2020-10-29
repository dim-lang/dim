// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrBuilder.h"
#include "Ast.h"
#include "Label.h"
#include "Symbol.h"
#include "Token.h"
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
#include <utility>

namespace detail {

namespace ir_builder {

struct Context : public VisitorContext,
                 public LLVMValuable,
                 public LLVMTypable,
                 public LLVMModular,
                 public Scoped {
  Context(IrBuilder *a_irBuilder) : irBuilder(a_irBuilder) {}

  IrBuilder *irBuilder;
};

#define INITIALIZE(astype)                                                     \
  Context *ctx = static_cast<Context *>(context);                              \
  IrBuilder *ib = ctx->irBuilder;                                              \
  astype *node = static_cast<astype *>(ast);                                   \
  (void)ctx;                                                                   \
  (void)ib;                                                                    \
  (void)node;

struct Integer : public Visitor {
  Integer() : Visitor("IrBuilder::Visitor::Integer") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    INITIALIZE(A_Integer)
    switch (node->bit()) {
    case 32: {
      llvm::APInt ap = node->isSigned() ? llvm::APInt(32, node->asInt32(), true)
                                        : llvm::APInt(node->asUInt32(), false);
      llvm::ConstantInt *ci = llvm::ConstantInt::get(ib->llvmContext(), ap);
      node->llvmValue() = ci;
      ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(ci);
      break;
    }
    case 64: {
      llvm::APInt ap = node->isSigned()
                           ? llvm::APInt(64, node->asInt64(), true)
                           : llvm::APInt(64, node->asUInt64(), false);
      llvm::ConstantInt *ci = llvm::ConstantInt::get(ib->llvmContext(), ap);
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
    INITIALIZE(A_Float)
    switch (node->bit()) {
    case 32: {
      llvm::APFloat ap = llvm::APFloat(node->asFloat());
      llvm::ConstantFP *cf = llvm::ConstantFP::get(ib->llvmContext(), ap);
      node->llvmValue() = cf;
      ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(cf);
      break;
    }
    case 64: {
      llvm::APFloat ap = llvm::APFloat(node->asDouble());
      llvm::ConstantFP *cf = llvm::ConstantFP::get(ib->llvmContext(), ap);
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
    INITIALIZE(A_Loop)
    // update scope
    ctx->scope() = node->scope();
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    INITIALIZE(A_Loop)
    // update scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct Block : public Visitor {
  Block() : Visitor("IrBuilder::Visitor::Block") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    INITIALIZE(A_Block)
    // update scope
    ctx->scope() = node->scope();
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    INITIALIZE(A_Block)
    // update scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct PlainType : public Visitor {
  PlainType() : Visitor("IrBuilder::Visitor::PlainType") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    INITIALIZE(A_PlainType)

    TypeSymbol *tsym = ctx->scope()->ts_resolve(tokenName(node->token));
    LOG_ASSERT(tsym, "type symbol {}:{} cannot resolve in scope {}:{}",
               tokenName(node->token), node->location(), ctx->scope()->name(),
               ctx->scope()->location());
    LOG_ASSERT(
        tsym->kind()._to_integral() == (+TypeSymbolKind::Plain)._to_integral(),
        "tsym {}:{} kind {} != TypeSymbolKind::Plain {}",
        tokenName(node->token), tsym->location(), tsym->kind()._to_string(),
        (+TypeSymbolKind::Plain)._to_string());

    Ts_Plain *tp = static_cast<Ts_Plain *>(tsym);
    llvm::Type *ty = nullptr;
    if (tp == TypeSymbol::ts_byte() || tp == TypeSymbol::ts_ubyte()) {
      ty = llvm::Type::getInt8Ty(ib->llvmContext());
    } else if (tp == TypeSymbol::ts_short() || tp == TypeSymbol::ts_ushort()) {
      ty = llvm::Type::getInt16Ty(ib->llvmContext());
    } else if (tp == TypeSymbol::ts_int() || tp == TypeSymbol::ts_uint()) {
      ty = llvm::Type::getInt32Ty(ib->llvmContext());
    } else if (tp == TypeSymbol::ts_long() || tp == TypeSymbol::ts_ulong()) {
      ty = llvm::Type::getInt64Ty(ib->llvmContext());
    } else if (tp == TypeSymbol::ts_float()) {
      ty = llvm::Type::getFloatTy(ib->llvmContext());
    } else if (tp == TypeSymbol::ts_double()) {
      ty = llvm::Type::getDoubleTy(ib->llvmContext());
    } else if (tp == TypeSymbol::ts_char()) {
      ty = llvm::Type::getInt8Ty(ib->llvmContext());
    } else if (tp == TypeSymbol::ts_boolean()) {
      ty = llvm::Type::getInt1Ty(ib->llvmContext());
    } else if (tp == TypeSymbol::ts_void()) {
      ty = llvm::Type::getVoidTy(ib->llvmContext());
    } else {
      LOG_ASSERT(false, "invalid plain type {}:{}", tp->name(), tp->location());
    }
    tp->llvmType() = ty;
    ctx->llvmType() = ty;
  }
};

struct FuncDef : public Visitor {
  FuncDef() : Visitor("IrBuilder::Visitor::FuncDef") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    INITIALIZE(A_FuncDef)
    A_FuncSign *sign = static_cast<A_FuncSign *>(node->funcSign);
    A_VarId *varId = static_cast<A_VarId *>(sign->id);
    // update scope
    ctx->scope() = dynamic_cast<Scope *>(varId->symbol());
  }
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    INITIALIZE(A_FuncDef)
    // update scope
    ctx->scope() = ctx->scope()->owner();
  }
};

struct VarDef : public Visitor {
  VarDef() : Visitor("IrBuilder::Visitor::VarDef") {}
  virtual void finishVisit(Ast *ast, VisitorContext *context) {
    INITIALIZE(A_VarDef)
    A_VarId *varId = static_cast<A_VarId *>(node->id);
    A_PlainType *varTy = static_cast<A_PlainType *>(node->type);

    // variable type
    TypeSymbol *tsym = ctx->scope()->ts_resolve(tokenName(varTy->token));
    LOG_ASSERT(tsym, "variable {}:{} type symbol {}:{} not exist",
               varId->name(), varId->location(), tokenName(varTy->token),
               varTy->location());
    LOG_ASSERT(tsym->kind()._to_integral() ==
                   (+TypeSymbolKind::Plain)._to_integral(),
               "variable {}:{} type symbol {} kind {} != TypeSymbolKind::Plain",
               varId->name(), varId->location(), tsym->name(),
               tsym->kind()._to_string());
    Ts_Plain *tp = static_cast<Ts_Plain *>(tsym);
    llvm::Type *ty = tp->llvmType();

    // initialize expression
    llvm::Constant *init = llvm::dyn_cast<llvm::Constant>(ctx->llvmValue());

    llvm::GlobalVariable *gv = new llvm::GlobalVariable(
        *ctx->llvmModule(), ty, false, llvm::GlobalValue::ExternalLinkage, init,
        Label::globalVariable(varId).str(), nullptr,
        llvm::GlobalValue::NotThreadLocal, 0, false);

    // variable symbol
    Symbol *sym = ctx->scope()->s_resolve(varId->name());
    LOG_ASSERT(sym, "variable {}:{} symbol not exist", varId->name(),
               varId->location());
    LOG_ASSERT(sym->kind()._to_integral() == (+SymbolKind::Var)._to_integral(),
               "variable {}:{} kind {} != SymbolKind::Var", varId->name(),
               varId->location(), sym->kind()._to_string());
    S_Var *vs = static_cast<S_Var *>(sym);
    vs->llvmValue() = gv;
  }
};

struct CompileUnit : public Visitor {
  CompileUnit() : Visitor("IrBuilder::Visitor::CompileUnit") {}
  virtual void visit(Ast *ast, VisitorContext *context) {
    INITIALIZE(A_CompileUnit)
    // update scope
    LOG_ASSERT(!ctx->scope(),
               "context scope must be null before compile unit:{}",
               ctx->scope()->name());
    ctx->scope() = node->scope();
    // init llvmModule
    llvm::Module *m = new llvm::Module(node->name().str(), ib->llvmContext());
    ctx->llvmModule() = m;
    // global scope
    LOG_ASSERT(
        !node->scope()->owner(), "global scope {}:{} must has no owner {}:{}",
        node->scope()->name(), node->scope()->location(),
        node->scope()->owner()->name(), node->scope()->owner()->location());
    S_Global *gs = static_cast<S_Global *>(node->scope());
    gs->llvmModule() = m;
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

IrBuilder::IrBuilder()
    : Phase("IrBuilder"), llvmContext_(), llvmIRBuilder_(llvmContext_),
      context_(new detail::ir_builder::Context(this)), binder_(context_) {
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

  std::string s;
  llvm::raw_string_ostream sos(s);
  ctx->llvmModule()->print(sos, nullptr);
  llvmLL_ = s;
}

const Cowstr &IrBuilder::llvmLL() const { return llvmLL_; }

llvm::LLVMContext &IrBuilder::llvmContext() { return llvmContext_; }

const llvm::LLVMContext &IrBuilder::llvmContext() const { return llvmContext_; }

llvm::IRBuilder<> &IrBuilder::llvmIRBuilder() { return llvmIRBuilder_; }

const llvm::IRBuilder<> &IrBuilder::llvmIRBuilder() const {
  return llvmIRBuilder_;
}
