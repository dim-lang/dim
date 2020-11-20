// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "IrBuilder.h"
#include "Ast.h"
#include "Symbol.h"
#include "Token.h"
#include "boost/preprocessor/stringize.hpp"
#include "infra/LinkedHashMap.hpp"
#include "infra/Log.h"
#include "llvm/Support/Casting.h"

// namespace detail {
//
// // I_Mod {
//
// I_Mod::VISITOR(GlobalVar)::VISITOR(GlobalVar)()
//     : Visitor("IrBuilder::I_Mod::" BOOST_PP_STRINGIZE(VISITOR(GlobalVar))) {}
//
// void I_Mod::VISITOR(GlobalVar)::visit(Ast *ast) {
//   I_Mod::Context *ctx = static_cast<I_Mod::Context *>(context());
//   ctx->iMod->iGlobalVar->run(ast);
// }
//
// I_Mod::VISITOR(FuncDef)::VISITOR(FuncDef)()
//     : Visitor("IrBuilder::I_Mod::" BOOST_PP_STRINGIZE(VISITOR(FuncDef))) {}
//
// void I_Mod::VISITOR(FuncDef)::visit(Ast *ast) {
//   I_Mod::Context *ctx = static_cast<I_Mod::Context *>(context());
//   ctx->iMod->iFuncDef->run(ast);
// }
//
// #define BIND(x, y) \
//   do { \
//     Visitor *v = new detail::I_Mod::VISITOR(x)(); \
//     binder.bind((+AstKind::y), v); \
//     visitors.push_back(v); \
//   } while (0)
//
// I_Mod::I_Mod(IrBuilder *irBuilder)
//     : Phase("I_Mod"),
//       // traversor
//       context(new I_Mod::Context(irBuilder, this)), binder(context),
//       // other traversors
//       iGlobalVar(new I_GlobalVar()), iFuncDef(new I_FuncDef()) {
//   BIND(GlobalVar, VarDef);
//   BIND(FuncDef, FuncDef);
// }
//
// I_Mod::~I_Mod() {
//   delete context;
//   context = nullptr;
//   for (int i = 0; i < (int)visitors.size(); i++) {
//     delete visitors[i];
//     visitors[i] = nullptr;
//   }
//   delete iGlobalVar;
//   iGlobalVar = nullptr;
//   delete iFuncDef;
//   iFuncDef = nullptr;
// }
//
// void I_Mod::run(Ast *ast) { Visitor::traverse(&binder, ast); }
//
// #undef BIND
//
// // I_Mod }
//
// struct Context : public VisitorContext, public Scoped, public Modular {};
// struct VISITOR(GlobalVar) : public Visitor {
//   VISITOR(GlobalVar)();
//   virtual void visit(Ast *ast);
// };
// struct VISITOR(FuncDef) : public Visitor {
//   VISITOR(FuncDef)();
//   virtual void visit(Ast *ast);
// };
//
// VisitorContext *context;
// VisitorBinder binder;
// std::vector<Visitor *> visitors;
//
// I_GlobalVar *iGlobalVar;
// I_FuncDef *iFuncDef;
//
// llvm::LLVMContext llvmContext;
// llvm::IRBuilder<> llvmIRBuilder;
// llvm::Module *llvmModule;
//
// namespace ir_builder {
//
// struct Context : public VisitorContext, public Scoped {
//   Context(IrBuilder *a_irBuilder) : irBuilder(a_irBuilder) {}
//   IrBuilder *irBuilder;
// };
//
// VDECL1(Integer);
// VDECL1(Float);
// VDECL2(Loop);
// struct Block : public Visitor {
//   llvm::BasicBlock *entryBlock;
//   Block();
//   virtual void visit(Ast *ast);
//   virtual void finishVisit(Ast *ast);
// };
// VDECL1(PlainType);
// struct FuncDef : public Visitor {
//   Cowstr funcName;
//   std::vector<Cowstr> paramNames;
//   std::vector<llvm::Type *> paramTypes;
//   llvm::Type *resultType;
//   llvm::FunctionType *funcType;
//   llvm::Function *func;
//   FuncDef();
//   virtual void reset();
//   virtual void visit(Ast *ast);
//   virtual void visitAfter(Ast *ast, Ast *child);
//   virtual void finishVisit(Ast *ast);
// };
// VDECL1(FuncSign);
// struct Param : public Visitor {
//   Param();
//   virtual void visitAfter(Ast *ast, Ast *child);
// };
// struct VarDef : public Visitor, public LLVMValuable {
//   llvm::Type *varTy;
//   llvm::Constant *varInitializer;
//   VarDef();
//   virtual void visit(Ast *ast);
//   virtual void visitAfter(Ast *ast, Ast *child);
//   virtual void finishVisit(Ast *ast);
// };
// VDECL2(CompileUnit);
//
// Integer::Integer() : Visitor("IrBuilder::Visitor::Integer") {}
//
// void Integer::visit(Ast *ast) {
//   INITIALIZE(A_Integer)
//   ctx->resetLLVMValue();
//
//   switch (node->bit()) {
//   case 32: {
//     llvm::APInt ap = node->isSigned()
//                          ? llvm::APInt(32, node->asInt32(), true)
//                          : llvm::APInt(32, node->asUInt32(), false);
//     llvm::ConstantInt *ci = llvm::ConstantInt::get(ib->llvmContext(), ap);
//     node->llvmValue() = ci;
//     ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(ci);
//     break;
//   }
//   case 64: {
//     llvm::APInt ap = node->isSigned()
//                          ? llvm::APInt(64, node->asInt64(), true)
//                          : llvm::APInt(64, node->asUInt64(), false);
//     llvm::ConstantInt *ci = llvm::ConstantInt::get(ib->llvmContext(), ap);
//     node->llvmValue() = ci;
//     ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(ci);
//     break;
//   }
//   default:
//     LOG_ASSERT(false, "invalid integer node {}:{}", node->name(),
//                node->location());
//   }
// }
//
// Float::Float() : Visitor("IrBuilder::Visitor::Float") {}
//
// void Float::visit(Ast *ast) {
//   INITIALIZE(A_Float)
//   ctx->resetLLVMValue();
//   switch (node->bit()) {
//   case 32: {
//     llvm::APFloat ap = llvm::APFloat(node->asFloat());
//     llvm::ConstantFP *cf = llvm::ConstantFP::get(ib->llvmContext(), ap);
//     node->llvmValue() = cf;
//     ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(cf);
//     break;
//   }
//   case 64: {
//     llvm::APFloat ap = llvm::APFloat(node->asDouble());
//     llvm::ConstantFP *cf = llvm::ConstantFP::get(ib->llvmContext(), ap);
//     node->llvmValue() = cf;
//     ctx->llvmValue() = llvm::dyn_cast<llvm::Value>(cf);
//     break;
//   }
//   default:
//     LOG_ASSERT(false, "invalid float node {}:{}", node->name(),
//                node->location());
//   }
// }
//
// Loop::Loop() : Visitor("IrBuilder::Visitor::Loop") {}
//
// void Loop::visit(Ast *ast) {
//   INITIALIZE(A_Loop)
//   // update scope
//   ctx->scope() = node->scope();
// }
//
// void Loop::finishVisit(Ast *ast) {
//   INITIALIZE(A_Loop)
//   // update scope
//   ctx->scope() = ctx->scope()->owner();
// }
//
// Block::Block() : Visitor("IrBuilder::Visitor::Block") {}
//
// void Block::visit(Ast *ast) {
//   INITIALIZE(A_Block)
//   entryBlock = nullptr;
//   // update scope
//   ctx->scope() = node->scope();
//   FuncDef *fd = static_cast<FuncDef *>(visitor(+AstKind::FuncDef));
//   llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(
//       ib->llvmContext(), Label::basicBlock(ast).str(), fd->func);
//   ib->llvmIRBuilder().SetInsertPoint(entryBlock);
// }
//
// void Block::finishVisit(Ast *ast) {
//   INITIALIZE(A_Block)
//   // update scope
//   ctx->scope() = ctx->scope()->owner();
// }
//
// PlainType::PlainType() : Visitor("IrBuilder::Visitor::PlainType") {}
//
// void PlainType::visit(Ast *ast) {
//   INITIALIZE(A_PlainType)
//   ctx->resetLLVMType();
//   TypeSymbol *tsym = ctx->scope()->ts_resolve(tokenName(node->token));
//   LOG_ASSERT(tsym, "type symbol {}:{} cannot resolve in scope {}:{}",
//              tokenName(node->token), node->location(), ctx->scope()->name(),
//              ctx->scope()->location());
//   LOG_ASSERT(tsym->kind()._to_integral() ==
//                  (+TypeSymbolKind::Plain)._to_integral(),
//              "tsym {}:{} kind {} != TypeSymbolKind::Plain {}",
//              tokenName(node->token), tsym->location(),
//              tsym->kind()._to_string(),
//              (+TypeSymbolKind::Plain)._to_string());
//
//   Ts_Plain *tp = static_cast<Ts_Plain *>(tsym);
//   llvm::Type *ty = nullptr;
//   if (tp == TypeSymbol::ts_byte() || tp == TypeSymbol::ts_ubyte()) {
//     ty = llvm::Type::getInt8Ty(ib->llvmContext());
//   } else if (tp == TypeSymbol::ts_short() || tp == TypeSymbol::ts_ushort()) {
//     ty = llvm::Type::getInt16Ty(ib->llvmContext());
//   } else if (tp == TypeSymbol::ts_int() || tp == TypeSymbol::ts_uint()) {
//     ty = llvm::Type::getInt32Ty(ib->llvmContext());
//   } else if (tp == TypeSymbol::ts_long() || tp == TypeSymbol::ts_ulong()) {
//     ty = llvm::Type::getInt64Ty(ib->llvmContext());
//   } else if (tp == TypeSymbol::ts_float()) {
//     ty = llvm::Type::getFloatTy(ib->llvmContext());
//   } else if (tp == TypeSymbol::ts_double()) {
//     ty = llvm::Type::getDoubleTy(ib->llvmContext());
//   } else if (tp == TypeSymbol::ts_char()) {
//     ty = llvm::Type::getInt8Ty(ib->llvmContext());
//   } else if (tp == TypeSymbol::ts_boolean()) {
//     ty = llvm::Type::getInt1Ty(ib->llvmContext());
//   } else if (tp == TypeSymbol::ts_void()) {
//     ty = llvm::Type::getVoidTy(ib->llvmContext());
//   } else {
//     LOG_ASSERT(false, "invalid plain type {}:{}", tp->name(),
//     tp->location());
//   }
//   tp->llvmType() = ty;
//   ctx->llvmType() = ty;
// }
//
// FuncDef::FuncDef() : Visitor("IrBuilder::Visitor::FuncDef") { reset(); }
//
// void FuncDef::reset() {
//   funcName = "";
//   paramNames.clear();
//   paramTypes.clear();
//   resultType = nullptr;
//   funcType = nullptr;
//   func = nullptr;
// }
//
// void FuncDef::visit(Ast *ast) {
//   INITIALIZE(A_FuncDef)
//   A_FuncSign *funcSign = static_cast<A_FuncSign *>(node->funcSign);
//   A_VarId *funcId = static_cast<A_VarId *>(funcSign->id);
//   reset();
//   // update scope
//   ctx->scope() = dynamic_cast<Scope *>(funcId->symbol());
// }
//
// void FuncDef::visitAfter(Ast *ast, Ast *child) {
//   INITIALIZE(A_FuncDef)
//   if (CHILD(node->funcSign)) {
//     // initialized:
//     // - funcName
//     // - paramNames
//     // - paramTypes
//   } else if (CHILD(node->resultType)) {
//     // initialize:
//     // - resultType
//     // - funcType
//     // - func
//     LOG_ASSERT(node->resultType->kind() == +AstKind::PlainType,
//                "funcDef resultType {}:{} kind {} != AstKind::PlainType",
//                node->resultType->name(), node->resultType->location(),
//                node->resultType->kind()._to_string());
//     resultType = ctx->llvmType();
//     funcType = llvm::FunctionType::get(resultType, paramTypes, false);
//     func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,
//                                   funcName.str(), ib->llvmModule());
//     int i = 0;
//     for (auto &arg : func->args()) {
//       arg.setName(paramNames[i++].str());
//     }
//   } else if (CHILD(node->body)) {
//     // initialized
//     // - body's basic block
//   }
// }
//
// void FuncDef::finishVisit(Ast *ast) {
//   INITIALIZE(A_FuncDef)
//   // update scope
//   ctx->scope() = ctx->scope()->owner();
// }
//
// FuncSign::FuncSign() : Visitor("IrBuilder::Visitor::FuncSign") {}
//
// void FuncSign::visit(Ast *ast) {
//   INITIALIZE(A_FuncSign)
//   A_VarId *funcId = static_cast<A_VarId *>(node->id);
//   FuncDef *fd = static_cast<FuncDef *>(visitor(+AstKind::FuncDef));
//   fd->funcName = Label::function(funcId);
// }
//
// Param::Param() : Visitor("IrBuilder::Visitor::Param") {}
//
// void Param::visitAfter(Ast *ast, Ast *child) {
//   INITIALIZE(A_Param)
//   if (CHILD(node->id)) {
//     LOG_ASSERT(node->id->kind() == +AstKind::VarId,
//                "param id {}:{} kind {} != AstKind::VarId", node->id->name(),
//                node->id->location(), node->id->kind()._to_string());
//     FuncDef *fd = static_cast<FuncDef *>(visitor(+AstKind::FuncDef));
//     fd->paramNames.push_back(node->id->name());
//   } else if (CHILD(node->type)) {
//     LOG_ASSERT(node->type->kind() == +AstKind::PlainType,
//                "param type {}:{} kind {} != AstKind::PlainType",
//                node->type->name(), node->type->location(),
//                node->type->kind()._to_string());
//     FuncDef *fd = static_cast<FuncDef *>(visitor(+AstKind::FuncDef));
//     fd->paramTypes.push_back(ctx->llvmType());
//   }
// }
//
// VarDef::VarDef() : Visitor("IrBuilder::Visitor::VarDef") {}
//
// static void visitGlobalVariable(VarDef *v, Ast *ast) {
//   v->varTy = nullptr;
//   v->varInitializer = nullptr;
//   v->resetLLVMValue();
// }
// static void visitAfterGlobalVariable(VarDef *v, Ast *ast, Ast *child) {
//   Context *ctx = static_cast<Context *>(v->context());
//   A_VarDef *node = static_cast<A_VarDef *>(ast);
//   if (CHILD(node->type)) {
//     // variable type
//     v->varTy = ctx->llvmType();
//   } else if (CHILD(node->expr)) {
//     // variable initializer constant expression
//     v->varInitializer = llvm::dyn_cast<llvm::Constant>(ctx->llvmValue());
//   }
// }
// static void finishVisitGlobalVariable(VarDef *v, Ast *ast) {
//   Context *ctx = static_cast<Context *>(v->context());
//   IrBuilder *ib = ctx->irBuilder;
//   A_VarDef *node = static_cast<A_VarDef *>(ast);
//   A_VarId *varId = static_cast<A_VarId *>(node->id);
//
//   llvm::GlobalVariable *gv = new llvm::GlobalVariable(
//       *ib->llvmModule(), v->varTy, false, llvm::GlobalValue::ExternalLinkage,
//       v->varInitializer, Label::globalVariable(varId).str(), nullptr,
//       llvm::GlobalValue::NotThreadLocal, 0, false);
//
//   // variable symbol
//   Symbol *sym = ctx->scope()->s_resolve(varId->name());
//   LOG_ASSERT(sym, "variable {}:{} symbol not exist", varId->name(),
//              varId->location());
//   LOG_ASSERT(sym->kind()._to_integral() == (+SymbolKind::Var)._to_integral(),
//              "variable {}:{} kind {} != SymbolKind::Var", varId->name(),
//              varId->location(), sym->kind()._to_string());
//   S_Var *vs = static_cast<S_Var *>(sym);
//   vs->llvmValue() = gv;
// }
//
// static void visitLocalVariable(VarDef *v, Ast *ast) {
//   v->varTy = nullptr;
//   v->varInitializer = nullptr;
//   v->resetLLVMValue();
// }
// static void visitAfterLocalVariable(VarDef *v, Ast *ast, Ast *child) {
//   Context *ctx = static_cast<Context *>(v->context());
//   A_VarDef *node = static_cast<A_VarDef *>(ast);
//   if (CHILD(node->type)) {
//     v->varTy = ctx->llvmType();
//   } else if (CHILD(node->expr)) {
//     v->varInitializer = llvm::dyn_cast<llvm::Constant>(ctx->llvmValue());
//   }
// }
// static void finishVisitLocalVariable(VarDef *v, Ast *ast) {
//   Context *ctx = static_cast<Context *>(v->context());
//   IrBuilder *ib = ctx->irBuilder;
//   A_VarDef *node = static_cast<A_VarDef *>(ast);
//   A_VarId *varId = static_cast<A_VarId *>(node->id);
//
//   llvm::AllocaInst *ai = ib->llvmIRBuilder().CreateAlloca(
//       v->varTy, nullptr, Label::localVariable(varId).str());
//   llvm::StoreInst *si =
//       ib->llvmIRBuilder().CreateStore(v->varInitializer, ai, false);
//
//   // variable symbol
//   Symbol *sym = ctx->scope()->s_resolve(varId->name());
//   LOG_ASSERT(sym, "variable {}:{} symbol not exist", varId->name(),
//              varId->location());
//   LOG_ASSERT(sym->kind() == +SymbolKind::Var,
//              "variable {}:{} kind {} != SymbolKind::Var", varId->name(),
//              varId->location(), sym->kind()._to_string());
//   S_Var *vs = static_cast<S_Var *>(sym);
//   vs->llvmValue() = si;
// }
//
// void VarDef::visit(Ast *ast) {
//   INITIALIZE(A_VarDef)
//   if (!ctx->scope()->owner()) {
//     visitGlobalVariable(this, ast);
//   } else {
//     visitLocalVariable(this, ast);
//   }
// }
//
// void VarDef::visitAfter(Ast *ast, Ast *child) {
//   INITIALIZE(A_VarDef)
//   if (!ctx->scope()->owner()) {
//     visitAfterGlobalVariable(this, ast, child);
//   } else {
//     visitAfterLocalVariable(this, ast, child);
//   }
// }
//
// void VarDef::finishVisit(Ast *ast) {
//   INITIALIZE(A_VarDef)
//   if (!ctx->scope()->owner()) {
//     finishVisitGlobalVariable(this, ast);
//   } else {
//     finishVisitLocalVariable(this, ast);
//   }
// }
//
// struct VISITOR(VarDef) : public Visitor {
//   Ir *ir;
//
//   VISITOR(VarDef)
//   () : Visitor("IrBuilder" BOOST_PP_STRINGIZE(VISITOR(VarDef))), ir(nullptr)
//   {}
//
//   virtual void visit(Ast *ast) {
//     INITIALIZE(A_VarDef)
//     S_Var *s_var = static_cast<S_Var *>(node->symbol());
//     if (!s_var->owner()->owner()) {
//       ir = new I_GlobalVar();
//     }
//   }
//
//   virtual void finishVisit(Ast *ast) { INITIALIZE(A_VarDef) }
// };
//
// // VarDefVisitor {
//
// IrBuilder::VISITOR(VarDef)::VISITOR(VarDef)()
//     : Visitor("IrBuilder::" BOOST_PP_STRINGIZE(VISITOR(VarDef))), ir(nullptr)
//     {}
//
// void IrBuilder::VISITOR(VarDef)::visitAfter(Ast *ast, Ast *child) {
//   INITIALIZE(A_VarDef)
//   if (node->id == child) {
//     IrBuilder::VISITOR(VarId) *v = visitor(+AstKind::VarId);
//     if (!v->symbol->owner()->owner()) {
//       // global variable
//       ir = new I_GlobalVar();
//     } else {
//       // local variable
//       ir = new I_Stat();
//     }
//   } else if (node->type == child) {
//
//   } else if (node->expr == child) {
//   }
// }
//
// void IrBuilder::VISITOR(VarDef)::finishVisit(Ast *ast) {}
//
// // VarDefVisitor }
//
// // VarIdVisitor {
//
// IrBuilder::VISITOR(VarId)::VISITOR(VarId)()
//     : Visitor("IrBuilder::" BOOST_PP_STRINGIZE(VISITOR(VarId))),
//               symbol(nullptr) {}
//
// void IrBuilder::VISITOR(VarId)::visit(Ast *ast) {
//   INITIALIZE(A_VarId)
//   LOG_ASSERT(node->symbol()->kind() == (+SymbolKind::Var),
//              "symbol {}:{} kind {} != SymbolKind::Var", node->symbol());
//   symbol = node->symbol();
// }
//
// // VarIdVisitor }
//
// // PlainTypeVisitor {
//
// IrBuilder::VISITOR(PlainType)::VISITOR(PlainType)()
//     : Visitor("IrBuilder::" BOOST_PP_STRINGIZE(VISITOR(PlainType))),
//               typeSymbol(nullptr) {}
//
// void IrBuilder::VISITOR(PlainType)::visit(Ast *ast) {
//   INITIALIZE(A_PlainType)
//   typeSymbol = ctx->scope()->ts_resolve(node->name());
//   LOG_ASSERT(typeSymbol, "plain type {}:{} cannot resolve in scope {}:{}",
//              node->name(), node->location(), ctx->scope()->name(),
//              ctx->scope()->location());
// }
//
// // PlainTypeVisitor }
//
// // CompileUnitVisitor {
//
// IrBuilder::VISITOR(CompileUnit)::VISITOR(CompileUnit)()
//     : Visitor("IrBuilder::" BOOST_PP_STRINGIZE(VISITOR(CompileUnit))),
//               ir(nullptr) {}
//
// void IrBuilder::VISITOR(CompileUnit)::visit(Ast *ast) {
//   INITIALIZE(A_CompileUnit)
//   ctx->scope() = node->scope();
//   ir = new I_Mod();
//   ib->ir() = ir;
// }
//
// void IrBuilder::VISITOR(CompileUnit)::finishVisit(Ast *ast) {
//   INITIALIZE(A_CompileUnit)
//   ctx->scope() = ctx->scope()->owner();
// }
//
// // CompileUnitVisitor }

// Data {

IrSpace::Data IrSpace::Data::fromUnknown() {
  Data data;
  data.kind = IrSpace::Data::UNKNOWN;
  return data;
}

IrSpace::Data IrSpace::Data::fromValue(llvm::Value *a_value) {
  Data data;
  data.kind = IrSpace::Data::VALUE;
  data.data.value = a_value;
  return data;
}

IrSpace::Data IrSpace::Data::fromType(llvm::Type *a_type) {
  Data data;
  data.kind = IrSpace::Data::TYPE;
  data.data.type = a_type;
  return data;
}

IrSpace::Data IrSpace::Data::fromConstant(llvm::Constant *a_constant) {
  Data data;
  data.kind = IrSpace::Data::CONSTANT;
  data.data.constant = a_constant;
  return data;
}

IrSpace::Data IrSpace::Data::fromFunction(llvm::Function *a_function) {
  Data data;
  data.kind = IrSpace::Data::FUNCTION;
  data.data.function = a_function;
  return data;
}

llvm::Value *IrSpace::Data::asValue() const {
  LOG_ASSERT(kind == +IrSpace::Data::VALUE, "kind {} != DataKind::VALUE", kind);
  return data.value;
}

llvm::Type *IrSpace::Data::asType() const {
  LOG_ASSERT(kind == +IrSpace::Data::TYPE, "kind {} != DataKind::TYPE", kind);
  return data.type;
}

llvm::Constant *IrSpace::Data::asConstant() const {
  LOG_ASSERT(kind == +IrSpace::Data::CONSTANT, "kind {} != DataKind::CONSTANT",
             kind);
  return data.constant;
}

llvm::Function *IrSpace::Data::asFunction() const {
  LOG_ASSERT(kind == +IrSpace::Data::FUNCTION, "kind {} != DataKind::FUNCTION",
             kind);
  return data.function;
}

// Data }

// IrSpace {

IrSpace::IrSpace(IrSpace *owner) : owner_(owner) {
  if (owner_) {
    owner_->children_.push_back(this);
  }
}

IrSpace::~IrSpace() {
  for (int i = 0; i < (int)children_.size(); i++) {
    delete children_[i];
    children_[i] = nullptr;
  }
  children_.clear();
}

IrSpace *IrSpace::owner() const { return owner_; }

void IrSpace::defineValue(const Cowstr &name, llvm::Value *value) {
  dataMap_.insert(name, Data::fromValue(value));
}

void IrSpace::defineType(const Cowstr &name, llvm::Type *type) {
  dataMap_.insert(name, Data::fromType(type));
}

void IrSpace::defineConstant(const Cowstr &name, llvm::Constant *constant) {
  dataMap_.insert(name, Data::fromConstant(constant));
}

void IrSpace::defineFunction(const Cowstr &name, llvm::Function *function) {
  dataMap_.insert(name, Data::fromFunction(function));
}

llvm::Value *IrSpace::resolveValue(const Cowstr &name) const {
  auto it = dataMap_.find(name);
  if (it != dataMap_.end()) {
    return it->second.asValue();
  }
  return owner() ? owner()->resolveValue(name) : nullptr;
}

llvm::Type *IrSpace::resolveType(const Cowstr &name) const {
  auto it = dataMap_.find(name);
  if (it != dataMap_.end()) {
    return it->second.asType();
  }
  return owner() ? owner()->resolveType(name) : nullptr;
}

llvm::Constant *IrSpace::resolveConstant(const Cowstr &name) const {
  auto it = dataMap_.find(name);
  if (it != dataMap_.end()) {
    return it->second.asConstant();
  }
  return owner() ? owner()->resolveConstant(name) : nullptr;
}

llvm::Function *IrSpace::resolveFunction(const Cowstr &name) const {
  auto it = dataMap_.find(name);
  if (it != dataMap_.end()) {
    return it->second.asFunction();
  }
  return owner() ? owner()->resolveFunction(name) : nullptr;
}

// IrSpace }

// IrBuilder {

static Cowstr label(Ast *ast) {
  return fmt::format("{}.{}", ast->name(), ast->location());
}

IrBuilder::IrBuilder()
    : Phase("IrBuilder"), llvmContext_(), llvmIRBuilder_(llvmContext_),
      llvmModule_(nullptr), currentScope_(nullptr), globalSpace_(nullptr),
      currentSpace_(nullptr) {}

IrBuilder::~IrBuilder() {
  delete llvmModule_;
  delete globalSpace_;
}

void IrBuilder::run(Ast *ast) { ast->accept(this); }

llvm::Module *IrBuilder::llvmModule() const { return llvmModule_; }

void IrBuilder::visitInteger(A_Integer *ast) {
  switch (ast->bit()) {
  case 32: {
    llvm::APInt ap = ast->isSigned() ? llvm::APInt(32, ast->asInt32(), true)
                                     : llvm::APInt(ast->asUInt32(), false);
    llvm::ConstantInt *ci = llvm::ConstantInt::get(llvmContext_, ap);
    currentSpace_->defineConstant(label(ast),
                                  llvm::dyn_cast<llvm::Constant>(ci));
    break;
  }
  case 64: {
    llvm::APInt ap = ast->isSigned() ? llvm::APInt(64, ast->asInt64(), true)
                                     : llvm::APInt(64, ast->asUInt64(), false);
    llvm::ConstantInt *ci = llvm::ConstantInt::get(llvmContext_, ap);
    currentSpace_->defineConstant(label(ast),
                                  llvm::dyn_cast<llvm::Constant>(ci));
    break;
  }
  default:
    LOG_ASSERT(false, "invalid bit {} in ast {}:{}", ast->bit(), ast->name(),
               ast->location());
  }
}

void IrBuilder::visitFloat(A_Float *ast) {
  switch (ast->bit()) {
  case 32: {
    llvm::APFloat ap = llvm::APFloat(ast->asFloat());
    llvm::ConstantFP *cf = llvm::ConstantFP::get(llvmContext_, ap);
    currentSpace_->defineConstant(label(ast),
                                  llvm::dyn_cast<llvm::Constant>(cf));
    break;
  }
  case 64: {
    llvm::APFloat ap = llvm::APFloat(ast->asDouble());
    llvm::ConstantFP *cf = llvm::ConstantFP::get(llvmContext_, ap);
    currentSpace_->defineConstant(label(ast),
                                  llvm::dyn_cast<llvm::Constant>(cf));
    break;
  }
  default:
    LOG_ASSERT(false, "invalid float ast {}:{}", ast->name(), ast->location());
  }
}

void IrBuilder::visitBoolean(A_Boolean *ast) {
  llvm::Constant *constant = ast->asBoolean()
                                 ? llvm::ConstantInt::getTrue(llvmContext_)
                                 : llvm::ConstantInt::getFalse(llvmContext_);
  currentSpace_->defineConstant(label(ast), constant);
}

void IrBuilder::visitCharacter(A_Character *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::visitString(A_String *ast) {
  // llvmValue_ = llvmIRBuilder_.CreateGlobalString(ast->asString().str());
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::visitNil(A_Nil *ast) { LOG_ASSERT(false, "not implemented"); }

void IrBuilder::visitVoid(A_Void *ast) { LOG_ASSERT(false, "not implemented"); }

void IrBuilder::visitVarId(A_VarId *ast) {
  LOG_ASSERT(currentSpace_->resolveValue(label(ast)),
             "ast {}:{} LLVMValue not exist", ast->name(), ast->location());
}

void IrBuilder::visitReturn(A_Return *ast) {
  if (ast->expr) {
    ast->expr->accept(this);
    llvm::Value *retValue = currentSpace_->resolveValue(label(ast->expr));
    llvmIRBuilder_.CreateRet(retValue);
  } else {
    llvmIRBuilder_.CreateRetVoid();
  }
}

void IrBuilder::visitAssign(A_Assign *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::visitPostfix(A_Postfix *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::visitInfix(A_Infix *ast) {
  ast->left->accept(this);
  llvm::Value *a = currentSpace_->resolveValue(label(ast->left));
  ast->right->accept(this);
  llvm::Value *b = currentSpace_->resolveValue(label(ast->right));
  llvm::Value *v = nullptr;

  switch (ast->infixOp) {
  case T_PLUS: { // +
    v = llvmIRBuilder_.CreateAdd(a, b, "add");
    break;
  }
  case T_MINUS: { // -
    v = llvmIRBuilder_.CreateSub(a, b, "sub");
    break;
  }
  case T_ASTERISK: { // *
    v = llvmIRBuilder_.CreateMul(a, b, "mul");
    break;
  }
  case T_SLASH: { // /
    v = llvmIRBuilder_.CreateSDiv(a, b, "div");
    break;
  }
  case T_PERCENT: { // %
    v = llvmIRBuilder_.CreateSRem(a, b, "mod");
    break;
  }
  case T_BAR2:
  case T_OR: { // || or
    v = llvmIRBuilder_.CreateOr(a, b, "or");
    break;
  }
  case T_AMPERSAND2:
  case T_AND: { // && and
    v = llvmIRBuilder_.CreateAnd(a, b, "and");
    break;
  }
  case T_BAR: { // |
    v = llvmIRBuilder_.CreateOr(a, b, "bitor");
    break;
  }
  case T_AMPERSAND: { // &
    v = llvmIRBuilder_.CreateAnd(a, b, "bitand");
    break;
  }
  case T_CARET: { // ^
    v = llvmIRBuilder_.CreateXor(a, b, "xor");
    break;
  }
  case T_EQ: { // ==
    v = llvmIRBuilder_.CreateICmpEQ(a, b, "eq");
    break;
  }
  case T_NEQ: { // !=
    v = llvmIRBuilder_.CreateICmpNE(a, b, "ne");
    break;
  }
  case T_LT: { // <
    v = llvmIRBuilder_.CreateICmpSLT(a, b, "lt");
    break;
  }
  case T_LE: { // <=
    v = llvmIRBuilder_.CreateICmpSLE(a, b, "le");
    break;
  }
  case T_GT: { // >
    v = llvmIRBuilder_.CreateICmpSGT(a, b, "gt");
    break;
  }
  case T_GE: { // >=
    v = llvmIRBuilder_.CreateICmpSGE(a, b, "ge");
    break;
  }
  // case T_LSHIFT:
  // case T_RSHIFT:
  // case T_ARSHIFT:
  default:
    LOG_ASSERT(false, "invalid infixOp {} in ast {}:{}",
               tokenName(ast->infixOp), ast->name(), ast->location());
  }
  currentSpace_->defineValue(label(ast), v);
}

void IrBuilder::visitPrefix(A_Prefix *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::visitCall(A_Call *ast) { LOG_ASSERT(false, "not implemented"); }

void IrBuilder::visitBlock(A_Block *ast) {
  if (ast->parent()->kind() == (+AstKind::FuncDef)) {
    llvm::Function *func = currentSpace_->resolveFunction(
        label(static_cast<A_FuncDef *>(ast->parent())->getId()));
    // entry block of function
    llvm::BasicBlock *entryBlock =
        llvm::BasicBlock::Create(llvmContext_, "entry", func);
    llvmIRBuilder_.SetInsertPoint(entryBlock);
    if (ast->blockStats) {
      ast->blockStats->accept(this);
    }
  } else {
    LOG_ASSERT(false, "not implemented");
  }
}

void IrBuilder::visitPlainType(A_PlainType *ast) {
  TypeSymbol *ts = currentScope_->ts_resolve(ast->name());
  LOG_ASSERT(ts->kind() == +TypeSymbolKind::Plain,
             "ts kind {} != TypeSymbolKind::Plain", ts->kind()._to_string());
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
  currentSpace_->defineType(label(ast), ty);
}

void IrBuilder::visitFuncDef(A_FuncDef *ast) {
  Ast *funcId = ast->getId();
  std::vector<std::pair<Ast *, Ast *>> funcArgs = ast->getArguments();

  std::vector<llvm::Type *> funcArgTypes;
  for (int i = 0; i < (int)funcArgs.size(); i++) {
    funcArgTypes.push_back(
        currentSpace_->resolveType(label(funcArgs[i].second)));
  }

  ast->resultType->accept(this);
  llvm::Type *funcResultType =
      currentSpace_->resolveType(label(ast->resultType));

  llvm::FunctionType *funcType =
      llvm::FunctionType::get(funcResultType, funcArgTypes, false);
  llvm::Function *func =
      llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,
                             label(funcId).str(), llvmModule_);
  currentSpace_->defineFunction(label(funcId), func);

  // set function as new space
  currentSpace_ = new IrSpace(currentSpace_);

  int i = 0;
  for (llvm::Function::arg_iterator it = func->args().begin();
       it != func->args().end(); it++, i++) {
    llvm::Argument *arg = it;
    arg->setName(label(funcArgs[i].first).str());
    currentSpace_->defineValue(label(funcArgs[i].first), arg);
  }

  ast->body->accept(this);

  currentSpace_ = currentSpace_->owner();
}

void IrBuilder::visitVarDef(A_VarDef *ast) {
  Ast *varId = ast->id;

  ast->type->accept(this);
  llvm::Type *ty_var = currentSpace_->resolveType(label(ast->type));

  // global variable
  if (ast->parent()->kind() == (+AstKind::TopStats) ||
      ast->parent()->kind() == (+AstKind::CompileUnit)) {
    IrBuilder::ConstantBuilder cb(this);
    ast->expr->accept(&cb);
    llvm::Constant *globalInitializer =
        currentSpace_->resolveConstant(label(ast->expr));
    llvm::GlobalVariable *gv = new llvm::GlobalVariable(
        *llvmModule_, ty_var, false, llvm::GlobalValue::ExternalLinkage,
        globalInitializer, label(varId).str(), nullptr,
        llvm::GlobalValue::NotThreadLocal, 0, false);
    currentSpace_->defineValue(label(varId), llvm::dyn_cast<llvm::Value>(gv));
  } else if (ast->parent()->kind() == (+AstKind::BlockStats)) {
    // local variable
    ast->expr->accept(this);
    llvm::Value *localInitializer =
        currentSpace_->resolveValue(label(ast->expr));
    llvm::AllocaInst *ai =
        llvmIRBuilder_.CreateAlloca(ty_var, nullptr, label(varId).str());
    llvm::StoreInst *si =
        llvmIRBuilder_.CreateStore(localInitializer, ai, false);
    currentSpace_->defineValue(label(varId), llvm::dyn_cast<llvm::Value>(si));
  }
}

void IrBuilder::visitCompileUnit(A_CompileUnit *ast) {
  llvmModule_ = new llvm::Module(ast->name().str(), llvmContext_);
  currentScope_ = ast->scope();
  globalSpace_ = currentSpace_ = new IrSpace();

  if (ast->topStats) {
    ast->topStats->accept(this);
  }

  currentScope_ = currentScope_->owner();
}

// IrBuilder }

// ConstantBuilder {

IrBuilder::ConstantBuilder::ConstantBuilder(IrBuilder *a_irBuilder)
    : irBuilder(a_irBuilder) {}

void IrBuilder::ConstantBuilder::visitInteger(A_Integer *ast) {
  switch (ast->bit()) {
  case 32: {
    llvm::APInt ap = ast->isSigned() ? llvm::APInt(32, ast->asInt32(), true)
                                     : llvm::APInt(ast->asUInt32(), false);
    llvm::ConstantInt *ci = llvm::ConstantInt::get(irBuilder->llvmContext_, ap);
    irBuilder->currentSpace_->defineConstant(
        label(ast), llvm::dyn_cast<llvm::Constant>(ci));
    break;
  }
  case 64: {
    llvm::APInt ap = ast->isSigned() ? llvm::APInt(64, ast->asInt64(), true)
                                     : llvm::APInt(64, ast->asUInt64(), false);
    llvm::ConstantInt *ci = llvm::ConstantInt::get(irBuilder->llvmContext_, ap);
    irBuilder->currentSpace_->defineConstant(
        label(ast), llvm::dyn_cast<llvm::Constant>(ci));
    break;
  }
  default:
    LOG_ASSERT(false, "invalid bit {} in ast {}:{}", ast->bit(), ast->name(),
               ast->location());
  }
}

void IrBuilder::ConstantBuilder::visitFloat(A_Float *ast) {
  switch (ast->bit()) {
  case 32: {
    llvm::APFloat ap = llvm::APFloat(ast->asFloat());
    llvm::ConstantFP *cf = llvm::ConstantFP::get(irBuilder->llvmContext_, ap);
    irBuilder->currentSpace_->defineConstant(
        label(ast), llvm::dyn_cast<llvm::Constant>(cf));
    break;
  }
  case 64: {
    llvm::APFloat ap = llvm::APFloat(ast->asDouble());
    llvm::ConstantFP *cf = llvm::ConstantFP::get(irBuilder->llvmContext_, ap);
    irBuilder->currentSpace_->defineConstant(
        label(ast), llvm::dyn_cast<llvm::Constant>(cf));
    break;
  }
  default:
    LOG_ASSERT(false, "invalid float ast {}:{}", ast->name(), ast->location());
  }
}

void IrBuilder::ConstantBuilder::visitBoolean(A_Boolean *ast) {
  llvm::Constant *cb =
      ast->asBoolean() ? llvm::ConstantInt::getTrue(irBuilder->llvmContext_)
                       : llvm::ConstantInt::getFalse(irBuilder->llvmContext_);
  irBuilder->currentSpace_->defineConstant(label(ast), cb);
}

void IrBuilder::ConstantBuilder::visitCharacter(A_Character *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::ConstantBuilder::visitString(A_String *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::ConstantBuilder::visitNil(A_Nil *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::ConstantBuilder::visitVoid(A_Void *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::ConstantBuilder::visitVarId(A_VarId *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::ConstantBuilder::visitAssign(A_Assign *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::ConstantBuilder::visitPostfix(A_Postfix *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::ConstantBuilder::visitInfix(A_Infix *ast) {
  ast->left->accept(this);
  llvm::Constant *a =
      irBuilder->currentSpace_->resolveConstant(label(ast->left));
  ast->right->accept(this);
  llvm::Constant *b =
      irBuilder->currentSpace_->resolveConstant(label(ast->right));
  switch (ast->infixOp) {
  case T_PLUS: { // +
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast), llvm::ConstantExpr::getAdd(a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast), llvm::ConstantExpr::getFAdd(a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_MINUS: { // -
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast), llvm::ConstantExpr::getSub(a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast), llvm::ConstantExpr::getFSub(a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_ASTERISK: { // *
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast), llvm::ConstantExpr::getMul(a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast), llvm::ConstantExpr::getFMul(a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_SLASH: { // /
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast), llvm::ConstantExpr::getSDiv(a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast), llvm::ConstantExpr::getFDiv(a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_PERCENT: { // %
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast), llvm::ConstantExpr::getSRem(a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast), llvm::ConstantExpr::getFRem(a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_BAR2:
  case T_OR: { // || or
    // a and b must be 1-bit unsigned true/false value
    LOG_ASSERT(llvm::isa<llvm::ConstantInt>(a), "a must be ConstantInt:{}",
               Cowstr::from(a));
    LOG_ASSERT(llvm::dyn_cast<llvm::ConstantInt>(a)->getBitWidth() == 1,
               "a bitWidth != 1:{}", Cowstr::from(a));
    LOG_ASSERT(llvm::isa<llvm::ConstantInt>(b), "b must be ConstantInt:{}",
               Cowstr::from(b));
    LOG_ASSERT(llvm::dyn_cast<llvm::ConstantInt>(b)->getBitWidth() == 1,
               "b bitWidth != 1:{}", Cowstr::from(b));
    irBuilder->currentSpace_->defineConstant(label(ast),
                                             llvm::ConstantExpr::getOr(a, b));
    break;
  }
  case T_AMPERSAND2:
  case T_AND: { // && and
    // a and b must be 1-bit unsigned true/false value
    LOG_ASSERT(llvm::isa<llvm::ConstantInt>(a), "a must be ConstantInt:{}",
               Cowstr::from(a));
    LOG_ASSERT(llvm::dyn_cast<llvm::ConstantInt>(a)->getBitWidth() == 1,
               "a bitWidth != 1:{}", Cowstr::from(a));
    LOG_ASSERT(llvm::isa<llvm::ConstantInt>(b), "b must be ConstantInt:{}",
               Cowstr::from(b));
    LOG_ASSERT(llvm::dyn_cast<llvm::ConstantInt>(b)->getBitWidth() == 1,
               "b bitWidth != 1:{}", Cowstr::from(b));
    irBuilder->currentSpace_->defineConstant(label(ast),
                                             llvm::ConstantExpr::getAnd(a, b));
    break;
  }
  case T_BAR: { // |
    irBuilder->currentSpace_->defineConstant(label(ast),
                                             llvm::ConstantExpr::getOr(a, b));
    break;
  }
  case T_AMPERSAND: { // &
    irBuilder->currentSpace_->defineConstant(label(ast),
                                             llvm::ConstantExpr::getAnd(a, b));
    break;
  }
  case T_CARET: { // ^
    irBuilder->currentSpace_->defineConstant(label(ast),
                                             llvm::ConstantExpr::getXor(a, b));
    break;
  }
  case T_EQ: { // ==
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_EQ, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::FCMP_OEQ, a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_NEQ: { // !=
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_NE, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::FCMP_ONE, a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_LT: { // <
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_SLT, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::FCMP_OLT, a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_LE: { // <=
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_SLE, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::FCMP_OLE, a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_GT: { // >
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_SGT, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::FCMP_OGT, a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_GE: { // >=
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_SGE, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->currentSpace_->defineConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::FCMP_OGE, a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  // case T_LSHIFT:
  // case T_RSHIFT:
  // case T_ARSHIFT:
  default:
    LOG_ASSERT(false, "invalid infixOp {} in ast {}:{}",
               tokenName(ast->infixOp), ast->name(), ast->location());
  }
}

void IrBuilder::ConstantBuilder::visitPrefix(A_Prefix *ast) {
  ast->expr->accept(this);
  llvm::Constant *a =
      irBuilder->currentSpace_->resolveConstant(label(ast->expr));
  switch (ast->prefixOp) {
  case T_PLUS: { // +
    // do nothing
    break;
  }
  case T_MINUS: { // -
    if (llvm::isa<llvm::ConstantInt>(a)) {
      irBuilder->currentSpace_->defineConstant(label(ast),
                                               llvm::ConstantExpr::getNeg(a));
    } else if (llvm::isa<llvm::ConstantFP>(a)) {
      irBuilder->currentSpace_->defineConstant(label(ast),
                                               llvm::ConstantExpr::getFNeg(a));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_TILDE: { // ~
    irBuilder->currentSpace_->defineConstant(label(ast),
                                             llvm::ConstantExpr::getNeg(a));
    break;
  }
  case T_EXCLAM:
  case T_NOT: { // ! not
    irBuilder->currentSpace_->defineConstant(label(ast),
                                             llvm::ConstantExpr::getNeg(a));
    break;
  }
  default:
    LOG_ASSERT(false, "invalid prefixOp {} in ast {}:{}",
               tokenName(ast->prefixOp), ast->name(), ast->location());
  }
}

// ConstantBuilder }
