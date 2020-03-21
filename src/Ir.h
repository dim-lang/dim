// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Log.h"
#include "boost/core/noncopyable.hpp"
//#include "llvm/ADT/APFloat.h"
//#include "llvm/ADT/STLExtras.h"
//#include "llvm/IR/BasicBlock.h"
//#include "llvm/IR/Constants.h"
//#include "llvm/IR/DerivedTypes.h"
//#include "llvm/IR/Function.h"
//#include "llvm/IR/IRBuilder.h"
//#include "llvm/IR/Instructions.h"
//#include "llvm/IR/LLVMContext.h"
//#include "llvm/IR/Module.h"
//#include "llvm/IR/Type.h"
//#include "llvm/IR/Verifier.h"
//#include "llvm/Support/TargetSelect.h"
//#include "llvm/Target/TargetMachine.h"

class IrContext : protected boost::noncopyable {
public:
  IrContext();
  virtual ~IrContext();
  llvm::LLVMContext &context();
  const llvm::LLVMContext &context() const;
  llvm::IRBuilder<> &builder();
  const llvm::IRBuilder<> &builder() const;
  llvm::Module *&module();
  const llvm::Module *module() const;
  std::map<std::string, llvm::Value *> &symtable();
  const td::map<std::string, llvm::Value *> &symtable() const;

private:
  llvm::LLVMContext context_;
  llvm::IRBuilder<> builder_;
  llvm::Module *module_;
  std::map<std::string, llvm::Value *> symtable_;
};

template <typename T>
class Ir : public Namely,
           public Typely,
           public Stringify,
           protected boost::noncopyable {
public:
  Ir(T node) : node_(node) {}
  virtual ~Ir() { node_ = nullptr; }
  virtual std::string toString() const {
    return fmt::format("[ {} node_:{} ]", stringify(), node_->name());
  }
  virtual std::string name() const { return name_; }
  virtual int type() const = 0;
  virtual llvm::Value *codegen() = 0;

protected:
  virtual std::string stringify() const = 0;
  T node_;
  std::string name_;
};

/* translate unit */
class IrTranslateUnit : public Ir<AstTranslateUnit *> {
public:
  IrTranslateUnit(AstTranslateUnit *node);
  virtual ~IrTranslateUnit() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstTranslateUnit *node_;
  std::string name_;
};

/* identifier constant */
class IrIdentifierConstant : public Ir<AstIdentifierConstant *> {
public:
  IrIdentifierConstant(AstIdentifierConstant *node);
  virtual ~IrIdentifierConstant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstIdentifierConstant *node_;
  std::string name_;
};

/* i8 constant */
class IrI8Constant : public Ir<AstI8Constant *> {
public:
  IrI8Constant(AstI8Constant *node);
  virtual ~IrI8Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstI8Constant *node_;
  std::string name_;
};

/* u8 constant */
class IrU8Constant : public Ir<AstU8Constant *> {
public:
  IrU8Constant(AstU8Constant *node);
  virtual ~IrU8Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstU8Constant *node_;
  std::string name_;
};

/* i16 constant */
class IrI16Constant : public Ir<AstI16Constant *> {
public:
  IrI16Constant(AstI16Constant *node);
  virtual ~IrI16Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstI16Constant *node_;
  std::string name_;
};

/* u16 constant */
class IrU16Constant : public Ir<AstU16Constant *> {
public:
  IrU16Constant(AstU16Constant *node);
  virtual ~IrU16Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstU16Constant *node_;
  std::string name_;
};

/* i32 constant */
class IrI32Constant : public Ir<AstI32Constant *> {
public:
  IrI32Constant(AstI32Constant *node);
  virtual ~IrI32Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstI32Constant *node_;
  std::string name_;
};

/* u32 constant */
class IrU32Constant : public Ir<AstU32Constant *> {
public:
  IrU32Constant(AstU32Constant *node);
  virtual ~IrU32Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstU32Constant *node_;
  std::string name_;
};

/* i64 constant */
class IrI64Constant : public Ir<AstI64Constant *> {
public:
  IrI64Constant(AstI64Constant *node);
  virtual ~IrI64Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstI64Constant *node_;
  std::string name_;
};

/* u64 constant */
class IrU64Constant : public Ir<AstU64Constant *> {
public:
  IrU64Constant(AstU64Constant *node);
  virtual ~IrU64Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstU64Constant *node_;
  std::string name_;
};

/* f32 constant */
class IrF32Constant : public Ir {
public:
  IrF32Constant(AstF32Constant *node);
  virtual ~IrF32Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstF32Constant *node_;
  std::string name_;
};

/* f64 constant */
class IrF64Constant : public Ir {
public:
  IrF64Constant(AstF64Constant *node);
  virtual ~IrF64Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstF64Constant *node_;
  std::string name_;
};

/* string constant */
class IrStringConstant : public Ir {
public:
  IrStringConstant(AstStringConstant *node);
  virtual ~IrStringConstant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstStringConstant *node_;
  std::string name_;
};

/* boolean constant */
class IrBooleanConstant : public Ir {
public:
  IrBooleanConstant(AstBooleanConstant *node);
  virtual ~IrBooleanConstant() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstBooleanConstant *node_;
  std::string name_;
};

/* call expression */
class IrCallExpression : public Ir {
public:
  IrCallExpression(AstCallExpression *node);
  virtual ~IrCallExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstCallExpression *node_;
  std::string name_;
};

/* unary operation expression */
class IrUnaryExpression : public Ir {
public:
  IrUnaryExpression(AstUnaryExpression *node);
  virtual ~IrUnaryExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstUnaryExpression *node_;
  std::string name_;
};

/* binary operation expression */
class IrBinaryExpression : public Ir {
public:
  IrBinaryExpression(AstBinaryExpression *node);
  virtual ~IrBinaryExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstBinaryExpression *node_;
  std::string name_;
};

/* conditional expression */
class IrConditionalExpression : public Ir {
public:
  IrConditionalExpression(AstConditionalExpression *node);
  virtual ~IrConditionalExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstConditionalExpression *node_;
  std::string name_;
};

/* assignment expression */
class IrAssignmentExpression : public Ir {
public:
  IrAssignmentExpression(AstAssignmentExpression *node);
  virtual ~IrAssignmentExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstAssignmentExpression *node_;
  std::string name_;
};

/* sequel expression */
class IrSequelExpression : public Ir {
public:
  IrSequelExpression(AstSequelExpression *node);
  virtual ~IrSequelExpression() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstSequelExpression *node_;
  std::string name_;
};

/* expression statement */
class IrExpressionStatement : public Ir {
public:
  IrExpressionStatement(AstExpressionStatement *node);
  virtual ~IrExpressionStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstExpressionStatement *node_;
  std::string name_;
};

/* compound statement */
class IrCompoundStatement : public Ir {
public:
  IrCompoundStatement(AstCompoundStatement *node);
  virtual ~IrCompoundStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstCompoundStatement *node_;
  std::string name_;
};

/* if statement */
class IrIfStatement : public Ir {
public:
  IrIfStatement(AstIfStatement *node);
  virtual ~IrIfStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstIfStatement *node_;
  std::string name_;
};

/* while statement */
class IrWhileStatement : public Ir {
public:
  IrWhileStatement(AstWhileStatement *node);
  virtual ~IrWhileStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
  AstWhileStatement *node_;
  std::string name_;
};

/* for statement */
class IrForStatement : public Ir {
public:
  IrForStatement(AstForStatement *node);
  virtual ~IrForStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  AstForStatement *node_;
  std::string name_;
};

/* continue statement */
class IrContinueStatement : public Ir {
public:
  IrContinueStatement(AstContinueStatement *node);
  virtual ~IrContinueStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  AstContinueStatement *node_;
  std::string name_;
};

/* break statement */
class IrBreakStatement : public Ir {
public:
  IrBreakStatement(AstBreakStatement *node);
  virtual ~IrBreakStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  AstBreakStatement *node_;
  std::string name_;
};

/* return statement */
class IrReturnStatement : public Ir {
public:
  IrReturnStatement(AstReturnStatement *node);
  virtual ~IrReturnStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  AstReturnStatement *node_;
  std::string name_;
};

/* empty statement */
class IrEmptyStatement : public Ir {
public:
  IrEmptyStatement(AstEmptyStatement *node);
  virtual ~IrEmptyStatement() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  AstEmptyStatement *node_;
  std::string name_;
};

/* variable declaration */
class IrVariableDeclaration : public Ir {
public:
  IrVariableDeclaration(AstVariableDeclaration *node);
  virtual ~IrVariableDeclaration() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  AstVariableDeclaration *node_;
  std::string name_;
};

/* variable assignment declaration */
class IrVariableAssignmentDeclaration : public Ir {
public:
  IrVariableAssignmentDeclaration(AstVariableAssignmentDeclaration *node);
  virtual ~IrVariableAssignmentDeclaration() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  AstVariableAssignmentDeclaration *node_;
  std::string name_;
};

/* function declaration */
class IrFunctionDeclaration : public Ir {
public:
  IrFunctionDeclaration(AstFunctionDeclaration *node);
  virtual ~IrFunctionDeclaration() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  AstFunctionDeclaration *node_;
  std::string name_;
};

/* function argument declaration */
class IrFunctionArgumentDeclaration : public Ir {
public:
  IrFunctionArgumentDeclaration(AstFunctionArgumentDeclaration *node);
  virtual ~IrFunctionArgumentDeclaration() = default;
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  AstFunctionArgumentDeclaration *node_;
  std::string name_;
};
