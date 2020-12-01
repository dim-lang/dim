// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "IrBuilder.h"
#include "Ast.h"
#include "Symbol.h"
#include "Token.h"
#include "boost/preprocessor/stringize.hpp"
#include "infra/LinkedHashMap.hpp"
#include "infra/Log.h"
#include "llvm/Support/Casting.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

namespace detail {

// SpaceData {

SpaceData SpaceData::fromValue(llvm::Value *a_value) {
  SpaceData data;
  data.kind = SpaceData::VALUE;
  data.data.value = a_value;
  return data;
}

SpaceData SpaceData::fromType(llvm::Type *a_type) {
  SpaceData data;
  data.kind = SpaceData::TYPE;
  data.data.type = a_type;
  return data;
}

SpaceData SpaceData::fromConstant(llvm::Constant *a_constant) {
  SpaceData data;
  data.kind = SpaceData::CONSTANT;
  data.data.constant = a_constant;
  return data;
}

SpaceData SpaceData::fromFunction(llvm::Function *a_function) {
  SpaceData data;
  data.kind = SpaceData::FUNCTION;
  data.data.function = a_function;
  return data;
}

llvm::Value *SpaceData::asValue() const {
  LOG_ASSERT(kind == +SpaceData::VALUE || kind == +SpaceData::CONSTANT,
             "kind {} != DataKind::VALUE or CONSTANT:{}", kind, str());
  return (kind == +SpaceData::VALUE) ? data.value : data.constant;
}

llvm::Type *SpaceData::asType() const {
  LOG_ASSERT(kind == +SpaceData::TYPE, "kind {} != DataKind::TYPE:{}", kind,
             str());
  return data.type;
}

llvm::Constant *SpaceData::asConstant() const {
  LOG_ASSERT(kind == +SpaceData::CONSTANT, "kind {} != DataKind::CONSTANT:{}",
             kind, str());
  return data.constant;
}

llvm::Function *SpaceData::asFunction() const {
  LOG_ASSERT(kind == +SpaceData::FUNCTION, "kind {} != DataKind::FUNCTION:{}",
             kind, str());
  return data.function;
}

Cowstr SpaceData::str() const {
  switch (kind) {
  case VALUE:
    return Cowstr::from(asValue());
  case TYPE:
    return Cowstr::from(asType());
  case CONSTANT:
    return Cowstr::from(asConstant());
  case FUNCTION:
    return Cowstr::from(asFunction());
  default:
    LOG_ASSERT(false, "invalid kind:{}", kind);
  }
  return "";
}

// SpaceData }

// Space {

Space::Space() {}

void Space::setValue(const Cowstr &name, llvm::Value *value) {
  LOG_ASSERT(!dataMap_.contains(name), "value {} already exist", name);
  dataMap_.insert(name, SpaceData::fromValue(value));
}

void Space::setType(const Cowstr &name, llvm::Type *type) {
  LOG_ASSERT(!dataMap_.contains(name), "type {} already exist", name);
  dataMap_.insert(name, SpaceData::fromType(type));
}

void Space::setConstant(const Cowstr &name, llvm::Constant *constant) {
  LOG_ASSERT(!dataMap_.contains(name), "constant {} already exist", name);
  dataMap_.insert(name, SpaceData::fromConstant(constant));
}

void Space::setFunction(const Cowstr &name, llvm::Function *function) {
  LOG_ASSERT(!dataMap_.contains(name), "function {} already exist", name);
  dataMap_.insert(name, SpaceData::fromFunction(function));
}

llvm::Value *Space::getValue(const Cowstr &name) const {
  auto it = dataMap_.find(name);
  return it == dataMap_.end() ? nullptr : it->second.asValue();
}

llvm::Type *Space::getType(const Cowstr &name) const {
  auto it = dataMap_.find(name);
  return it == dataMap_.end() ? nullptr : it->second.asType();
}

llvm::Constant *Space::getConstant(const Cowstr &name) const {
  auto it = dataMap_.find(name);
  return it == dataMap_.end() ? nullptr : it->second.asConstant();
}

llvm::Function *Space::getFunction(const Cowstr &name) const {
  auto it = dataMap_.find(name);
  return it == dataMap_.end() ? nullptr : it->second.asFunction();
}

// Space }

} // namespace detail

// IrBuilder {

static Cowstr label(Ast *ast) {
  return fmt::format("{}.{}_{}_{}_{}", ast->name(), ast->location().begin.line,
                     ast->location().begin.column, ast->location().end.line,
                     ast->location().end.column);
}

static Cowstr label(Symbol *symbol) {
  return fmt::format(
      "{}.{}_{}_{}_{}", symbol->name(), symbol->location().begin.line,
      symbol->location().begin.column, symbol->location().end.line,
      symbol->location().end.column);
}

static Cowstr label(TypeSymbol *typeSymbol) {
  return fmt::format(
      "{}.{}_{}_{}_{}", typeSymbol->name(), typeSymbol->location().begin.line,
      typeSymbol->location().begin.column, typeSymbol->location().end.line,
      typeSymbol->location().end.column);
}

IrBuilder::IrBuilder(bool enableFunctionPass)
    : Phase("IrBuilder"), llvmContext_(), llvmIRBuilder_(llvmContext_),
      llvmModule_(nullptr), enableFunctionPass_(enableFunctionPass),
      llvmFunctionPassManager_(nullptr), scope_(nullptr) {}

IrBuilder::~IrBuilder() {
  delete llvmModule_;
  delete llvmFunctionPassManager_;
}

void IrBuilder::run(Ast *ast) { ast->accept(this); }

llvm::Module *IrBuilder::llvmModule() const { return llvmModule_; }

void IrBuilder::visitInteger(A_Integer *ast) {
  switch (ast->bit()) {
  case 32: {
    llvm::APInt ap = ast->isSigned() ? llvm::APInt(32, ast->asInt32(), true)
                                     : llvm::APInt(ast->asUInt32(), false);
    llvm::ConstantInt *ci = llvm::ConstantInt::get(llvmContext_, ap);
    space_.setConstant(label(ast), llvm::dyn_cast<llvm::Constant>(ci));
    break;
  }
  case 64: {
    llvm::APInt ap = ast->isSigned() ? llvm::APInt(64, ast->asInt64(), true)
                                     : llvm::APInt(64, ast->asUInt64(), false);
    llvm::ConstantInt *ci = llvm::ConstantInt::get(llvmContext_, ap);
    space_.setConstant(label(ast), llvm::dyn_cast<llvm::Constant>(ci));
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
    space_.setConstant(label(ast), llvm::dyn_cast<llvm::Constant>(cf));
    break;
  }
  case 64: {
    llvm::APFloat ap = llvm::APFloat(ast->asDouble());
    llvm::ConstantFP *cf = llvm::ConstantFP::get(llvmContext_, ap);
    space_.setConstant(label(ast), llvm::dyn_cast<llvm::Constant>(cf));
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
  space_.setConstant(label(ast), constant);
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
  if (ast->symbol()) {
    llvm::Value *value = space_.getValue(label(ast->symbol()));
    LOG_ASSERT(value, "ast {}:{} symbol {}:{} does not has llvm value",
               ast->name(), ast->location(), ast->symbol()->name(),
               ast->symbol()->location());
    llvm::LoadInst *li = llvmIRBuilder_.CreateLoad(value);
    space_.setValue(label(ast), llvm::dyn_cast<llvm::Value>(li));
  } else if (ast->typeSymbol()) {
    llvm::Type *type = space_.getType(label(ast->typeSymbol()));
    LOG_ASSERT(type, "ast {}:{} type symbol {}:{} does not has llvm type",
               ast->name(), ast->location(), ast->typeSymbol()->name(),
               ast->typeSymbol()->location());
    space_.setType(label(ast), type);
  }
}

void IrBuilder::visitReturn(A_Return *ast) {
  if (ast->expr) {
    ast->expr->accept(this);
    llvm::Value *retValue = space_.getValue(label(ast->expr));
    LOG_ASSERT(retValue, "ast {}:{} ast->expr {}:{} retValue:{} must not null",
               ast->name(), ast->location(), ast->expr->name(),
               ast->expr->location(), Cowstr::from(retValue));
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
  llvm::Value *a = space_.getValue(label(ast->left));
  ast->right->accept(this);
  llvm::Value *b = space_.getValue(label(ast->right));
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
  space_.setValue(label(ast), v);
}

void IrBuilder::visitPrefix(A_Prefix *ast) {
  LOG_ASSERT(false, "not implemented");
}

void IrBuilder::visitCall(A_Call *ast) { LOG_ASSERT(false, "not implemented"); }

void IrBuilder::visitBlock(A_Block *ast) {
  if (ast->parent()->kind() == (+AstKind::FuncDef)) {
    A_FuncDef *funcDef = static_cast<A_FuncDef *>(ast->parent());
    A_FuncSign *funcSign = static_cast<A_FuncSign *>(funcDef->funcSign);
    A_VarId *funcId = static_cast<A_VarId *>(funcSign->id);
    llvm::Function *func = space_.getFunction(label(funcId->symbol()));
    // entry block of function
    llvm::BasicBlock *entryBlock =
        llvm::BasicBlock::Create(llvmContext_, "entry", func);
    llvmIRBuilder_.SetInsertPoint(entryBlock);
    if (ast->blockStats) {
      ast->blockStats->accept(this);
    }
    // insert `return void` if there's no return instruction in function
    bool hasReturn = false;
    for (llvm::Function::const_iterator i = func->getBasicBlockList().begin();
         i != func->getBasicBlockList().end() && !hasReturn; ++i) {
      for (llvm::BasicBlock::const_iterator j = i->getInstList().begin();
           j != i->getInstList().end() && !hasReturn; ++j) {
        const llvm::Instruction *inst = &(*j);
        // const llvm::Instruction *inst = j;
        if (llvm::ReturnInst::classof(inst)) {
          hasReturn = true;
        }
      }
    }
    // llvm::BasicBlock *lastBlock = &(*func->getBasicBlockList().rbegin());
    if (!hasReturn) {
      llvmIRBuilder_.CreateRetVoid();
    }
  } else {
    LOG_ASSERT(false, "not implemented");
  }
}

void IrBuilder::visitPlainType(A_PlainType *ast) {
  TypeSymbol *ts = scope_->ts_resolve(ast->name());
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
  space_.setType(label(ast), ty);
}

void IrBuilder::visitFuncDef(A_FuncDef *ast) {
  A_VarId *funcId = static_cast<A_VarId *>(ast->getId());
  std::vector<std::pair<Ast *, Ast *>> funcArgs = ast->getArguments();

  std::vector<llvm::Type *> funcArgTypes;
  for (int i = 0; i < (int)funcArgs.size(); ++i) {
    funcArgTypes.push_back(space_.getType(label(funcArgs[i].second)));
  }

  ast->resultType->accept(this);
  llvm::Type *funcResultType = space_.getType(label(ast->resultType));

  llvm::FunctionType *funcType =
      llvm::FunctionType::get(funcResultType, funcArgTypes, false);
  llvm::Function *func =
      llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,
                             label(funcId->symbol()).str(), llvmModule_);
  space_.setFunction(label(funcId->symbol()), func);
  // space_.setFunction(label(funcId), func);

  int i = 0;
  for (llvm::Function::arg_iterator it = func->args().begin();
       it != func->args().end(); ++it, ++i) {
    llvm::Argument *arg = it;
    arg->setName(label(funcArgs[i].first).str());
    space_.setValue(label(static_cast<A_VarId *>(funcArgs[i].first)), arg);
    // space_.setValue(label(funcArgs[i].first), arg);
  }

  ast->body->accept(this);

  if (enableFunctionPass_) {
    llvmFunctionPassManager_->run(*func);
  }
}

void IrBuilder::visitVarDef(A_VarDef *ast) {
  A_VarId *varId = static_cast<A_VarId *>(ast->id);

  ast->type->accept(this);
  llvm::Type *ty_var = space_.getType(label(ast->type));

  // global variable
  if (ast->parent()->kind() == (+AstKind::TopStats) ||
      ast->parent()->kind() == (+AstKind::CompileUnit)) {
    IrBuilder::ConstantBuilder cb(this);
    ast->expr->accept(&cb);
    llvm::Constant *gc = space_.getConstant(label(ast->expr));
    llvm::GlobalVariable *gv = new llvm::GlobalVariable(
        *llvmModule_, ty_var, false, llvm::GlobalValue::ExternalLinkage, gc,
        label(varId->symbol()).str(), nullptr,
        llvm::GlobalValue::NotThreadLocal, 0, false);
    space_.setValue(label(varId->symbol()), llvm::dyn_cast<llvm::Value>(gv));
  } else if (ast->parent()->kind() == (+AstKind::BlockStats)) {
    // local variable
    ast->expr->accept(this);
    llvm::Value *ae = space_.getValue(label(ast->expr));
    llvm::AllocaInst *ai = llvmIRBuilder_.CreateAlloca(
        ty_var, nullptr, label(varId->symbol()).str());
    llvm::StoreInst *si = llvmIRBuilder_.CreateStore(ae, ai, false);
    (void)si;
    space_.setValue(label(varId->symbol()), llvm::dyn_cast<llvm::Value>(ai));
    // space_.setValue(label(varId), llvm::dyn_cast<llvm::Value>(si));
  }
}

void IrBuilder::visitCompileUnit(A_CompileUnit *ast) {
  llvmModule_ = new llvm::Module(ast->name().str(), llvmContext_);
  if (enableFunctionPass_) {
    llvmFunctionPassManager_ =
        new llvm::legacy::FunctionPassManager(llvmModule_);
    llvmFunctionPassManager_->add(llvm::createInstructionCombiningPass());
    llvmFunctionPassManager_->add(llvm::createReassociatePass());
    llvmFunctionPassManager_->add(llvm::createGVNPass());
    llvmFunctionPassManager_->add(llvm::createCFGSimplificationPass());
    llvmFunctionPassManager_->add(llvm::createPromoteMemoryToRegisterPass());
    llvmFunctionPassManager_->doInitialization();
  }

  scope_ = ast->scope();

  if (ast->topStats) {
    ast->topStats->accept(this);
  }

  scope_ = scope_->owner();
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
    irBuilder->space_.setConstant(label(ast),
                                  llvm::dyn_cast<llvm::Constant>(ci));
    break;
  }
  case 64: {
    llvm::APInt ap = ast->isSigned() ? llvm::APInt(64, ast->asInt64(), true)
                                     : llvm::APInt(64, ast->asUInt64(), false);
    llvm::ConstantInt *ci = llvm::ConstantInt::get(irBuilder->llvmContext_, ap);
    irBuilder->space_.setConstant(label(ast),
                                  llvm::dyn_cast<llvm::Constant>(ci));
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
    irBuilder->space_.setConstant(label(ast),
                                  llvm::dyn_cast<llvm::Constant>(cf));
    break;
  }
  case 64: {
    llvm::APFloat ap = llvm::APFloat(ast->asDouble());
    llvm::ConstantFP *cf = llvm::ConstantFP::get(irBuilder->llvmContext_, ap);
    irBuilder->space_.setConstant(label(ast),
                                  llvm::dyn_cast<llvm::Constant>(cf));
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
  irBuilder->space_.setConstant(label(ast), cb);
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
  llvm::Constant *a = irBuilder->space_.getConstant(label(ast->left));
  ast->right->accept(this);
  llvm::Constant *b = irBuilder->space_.getConstant(label(ast->right));
  switch (ast->infixOp) {
  case T_PLUS: { // +
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->space_.setConstant(label(ast),
                                    llvm::ConstantExpr::getAdd(a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(label(ast),
                                    llvm::ConstantExpr::getFAdd(a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_MINUS: { // -
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->space_.setConstant(label(ast),
                                    llvm::ConstantExpr::getSub(a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(label(ast),
                                    llvm::ConstantExpr::getFSub(a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_ASTERISK: { // *
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->space_.setConstant(label(ast),
                                    llvm::ConstantExpr::getMul(a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(label(ast),
                                    llvm::ConstantExpr::getFMul(a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_SLASH: { // /
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->space_.setConstant(label(ast),
                                    llvm::ConstantExpr::getSDiv(a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(label(ast),
                                    llvm::ConstantExpr::getFDiv(a, b));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_PERCENT: { // %
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->space_.setConstant(label(ast),
                                    llvm::ConstantExpr::getSRem(a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(label(ast),
                                    llvm::ConstantExpr::getFRem(a, b));
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
    irBuilder->space_.setConstant(label(ast), llvm::ConstantExpr::getOr(a, b));
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
    irBuilder->space_.setConstant(label(ast), llvm::ConstantExpr::getAnd(a, b));
    break;
  }
  case T_BAR: { // |
    irBuilder->space_.setConstant(label(ast), llvm::ConstantExpr::getOr(a, b));
    break;
  }
  case T_AMPERSAND: { // &
    irBuilder->space_.setConstant(label(ast), llvm::ConstantExpr::getAnd(a, b));
    break;
  }
  case T_CARET: { // ^
    irBuilder->space_.setConstant(label(ast), llvm::ConstantExpr::getXor(a, b));
    break;
  }
  case T_EQ: { // ==
    if (llvm::isa<llvm::ConstantInt>(a) && llvm::isa<llvm::ConstantInt>(b)) {
      irBuilder->space_.setConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_EQ, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(
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
      irBuilder->space_.setConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_NE, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(
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
      irBuilder->space_.setConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_SLT, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(
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
      irBuilder->space_.setConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_SLE, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(
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
      irBuilder->space_.setConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_SGT, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(
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
      irBuilder->space_.setConstant(
          label(ast),
          llvm::ConstantExpr::getCompare(llvm::CmpInst::ICMP_SGE, a, b));
    } else if (llvm::isa<llvm::ConstantFP>(a) &&
               llvm::isa<llvm::ConstantFP>(b)) {
      irBuilder->space_.setConstant(
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
  llvm::Constant *a = irBuilder->space_.getConstant(label(ast->expr));
  switch (ast->prefixOp) {
  case T_PLUS: { // +
    // do nothing
    break;
  }
  case T_MINUS: { // -
    if (llvm::isa<llvm::ConstantInt>(a)) {
      irBuilder->space_.setConstant(label(ast), llvm::ConstantExpr::getNeg(a));
    } else if (llvm::isa<llvm::ConstantFP>(a)) {
      irBuilder->space_.setConstant(label(ast), llvm::ConstantExpr::getFNeg(a));
    } else {
      LOG_ASSERT(false, "invalid operation for {}:{}", ast->name(),
                 ast->location());
    }
    break;
  }
  case T_TILDE: { // ~
    irBuilder->space_.setConstant(label(ast), llvm::ConstantExpr::getNeg(a));
    break;
  }
  case T_EXCLAM:
  case T_NOT: { // ! not
    irBuilder->space_.setConstant(label(ast), llvm::ConstantExpr::getNeg(a));
    break;
  }
  default:
    LOG_ASSERT(false, "invalid prefixOp {} in ast {}:{}",
               tokenName(ast->prefixOp), ast->name(), ast->location());
  }
}

// ConstantBuilder }
