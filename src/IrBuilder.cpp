// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrBuilder.h"
#include "Ast.h"
#include "Symbol.h"
#include "Token.h"
#include "boost/preprocessor/stringize.hpp"
#include "infra/Log.h"
#include <vector>

namespace detail {

// I_Mod {

I_Mod::VISITOR(GlobalVar)::VISITOR(GlobalVar)()
    : Visitor("IrBuilder::I_Mod::" BOOST_PP_STRINGIZE(VISITOR(GlobalVar))) {}

void I_Mod::VISITOR(GlobalVar)::visit(Ast *ast) {
  I_Mod::Context *ctx = static_cast<I_Mod::Context *>(context());
  ctx->iMod->iGlobalVar->run(ast);
}

I_Mod::VISITOR(FuncDef)::VISITOR(FuncDef)()
    : Visitor("IrBuilder::I_Mod::" BOOST_PP_STRINGIZE(VISITOR(FuncDef))) {}

void I_Mod::VISITOR(FuncDef)::visit(Ast *ast) {
  I_Mod::Context *ctx = static_cast<I_Mod::Context *>(context());
  ctx->iMod->iFuncDef->run(ast);
}

#define BIND(x, y)                                                             \
  do {                                                                         \
    Visitor *v = new detail::I_Mod::VISITOR(x)();                              \
    binder.bind((+AstKind::y), v);                                             \
    visitors.push_back(v);                                                     \
  } while (0)

I_Mod::I_Mod(IrBuilder *irBuilder)
    : Phase("I_Mod"),
      // traversor
      context(new I_Mod::Context(irBuilder, this)), binder(context),
      // other traversors
      iGlobalVar(new I_GlobalVar()), iFuncDef(new I_FuncDef()) {
  BIND(GlobalVar, VarDef);
  BIND(FuncDef, FuncDef);
}

I_Mod::~I_Mod() {
  delete context;
  context = nullptr;
  for (int i = 0; i < (int)visitors.size(); i++) {
    delete visitors[i];
    visitors[i] = nullptr;
  }
  delete iGlobalVar;
  iGlobalVar = nullptr;
  delete iFuncDef;
  iFuncDef = nullptr;
}

void I_Mod::run(Ast *ast) { Visitor::traverse(&binder, ast); }

#undef BIND

// I_Mod }

struct Context : public VisitorContext, public Scoped, public Modular {};
struct VISITOR(GlobalVar) : public Visitor {
  VISITOR(GlobalVar)();
  virtual void visit(Ast *ast);
};
struct VISITOR(FuncDef) : public Visitor {
  VISITOR(FuncDef)();
  virtual void visit(Ast *ast);
};

VisitorContext *context;
VisitorBinder binder;
std::vector<Visitor *> visitors;

I_GlobalVar *iGlobalVar;
I_FuncDef *iFuncDef;

llvm::LLVMContext llvmContext;
llvm::IRBuilder<> llvmIRBuilder;
llvm::Module *llvmModule;

namespace ir_builder {

struct Context : public VisitorContext, public Scoped {
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

struct VISITOR(VarDef) : public Visitor {
  Ir *ir;

  VISITOR(VarDef)
  () : Visitor("IrBuilder" BOOST_PP_STRINGIZE(VISITOR(VarDef))), ir(nullptr) {}

  virtual void visit(Ast *ast) {
    INITIALIZE(A_VarDef)
    S_Var *s_var = static_cast<S_Var *>(node->symbol());
    if (!s_var->owner()->owner()) {
      ir = new I_GlobalVar();
    }
  }

  virtual void finishVisit(Ast *ast) { INITIALIZE(A_VarDef) }
};

// VarDefVisitor {

IrBuilder::VISITOR(VarDef)::VISITOR(VarDef)()
    : Visitor("IrBuilder::" BOOST_PP_STRINGIZE(VISITOR(VarDef))), ir(nullptr) {}

void IrBuilder::VISITOR(VarDef)::visitAfter(Ast *ast, Ast *child) {
  INITIALIZE(A_VarDef)
  if (node->id == child) {
    IrBuilder::VISITOR(VarId) *v = visitor(+AstKind::VarId);
    if (!v->symbol->owner()->owner()) {
      // global variable
      ir = new I_GlobalVar();
    } else {
      // local variable
      ir = new I_Stat();
    }
  } else if (node->type == child) {

  } else if (node->expr == child) {
  }
}

void IrBuilder::VISITOR(VarDef)::finishVisit(Ast *ast) {}

// VarDefVisitor }

// VarIdVisitor {

IrBuilder::VISITOR(VarId)::VISITOR(VarId)()
    : Visitor("IrBuilder::" BOOST_PP_STRINGIZE(VISITOR(VarId))),
              symbol(nullptr) {}

void IrBuilder::VISITOR(VarId)::visit(Ast *ast) {
  INITIALIZE(A_VarId)
  LOG_ASSERT(node->symbol()->kind() == (+SymbolKind::Var),
             "symbol {}:{} kind {} != SymbolKind::Var", node->symbol());
  symbol = node->symbol();
}

// VarIdVisitor }

// PlainTypeVisitor {

IrBuilder::VISITOR(PlainType)::VISITOR(PlainType)()
    : Visitor("IrBuilder::" BOOST_PP_STRINGIZE(VISITOR(PlainType))),
              typeSymbol(nullptr) {}

void IrBuilder::VISITOR(PlainType)::visit(Ast *ast) {
  INITIALIZE(A_PlainType)
  typeSymbol = ctx->scope()->ts_resolve(node->name());
  LOG_ASSERT(typeSymbol, "plain type {}:{} cannot resolve in scope {}:{}",
             node->name(), node->location(), ctx->scope()->name(),
             ctx->scope()->location());
}

// PlainTypeVisitor }

// CompileUnitVisitor {

IrBuilder::VISITOR(CompileUnit)::VISITOR(CompileUnit)()
    : Visitor("IrBuilder::" BOOST_PP_STRINGIZE(VISITOR(CompileUnit))),
              ir(nullptr) {}

void IrBuilder::VISITOR(CompileUnit)::visit(Ast *ast) {
  INITIALIZE(A_CompileUnit)
  ctx->scope() = node->scope();
  ir = new I_Mod();
  ib->ir() = ir;
}

void IrBuilder::VISITOR(CompileUnit)::finishVisit(Ast *ast) {
  INITIALIZE(A_CompileUnit)
  ctx->scope() = ctx->scope()->owner();
}

// CompileUnitVisitor }

static Cowstr label(Ast *ast) {
  return fmt::format("{}.{}", ast->name(), ast->location());
}

IrBuilder::IrBuilder()
    : Phase("IrBuilder"), llvmContext_(), llvmIRBuilder_(llvmContext_),
      llvmModule_(nullptr), currentScope_(nullptr) {}

IrBuilder::~IrBuilder() {}

void IrBuilder::run(Ast *ast) { ast->accept(this); }

void IrBuilder::visitInteger(A_Integer *ast) {
  switch (ast->bit()) {
  case 32: {
    llvm::APInt ap = ast->isSigned() ? llvm::APInt(32, ast->asInt32(), true)
                                     : llvm::APInt(ast->asUInt32(), false);
    llvm::ConstantInt *ci = llvm::ConstantInt::get(llvmContext_, ap);
    llvmValue_ = llvm::dyn_cast<llvm::Value>(ci);
    break;
  }
  case 64: {
    llvm::APInt ap = ast->isSigned() ? llvm::APInt(64, ast->asInt64(), true)
                                     : llvm::APInt(64, ast->asUInt64(), false);
    llvm::ConstantInt *ci = llvm::ConstantInt::get(llvmContext_, ap);
    llvmValue_ = llvm::dyn_cast<llvm::Value>(ci);
    break;
  }
  default:
    LOG_ASSERT(false, "invalid bit {} in ast {}:{}", ast->bit(), ast->name(),
               ast->location());
    break;
  }
}

void IrBuilder::visitFloat(A_Float *ast) {
  switch (ast->bit()) {
  case 32: {
    llvm::APFloat ap = llvm::APFloat(ast->asFloat());
    llvm::ConstantFP *cf = llvm::ConstantFP::get(llvmContext_, ap);
    llvmValue_ = llvm::dyn_cast<llvm::Value>(cf);
    break;
  }
  case 64: {
    llvm::APFloat ap = llvm::APFloat(ast->asDouble());
    llvm::ConstantFP *cf = llvm::ConstantFP::get(llvmContext_, ap);
    llvmValue_ = llvm::dyn_cast<llvm::Value>(cf);
    break;
  }
  default:
    LOG_ASSERT(false, "invalid float ast {}:{}", ast->name(), ast->location());
    break;
  }
}

void IrBuilder::visitBoolean(A_Boolean *ast) {}

void IrBuilder::visitCharacter(A_Character *ast);

void IrBuilder::visitString(A_String *ast);

void IrBuilder::visitNil(A_Nil *ast);

void IrBuilder::visitVoid(A_Void *ast);
void IrBuilder::visitVarId(A_VarId *ast);
void IrBuilder::visitBreak(A_Break *ast);
void IrBuilder::visitContinue(A_Continue *ast);

void IrBuilder::visitThrow(A_Throw *ast);
void IrBuilder::visitReturn(A_Return *ast);
void IrBuilder::visitAssign(A_Assign *ast);
void IrBuilder::visitPostfix(A_Postfix *ast);
void IrBuilder::visitInfix(A_Infix *ast);
void IrBuilder::visitPrefix(A_Prefix *ast);
void IrBuilder::visitCall(A_Call *ast);
void IrBuilder::visitExprs(A_Exprs *ast);
void IrBuilder::visitIf(A_If *ast);
void IrBuilder::visitLoop(A_Loop *ast);
void IrBuilder::visitYield(A_Yield *ast);
void IrBuilder::visitLoopCondition(A_LoopCondition *ast);
void IrBuilder::visitLoopEnumerator(A_LoopEnumerator *ast);
void IrBuilder::visitDoWhile(A_DoWhile *ast);
void IrBuilder::visitTry(A_Try *ast);
void IrBuilder::visitBlock(A_Block *ast);
void IrBuilder::visitBlockStats(A_BlockStats *ast);

void IrBuilder::visitPlainType(A_PlainType *ast) {
  TypeSymbol *ts = currentScope_->ts_resolve(ast->name());
  Ts_Plain *tp = static_cast<Ts_Plain *>(ts);
  llvm::Type *ty = nullptr;
  if (tp == TypeSymbol::ts_byte() || tp == TypeSymbol::ts_ubyte()) {
    ty = llvm::Type::getInt8Ty(llvmContext_);
  } else if (tp == TypeSymbol::ts_short() || tp == TypeSymbol::ts_ushort()) {
    ty = llvm::Type::getInt16Ty(llvmContext_);
  } else if (tp == TypeSymbol::ts_int() || tp == TypeSymbol::ts_uint()) {
    ty = llvm::Type::getInt32Ty(llvmContext_);
  } else if (tp == TypeSymbol::ts_long() || tp == TypeSymbol::ts_ulong()) {
    ty = llvm::Type::getInt64Ty(llvmContext_);
  } else if (tp == TypeSymbol::ts_float()) {
    ty = llvm::Type::getFloatTy(llvmContext_);
  } else if (tp == TypeSymbol::ts_double()) {
    ty = llvm::Type::getDoubleTy(llvmContext_);
  } else if (tp == TypeSymbol::ts_char()) {
    ty = llvm::Type::getInt8Ty(llvmContext_);
  } else if (tp == TypeSymbol::ts_boolean()) {
    ty = llvm::Type::getInt1Ty(llvmContext_);
  } else if (tp == TypeSymbol::ts_void()) {
    ty = llvm::Type::getVoidTy(llvmContext_);
  } else {
    LOG_ASSERT(false, "invalid plain type{}:{}", tp->name(), tp->location());
  }
  llvmType_ = ty;
}

void IrBuilder::visitFuncDef(A_FuncDef *ast);
void IrBuilder::visitFuncSign(A_FuncSign *ast);
void IrBuilder::visitParams(A_Params *ast);
void IrBuilder::visitParam(A_Param *ast);

void IrBuilder::visitVarDef(A_VarDef *ast) {
  A_VarId *varId = static_cast<A_VarId *>(ast->id);

  ast->type->accept(this);
  llvm::Type *ty_var = llvmType_;

  if (ast->parent()->kind() == (+AstKind::TopStats) ||
      ast->parent()->kind() == (+AstKind::CompileUnit)) {
    IrBuilder::ConstantBuilder cb;
    ast->expr->accept(&cb);
    llvm::GlobalVariable *gv = new llvm::GlobalVariable(
        llvmModule_, ty_var, false, llvm::GlobalValue::ExternalLinkage,
        cb.llvmConstant, label(ast), nullptr, llvm::GlobalValue::NotThreadLocal,
        0, false);
  } else if (ast->parent()->kind() == (+AstKind::BlockStats)) {
    ast->expr->accept(this);
    llvm::AllocaInst *ai =
        llvmIRBuilder_.CreateAlloca(ty_var, nullptr, label(varId).str());
    llvm::StoreInst *si = llvmIRBuilder_.CreateStore(llvmValue_, ai, false);
  }
}

void IrBuilder::visitCompileUnit(A_CompileUnit *ast) {
  llvmModule_ = new llvm::Module(ast->name(), llvmContext_);
  currentScope_ = ast->scope();

  if (ast->topStats) {
    ast->topStats->accept(this);
  }

  currentScope_ = currentScope_->owner();
}
