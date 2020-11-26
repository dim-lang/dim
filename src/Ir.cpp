// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "boost/preprocessor/stringize.hpp"

// I_Mod {

// I_Mod::GlobalVarVisitor {

I_Mod::VISITOR(GlobalVar)::VISITOR(GlobalVar)()
    : Visitor("Ir::I_Mod::" BOOST_PP_STRINGIZE(VISITOR(GlobalVar))) {}

void I_Mod::VISITOR(GlobalVar)::visit(Ast *ast) {
  I_Mod::Context *ctx = static_cast<I_Mod::Context *>(context());
  ctx->iMod->iGlobalVar->run(ast);
}

// I_Mod::GlobalVarVisitor }

// I_Mod::FuncDeclVisitor {

I_Mod::VISITOR(FuncDecl)::VISITOR(FuncDecl)()
    : Visitor("Ir::I_Mod::" BOOST_PP_STRINGIZE(VISITOR(FuncDecl))) {}

void I_Mod::VISITOR(FuncDecl)::visit(Ast *ast) {
  I_Mod::Context *ctx = static_cast<I_Mod::Context *>(context());
  ctx->iMod->iFuncDecl->run(ast);
}

// I_Mod::FuncDeclVisitor }

// I_Mod::FuncDefVisitor {

I_Mod::VISITOR(FuncDef)::VISITOR(FuncDef)()
    : Visitor("Ir::I_Mod::" BOOST_PP_STRINGIZE(VISITOR(FuncDef))) {}

void I_Mod::VISITOR(FuncDef)::visit(Ast *ast) {
  I_Mod::Context *ctx = static_cast<I_Mod::Context *>(context());
  ctx->iMod->iFuncDef->run(ast);
}

// I_Mod::FuncDefVisitor }

I_Mod::I_Mod()
    : Phase("I_Mod"),
      // traversor
      context(new I_Mod::Context(this)), binder(context),
      // other traversors
      iGlobalVar(new I_GlobalVar()), iFuncDecl(new I_FuncDecl()),
      iFuncDef(new I_FuncDef()),
      // LLVM
      llvmContext(), llvmIRBuilder(llvmContext), llvmModule(nullptr) {}

I_Mod::~I_Mod() {
  delete context;
  context = nullptr;
  for (int i = 0; i < (int)visitors.size(); i++) {
    delete visitors[i];
    visitors[i] = nullptr;
  }
  delete iGlobalVar;
  iGlobalVar = nullptr;
  delete iFuncDecl;
  iFuncDecl = nullptr;
  delete iFuncDef;
  iFuncDef = nullptr;
  delete llvmModule;
  llvmModule = nullptr;
}

// I_Mod }

// I_GlobalVar {

// I_GlobalVar }
