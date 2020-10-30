// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrBuilder.h"
#include "Ast.h"
#include "Label.h"
#include "Symbol.h"
#include "Token.h"
#include "iface/LLVMTypable.h"
#include "iface/LLVMValuable.h"
#include "iface/Scoped.h"
#include "infra/Log.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

#define INITIALIZE(astype)                                                     \
  Context *ctx = static_cast<Context *>(context());                            \
  IrBuilder *ib = ctx->irBuilder;                                              \
  astype *node = static_cast<astype *>(ast);                                   \
  (void)ctx;                                                                   \
  (void)ib;                                                                    \
  (void)node;

#define CHILD(x) ((x) == (child))

#define VDECL1(x)                                                              \
  struct x : public Visitor {                                                  \
    x();                                                                       \
    virtual void visit(Ast *ast);                                              \
  }

#define VDECL2(x)                                                              \
  struct x : public Visitor {                                                  \
    x();                                                                       \
    virtual void visit(Ast *ast);                                              \
    virtual void finishVisit(Ast *ast);                                        \
  }

namespace detail {

namespace ir_builder {

struct Context : public VisitorContext,
                 public Scoped,
                 public LLVMValuable,
                 public LLVMTypable {
  Context(IrBuilder *a_irBuilder) : irBuilder(a_irBuilder) {}

  IrBuilder *irBuilder;
};

VDECL1(Integer);
VDECL1(Float);
VDECL2(Loop);
struct Block : public Visitor {
  llvm::BasicBlock *entryBlock;
  Block();
  virtual void visit(Ast *ast);
  virtual void finishVisit(Ast *ast);
};
VDECL1(PlainType);
struct FuncDef : public Visitor {
  Cowstr funcName;
  std::vector<Cowstr> paramNames;
  std::vector<llvm::Type *> paramTypes;
  llvm::Type *resultType;
  llvm::FunctionType *funcType;
  llvm::Function *func;
  FuncDef();
  virtual void reset();
  virtual void visit(Ast *ast);
  virtual void visitAfter(Ast *ast, Ast *child);
  virtual void finishVisit(Ast *ast);
};
VDECL1(FuncSign);
struct Param : public Visitor {
  Param();
  virtual void visitAfter(Ast *ast, Ast *child);
};
struct VarDef : public Visitor, public LLVMValuable {
  llvm::Type *varTy;
  llvm::Constant *varInitializer;
  VarDef();
  virtual void visit(Ast *ast);
  virtual void visitAfter(Ast *ast, Ast *child);
  virtual void finishVisit(Ast *ast);
};
VDECL2(CompileUnit);

Integer::Integer() : Visitor("IrBuilder::Visitor::Integer") {}

void Integer::visit(Ast *ast) {
  INITIALIZE(A_Integer)
  ctx->resetLLVMValue();

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

Float::Float() : Visitor("IrBuilder::Visitor::Float") {}

void Float::visit(Ast *ast) {
  INITIALIZE(A_Float)
  ctx->resetLLVMValue();
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

Loop::Loop() : Visitor("IrBuilder::Visitor::Loop") {}

void Loop::visit(Ast *ast) {
  INITIALIZE(A_Loop)
  // update scope
  ctx->scope() = node->scope();
}

void Loop::finishVisit(Ast *ast) {
  INITIALIZE(A_Loop)
  // update scope
  ctx->scope() = ctx->scope()->owner();
}

Block::Block() : Visitor("IrBuilder::Visitor::Block") {}

void Block::visit(Ast *ast) {
  INITIALIZE(A_Block)
  entryBlock = nullptr;
  // update scope
  ctx->scope() = node->scope();
  FuncDef *fd = static_cast<FuncDef *>(visitor(+AstKind::FuncDef));
  llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(
      ib->llvmContext(), Label::basicBlock(ast).str(), fd->func);
  ib->llvmIRBuilder().SetInsertPoint(entryBlock);
}

void Block::finishVisit(Ast *ast) {
  INITIALIZE(A_Block)
  // update scope
  ctx->scope() = ctx->scope()->owner();
}

PlainType::PlainType() : Visitor("IrBuilder::Visitor::PlainType") {}

void PlainType::visit(Ast *ast) {
  INITIALIZE(A_PlainType)
  ctx->resetLLVMType();
  TypeSymbol *tsym = ctx->scope()->ts_resolve(tokenName(node->token));
  LOG_ASSERT(tsym, "type symbol {}:{} cannot resolve in scope {}:{}",
             tokenName(node->token), node->location(), ctx->scope()->name(),
             ctx->scope()->location());
  LOG_ASSERT(tsym->kind()._to_integral() ==
                 (+TypeSymbolKind::Plain)._to_integral(),
             "tsym {}:{} kind {} != TypeSymbolKind::Plain {}",
             tokenName(node->token), tsym->location(),
             tsym->kind()._to_string(), (+TypeSymbolKind::Plain)._to_string());

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

FuncDef::FuncDef() : Visitor("IrBuilder::Visitor::FuncDef") { reset(); }

void FuncDef::reset() {
  funcName = "";
  paramNames.clear();
  paramTypes.clear();
  resultType = nullptr;
  funcType = nullptr;
  func = nullptr;
}

void FuncDef::visit(Ast *ast) {
  INITIALIZE(A_FuncDef)
  A_FuncSign *funcSign = static_cast<A_FuncSign *>(node->funcSign);
  A_VarId *funcId = static_cast<A_VarId *>(funcSign->id);
  reset();
  // update scope
  ctx->scope() = dynamic_cast<Scope *>(funcId->symbol());
}

void FuncDef::visitAfter(Ast *ast, Ast *child) {
  INITIALIZE(A_FuncDef)
  if (CHILD(node->funcSign)) {
    // initialized:
    // - funcName
    // - paramNames
    // - paramTypes
  } else if (CHILD(node->resultType)) {
    // initialize:
    // - resultType
    // - funcType
    // - func
    LOG_ASSERT(node->resultType->kind() == +AstKind::PlainType,
               "funcDef resultType {}:{} kind {} != AstKind::PlainType",
               node->resultType->name(), node->resultType->location(),
               node->resultType->kind()._to_string());
    resultType = ctx->llvmType();
    funcType = llvm::FunctionType::get(resultType, paramTypes, false);
    func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,
                                  funcName.str(), ib->llvmModule());
    int i = 0;
    for (auto &arg : func->args()) {
      arg.setName(paramNames[i++].str());
    }
  } else if (CHILD(node->body)) {
    // initialized
    // - body's basic block
  }
}

void FuncDef::finishVisit(Ast *ast) {
  INITIALIZE(A_FuncDef)
  // update scope
  ctx->scope() = ctx->scope()->owner();
}

FuncSign::FuncSign() : Visitor("IrBuilder::Visitor::FuncSign") {}

void FuncSign::visit(Ast *ast) {
  INITIALIZE(A_FuncSign)
  A_VarId *funcId = static_cast<A_VarId *>(node->id);
  FuncDef *fd = static_cast<FuncDef *>(visitor(+AstKind::FuncDef));
  fd->funcName = Label::function(funcId);
}

Param::Param() : Visitor("IrBuilder::Visitor::Param") {}

void Param::visitAfter(Ast *ast, Ast *child) {
  INITIALIZE(A_Param)
  if (CHILD(node->id)) {
    LOG_ASSERT(node->id->kind() == +AstKind::VarId,
               "param id {}:{} kind {} != AstKind::VarId", node->id->name(),
               node->id->location(), node->id->kind()._to_string());
    FuncDef *fd = static_cast<FuncDef *>(visitor(+AstKind::FuncDef));
    fd->paramNames.push_back(node->id->name());
  } else if (CHILD(node->type)) {
    LOG_ASSERT(node->type->kind() == +AstKind::PlainType,
               "param type {}:{} kind {} != AstKind::PlainType",
               node->type->name(), node->type->location(),
               node->type->kind()._to_string());
    FuncDef *fd = static_cast<FuncDef *>(visitor(+AstKind::FuncDef));
    fd->paramTypes.push_back(ctx->llvmType());
  }
}

VarDef::VarDef() : Visitor("IrBuilder::Visitor::VarDef") {}

static void visitGlobalVariable(VarDef *v, Ast *ast) {
  v->varTy = nullptr;
  v->varInitializer = nullptr;
  v->resetLLVMValue();
}
static void visitAfterGlobalVariable(VarDef *v, Ast *ast, Ast *child) {
  Context *ctx = static_cast<Context *>(v->context());
  A_VarDef *node = static_cast<A_VarDef *>(ast);
  if (CHILD(node->type)) {
    // variable type
    v->varTy = ctx->llvmType();
  } else if (CHILD(node->expr)) {
    // variable initializer constant expression
    v->varInitializer = llvm::dyn_cast<llvm::Constant>(ctx->llvmValue());
  }
}
static void finishVisitGlobalVariable(VarDef *v, Ast *ast) {
  Context *ctx = static_cast<Context *>(v->context());
  IrBuilder *ib = ctx->irBuilder;
  A_VarDef *node = static_cast<A_VarDef *>(ast);
  A_VarId *varId = static_cast<A_VarId *>(node->id);

  llvm::GlobalVariable *gv = new llvm::GlobalVariable(
      *ib->llvmModule(), v->varTy, false, llvm::GlobalValue::ExternalLinkage,
      v->varInitializer, Label::globalVariable(varId).str(), nullptr,
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

static void visitLocalVariable(VarDef *v, Ast *ast) {
  v->varTy = nullptr;
  v->varInitializer = nullptr;
  v->resetLLVMValue();
}
static void visitAfterLocalVariable(VarDef *v, Ast *ast, Ast *child) {
  Context *ctx = static_cast<Context *>(v->context());
  A_VarDef *node = static_cast<A_VarDef *>(ast);
  if (CHILD(node->type)) {
    v->varTy = ctx->llvmType();
  } else if (CHILD(node->expr)) {
    v->varInitializer = llvm::dyn_cast<llvm::Constant>(ctx->llvmValue());
  }
}
static void finishVisitLocalVariable(VarDef *v, Ast *ast) {
  Context *ctx = static_cast<Context *>(v->context());
  IrBuilder *ib = ctx->irBuilder;
  A_VarDef *node = static_cast<A_VarDef *>(ast);
  A_VarId *varId = static_cast<A_VarId *>(node->id);

  llvm::AllocaInst *ai = ib->llvmIRBuilder().CreateAlloca(
      v->varTy, nullptr, Label::localVariable(varId).str());
  llvm::StoreInst *si =
      ib->llvmIRBuilder().CreateStore(v->varInitializer, ai, false);

  // variable symbol
  Symbol *sym = ctx->scope()->s_resolve(varId->name());
  LOG_ASSERT(sym, "variable {}:{} symbol not exist", varId->name(),
             varId->location());
  LOG_ASSERT(sym->kind() == +SymbolKind::Var,
             "variable {}:{} kind {} != SymbolKind::Var", varId->name(),
             varId->location(), sym->kind()._to_string());
  S_Var *vs = static_cast<S_Var *>(sym);
  vs->llvmValue() = si;
}

void VarDef::visit(Ast *ast) {
  INITIALIZE(A_VarDef)
  if (!ctx->scope()->owner()) {
    visitGlobalVariable(this, ast);
  } else {
    visitLocalVariable(this, ast);
  }
}

void VarDef::visitAfter(Ast *ast, Ast *child) {
  INITIALIZE(A_VarDef)
  if (!ctx->scope()->owner()) {
    visitAfterGlobalVariable(this, ast, child);
  } else {
    visitAfterLocalVariable(this, ast, child);
  }
}

void VarDef::finishVisit(Ast *ast) {
  INITIALIZE(A_VarDef)
  if (!ctx->scope()->owner()) {
    finishVisitGlobalVariable(this, ast);
  } else {
    finishVisitLocalVariable(this, ast);
  }
}

CompileUnit::CompileUnit() : Visitor("IrBuilder::Visitor::CompileUnit") {}

void CompileUnit::visit(Ast *ast) {
  INITIALIZE(A_CompileUnit)
  // update scope
  LOG_ASSERT(!ctx->scope(), "context scope must be null before compile unit:{}",
             ctx->scope()->name());
  ctx->scope() = node->scope();
  // init llvmModule
  llvm::Module *m = new llvm::Module(node->name().str(), ib->llvmContext());
  ib->llvmModule() = m;
}

void CompileUnit::finishVisit(Ast *ast) {
  Context *ctx = static_cast<Context *>(context());
  // update scope
  ctx->scope() = ctx->scope()->owner();
  LOG_ASSERT(!ctx->scope(), "global scope's owner scope must be null:{}",
             ctx->scope()->name());
}

} // namespace ir_builder

} // namespace detail

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new detail::ir_builder::x();                                  \
    binder_.bind(+AstKind::x, v);                                              \
    visitors_.push_back(v);                                                    \
  } while (0)

IrBuilder::IrBuilder()
    : Phase("IrBuilder"), llvmContext_(), llvmIRBuilder_(llvmContext_),
      llvmModule_(nullptr), context_(new detail::ir_builder::Context(this)),
      binder_(context_) {
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
  delete llvmModule_;
  llvmModule_ = nullptr;
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

  std::string s;
  llvm::raw_string_ostream sos(s);
  llvmModule_->print(sos, nullptr);
  llvmLL_ = s;
}

const Cowstr &IrBuilder::llvmLL() const { return llvmLL_; }

llvm::LLVMContext &IrBuilder::llvmContext() { return llvmContext_; }

const llvm::LLVMContext &IrBuilder::llvmContext() const { return llvmContext_; }

llvm::IRBuilder<> &IrBuilder::llvmIRBuilder() { return llvmIRBuilder_; }

const llvm::IRBuilder<> &IrBuilder::llvmIRBuilder() const {
  return llvmIRBuilder_;
}

llvm::Module *&IrBuilder::llvmModule() { return llvmModule_; }

const llvm::Module *IrBuilder::llvmModule() const { return llvmModule_; }
