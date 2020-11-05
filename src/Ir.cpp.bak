// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ir.h"
#include "Ast.h"
#include "IrUtil.h"
#include "Label.h"
#include "Log.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include <unordered_set>

static Label GlobalLabel("@nerd.global.ir", ".");
static Label LocalLabel("%nerd.local.ir", ".");

I_Module::I_Module(Ast *a_ast, std::shared_ptr<Scope> a_scope)
    : Nameable(a_ast->name()), ast(a_ast), scope(a_scope), llvmContext(),
      llvmIrBuilder(llvmContext),
      llvmModule(new llvm::Module(a_name.rawstr(), llvmContext)) {
  LOG_ASSERT(ast->kind() == (+AstKind::CompileUnit),
             "ast->kind {} != AstKind::CompileUnit", ast->kind()._to_string());
  A_CompileUnit *root = static_cast<A_CompileUnit *>(ast);
  if (!root->topStats) {
    return;
  }
  for (A_TopStats *ts = root->topStats; ts; ts = ts->next) {
    Ast *node = ts->topStat;
    switch (node->kind()) {
    case AstKind::VarDef: {
      std::shared_ptr<Ir> gvar(new I_GVarDef(this, node, scope));
      globalVariables.push_back(gvar);
      break;
    }
    case AstKind::FuncDef: {
      std::shared_ptr<Ir> func(new I_FuncDef(this, node, scope));
      functions.push_back(func);
      break;
    }
    default:
      LOG_ASSERT(false, "invalid node->kind {}", node->kind()._to_string());
    }
  }
}

I_Module::~I_Module() {
  if (llvmModule) {
    delete llvmModule;
    llvmModule = nullptr;
  }
}

llvm::Value *I_Module::value() const {
  return llvm::dyn_cast<llvm::Value>(llvmModule);
}

Cowstr I_Module::str() const {
  std::string result;
  llvm::raw_string_ostream oss(result);
  llvmModule->print(oss, nullptr);
  return result;
}

I_GVarDef::I_GVarDef(I_Module *a_iModule, Ast *a_ast,
                     std::shared_ptr<Scope> a_scope)
    : Nameable(a_ast->name()), iModule(a_iModule), ast(a_ast), scope(a_scope),
      globalVariable(nullptr), expr(nullptr) {
  LOG_ASSERT(ast->kind() == (+AstKind::VarDef),
             "ast->kind {} != AstKind::VarDef", ast->kind()._to_string());
  A_VarDef *root = static_cast<A_VarDef *>(ast);
  LOG_ASSERT(root->id->kind() == (+AstKind::VarId),
             "root->id->kind {} != AstKind::VarId",
             root->id->kind()._to_string());
  LOG_ASSERT(root->type->kind() == (+AstKind::PlainType),
             "root->type->kind {} != AstKind::PlainType",
             root->type->kind()._to_string());
  A_VarId *id = static_cast<A_VarId *>(root->id);
  A_PlainType *type = static_cast<A_PlainType *>(root->type);

  expr = Ir::expr(a_iModule, root->expr, scope);
  llvm::Type *exprType = expr->value()->getType();

  const static std::unordered_set<llvm::Type::TypeID> initializerTypeIDSet = {
      llvm::Type::IntegerTyID, llvm::Type::FloatTyID, llvm::Type::DoubleTyID};

  LOG_ASSERT(initializerTypeIDSet.find(exprType->getTypeID()) !=
                 initializerTypeIDSet.end(),
             "exprType {} == IntegerTyID|FloatTyID|DoubleTyID",
             IrUtil::stringize(exprType));

  switch (exprType->getTypeID()) {
  case llvm::Type::IntegerTyID: {
    globalVariable = getInt(GlobalLabel.encode(id),
                            llvm::cast<llvm::ConstantInt>(expr->value()));
    break;
  }
  case llvm::Type::FloatTyID:
  case llvm::Type::DoubleTyID: {
    globalVariable = getFP(GlobalLabel.encode(id),
                           llvm::cast<llvm::ConstantFP>(expr->value()));
    break;
  }
  default:
    LOG_ASSERT(false, "invalid exprType:{}", IrUtil::stringize(exprType));
  }
}

llvm::GlobalVariable *I_GVarDef::getInt(const Cowstr &name,
                                        llvm::ConstantInt *initializer) {
  llvm::GlobalVariable *var = nullptr;
  switch (initializer->getBitWidth()) {
  case 8:
    var = new llvm::GlobalVariable(
        iModule->llvmModule, iModule->llvmIrBuilder.getInt8Ty(), false,
        llvm::GlobalVariable::ExternalLinkage, initializer, name.str());
    break;
  case 16:
    var = new llvm::GlobalVariable(
        iModule->llvmModule, iModule->llvmIrBuilder.getInt16Ty(), false,
        llvm::GlobalVariable::ExternalLinkage, initializer, name.str());
    break;
  case 32:
    var = new llvm::GlobalVariable(
        iModule->llvmModule, iModule->llvmIrBuilder.getInt32Ty(), false,
        llvm::GlobalVariable::ExternalLinkage, initializer, name.str());
    break;
  case 64:
    var = new llvm::GlobalVariable(
        iModule->llvmModule, iModule->llvmIrBuilder.getInt64Ty(), false,
        llvm::GlobalVariable::ExternalLinkage, initializer, name.str());
    break;
  default:
    LOG_ASSERT(false, "invalid initializer {} getBitWidth: {}",
               IrUtil::stringize(initializer), initializer->getBitWidth());
  }
  return var;
}

llvm::GlobalVariable *I_GVarDef::getFP(const Cowstr &name,
                                       llvm::ConstantFP *initializer) {
  llvm::GlobalVariable *var = nullptr;
  switch (initializer->getType()->getTypeID()) {
  case llvm::Type::FloatTyID:
    var = new llvm::GlobalVariable(
        iModule->llvmModule, iModule->llvmIrBuilder.getFloatTy(), false,
        llvm::GlobalVariable::ExternalLinkage, initializer, name.str());
    break;
  case llvm::Type::DoubleTyID:
    var = new llvm::GlobalVariable(
        iModule->llvmModule, iModule->llvmIrBuilder.getDoubleTy(), false,
        llvm::GlobalVariable::ExternalLinkage, initializer, name.str());
    break;
  default:
    LOG_ASSERT(false, "invalid initializer:{} getTypeID:{}",
               IrUtil::stringize(initializer),
               IrUtil::stringize(initializer->getType()));
  }
  return var;
}

llvm::Value *I_GVarDef::value() const {
  return llvm::dyn_cast<llvm::Value>(globalVariable);
}

Cowstr I_GVarDef::str() const {}

I_Module *iModule;
Ast *ast;
std::shared_ptr<Scope> scope;

llvm::GlobalVariable *globalVariable;
llvm::ConstantInt *initializer;
}
;
