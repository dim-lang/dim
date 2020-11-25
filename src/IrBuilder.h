// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "AstClasses.h"
#include "SymbolClasses.h"
#include "enum.h"
#include "iface/Phase.h"
#include "iface/Visitor.h"
#include "infra/Cowstr.h"
#include "infra/LinkedHashMap.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include <unordered_set>
#include <vector>

namespace detail {

struct SpaceData {
  enum SpaceDataKind { VALUE = 0, TYPE, CONSTANT, FUNCTION };

  int kind;
  union {
    llvm::Value *value;
    llvm::Type *type;
    llvm::Constant *constant;
    llvm::Function *function;
  } data;

  virtual ~SpaceData() = default;

  static SpaceData fromValue(llvm::Value *a_value);
  static SpaceData fromType(llvm::Type *a_type);
  static SpaceData fromConstant(llvm::Constant *a_constant);
  static SpaceData fromFunction(llvm::Function *a_function);

  llvm::Value *asValue() const;
  llvm::Type *asType() const;
  llvm::Constant *asConstant() const;
  llvm::Function *asFunction() const;

  Cowstr str() const;
};

class Space {
public:
  Space();
  virtual ~Space() = default;

  void setValue(const Cowstr &name, llvm::Value *value);
  void setType(const Cowstr &name, llvm::Type *type);
  void setConstant(const Cowstr &name, llvm::Constant *constant);
  void setFunction(const Cowstr &name, llvm::Function *function);
  llvm::Value *getValue(const Cowstr &name) const;
  llvm::Type *getType(const Cowstr &name) const;
  llvm::Constant *getConstant(const Cowstr &name) const;
  llvm::Function *getFunction(const Cowstr &name) const;

private:
  LinkedHashMap<Cowstr, SpaceData> dataMap_;
};

} // namespace detail

class IrBuilder : public Phase, public Visitor {
public:
  IrBuilder();
  virtual ~IrBuilder();
  virtual void run(Ast *ast);
  virtual llvm::Module *llvmModule() const;

  virtual void visitInteger(A_Integer *ast);
  virtual void visitFloat(A_Float *ast);
  virtual void visitBoolean(A_Boolean *ast);
  virtual void visitCharacter(A_Character *ast);
  virtual void visitString(A_String *ast);
  virtual void visitNil(A_Nil *ast);
  virtual void visitVoid(A_Void *ast);
  virtual void visitVarId(A_VarId *ast);
  // virtual void visitBreak(A_Break *ast);
  // virtual void visitContinue(A_Continue *ast);

  // virtual void visitThrow(A_Throw *ast);
  virtual void visitReturn(A_Return *ast);
  virtual void visitAssign(A_Assign *ast);
  virtual void visitPostfix(A_Postfix *ast);
  virtual void visitInfix(A_Infix *ast);
  virtual void visitPrefix(A_Prefix *ast);
  virtual void visitCall(A_Call *ast);
  // virtual void visitExprs(A_Exprs *ast);
  // virtual void visitIf(A_If *ast);
  // virtual void visitLoop(A_Loop *ast);
  // virtual void visitYield(A_Yield *ast);
  // virtual void visitLoopCondition(A_LoopCondition *ast);
  // virtual void visitLoopEnumerator(A_LoopEnumerator *ast);
  // virtual void visitDoWhile(A_DoWhile *ast);
  // virtual void visitTry(A_Try *ast);
  virtual void visitBlock(A_Block *ast);
  // virtual void visitBlockStats(A_BlockStats *ast);
  virtual void visitPlainType(A_PlainType *ast);
  virtual void visitFuncDef(A_FuncDef *ast);
  // virtual void visitFuncSign(A_FuncSign *ast);
  // virtual void visitParams(A_Params *ast);
  // virtual void visitParam(A_Param *ast);
  virtual void visitVarDef(A_VarDef *ast);
  // virtual void visitTopStats(A_TopStats *ast);
  virtual void visitCompileUnit(A_CompileUnit *ast);

  struct ConstantBuilder : public Visitor {
    IrBuilder *irBuilder;

    ConstantBuilder(IrBuilder *a_irBuilder);
    virtual ~ConstantBuilder() = default;

    virtual void visitInteger(A_Integer *ast);
    virtual void visitFloat(A_Float *ast);
    virtual void visitBoolean(A_Boolean *ast);
    virtual void visitCharacter(A_Character *ast);
    virtual void visitString(A_String *ast);
    virtual void visitNil(A_Nil *ast);
    virtual void visitVoid(A_Void *ast);
    virtual void visitVarId(A_VarId *ast);
    virtual void visitAssign(A_Assign *ast);
    virtual void visitPostfix(A_Postfix *ast);
    virtual void visitInfix(A_Infix *ast);
    virtual void visitPrefix(A_Prefix *ast);
    // virtual void visitExprs(A_Exprs *ast);
  };

private:
  llvm::LLVMContext llvmContext_;
  llvm::IRBuilder<> llvmIRBuilder_;
  llvm::Module *llvmModule_;

  detail::Space space_;
  Scope *scope_;
};
