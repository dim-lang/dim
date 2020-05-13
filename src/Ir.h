// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Log.h"
#include "boost/core/noncopyable.hpp"
#include "container/LinkedHashMap.h"
#include "enum.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <vector>

/*================ type from start 4000 ================*/
BETTER_ENUM(IrType, int,
            // constant
            IdentifierConstant = 4000, Float32Constant, Float64Constant,
            Int8Constant, UInt8Constant, Int16Constant, UInt16Constant,
            Int32Constant, UInt32Constant, Int64Constant, UInt64Constant,
            StringConstant, BooleanConstant,
            // expression
            CallExpression, UnaryExpression, BinaryExpression,
            ConditionalExpression, AssignmentExpression, SequelExpression,
            // statement
            ExpressionStatement, CompoundStatement, IfStatement, WhileStatement,
            ForStatement,
            // declaration
            VariableDeclaration, FunctionDeclaration,
            FunctionSignatureDeclaration,
            // list
            ExpressionList, StatementList, DeclarationList,
            // common
            TranslateUnit, Expression, Statement, Declaration)

/*================ class ================*/
// interface
class Ir;
class IrExpression;
class IrStatement;
class IrDeclaration;

// translate unit
class IrTranslateUnit;

// list
class IrExpressionList;
class IrStatementList;
class IrDeclarationList;

// constant
class IrInt8Constant;
class IrUInt8Constant;
class IrInt16Constant;
class IrUInt16Constant;
class IrInt32Constant;
class IrUInt32Constant;
class IrInt64Constant;
class IrUInt64Constant;
class IrFloat32Constant;
class IrFloat64Constant;
class IrStringConstant;
class IrBooleanConstant;
class IrIdentifierConstant;

// expression
class IrCallExpression;
class IrUnaryExpression;
class IrBinaryExpression;
class IrConditionalExpression;
class IrAssignmentExpression;
class IrSequelExpression;

// statement
class IrExpressionStatement;
class IrCompoundStatement;
class IrIfStatement;
class IrWhileStatement;
class IrForStatement;

// declaration
class IrVariableDeclaration;
class IrFunctionDeclaration;
class IrFunctionSignatureDeclaration;

/*================ declaration ================*/

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
  LinkedHashMap<std::string, llvm::Value *> &symtable();
  const LinkedHashMap<std::string, llvm::Value *> &symtable() const;

private:
  llvm::LLVMContext context_;
  llvm::IRBuilder<> builder_;
  llvm::Module *module_;
  LinkedHashMap<std::string, llvm::Value *> symtable_;
};

class IrLLVMValue {
public:
  virtual llvm::Value *codeGen(IrContext *context) = 0;
};

class IrLLVMValueList {
public:
  virtual std::vector<llvm::Value *> codeGen(IrContext *context) = 0;
};

class IrLLVMFunction {
public:
  virtual llvm::Function *codeGen(IrContext *context) = 0;
};

class Ir : public Namely, public Stringify, private boost::noncopyable {
public:
  Ir(const std::string &name);
  virtual ~Ir() = default;
  virtual std::string name() const;
  virtual std::string toString() const = 0;
  virtual IrType type() const = 0;

protected:
  std::string name_;
};

class IrExpression : public Ir, public IrLLVMValue {
public:
  IrExpression(const std::string &name);
  virtual ~IrExpression() = default;
  virtual IrType type() const = 0;
  virtual llvm::Value *codeGen(IrContext *context) = 0;
};

class IrStatement : public Ir, public IrLLVMValue {
public:
  IrStatement(const std::string &name);
  virtual ~IrStatement() = default;
  virtual IrType type() const = 0;
  virtual llvm::Value *codeGen(IrContext *context) = 0;
};

class IrDeclaration : public Ir {
public:
  IrDeclaration(const std::string &name);
  virtual ~IrDeclaration() = default;
  virtual IrType type() const = 0;
};

namespace detail {

template <class T> class IrList : public Ir {
public:
  IrList(const std::string &name) : Ir(name) {}
  virtual ~IrList() {
    for (int i = 0; i < (int)items_.size(); i++) {
      delete items_[i];
      items_[i] = nullptr;
    }
    items_.clear();
  }
  virtual IrType type() const = 0;
  virtual std::string toString() const {
    std::stringstream ss;
    ss << fmt::format("[ @{} size:{}", stringify(), items_.size());
    if (items_.empty()) {
      ss << " ]";
      return ss.str();
    }
    ss << ", ";
    for (int i = 0; i < (int)items_.size(); i++) {
      Ir *item = dynamic_cast<Ir *>(items_[i]);
      ss << fmt::format("{}:{}", i, item ? item->toString() : "null");
      if (i < (int)items_.size() - 1) {
        ss << ",";
      }
    }
    ss << " ]";
    return ss.str();
  }
  virtual bool empty() const { return items_.empty(); }
  virtual int size() const { return items_.size(); }
  virtual T *get(int pos) const { return items_[pos]; }
  virtual void add(T *item) { items_.push_back(item); }

protected:
  virtual std::string stringify() const = 0;
  std::vector<T *> items_;
};

} // namespace detail

class IrExpressionList : public detail::IrList<IrExpression> {
public:
  IrExpressionList();
  virtual ~IrExpressionList() = default;
  virtual IrType type() const;

protected:
  virtual std::string stringify() const;
};

class IrStatementList : public detail::IrList<IrStatement> {
public:
  IrStatementList();
  virtual ~IrStatementList() = default;
  virtual IrType type() const;

protected:
  virtual std::string stringify() const;
};

class IrDeclarationList : public detail::IrList<IrDeclaration> {
public:
  IrDeclarationList();
  virtual ~IrDeclarationList() = default;
  virtual IrType type() const;

protected:
  virtual std::string stringify() const;
};

/* translate unit */
class IrTranslateUnit : public detail::IrList<IrDeclaration> {
public:
  IrTranslateUnit(AstTranslateUnit *node);
  virtual ~IrTranslateUnit() = default;
  virtual IrType type() const;
  virtual std::string dumpCodeGen(IrContext *context);

protected:
  virtual std::string stringify() const;

private:
  AstTranslateUnit *node_;
};

/* identifier constant */
class IrIdentifierConstant : public IrExpression {
public:
  IrIdentifierConstant(AstIdentifierConstant *node);
  virtual ~IrIdentifierConstant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstIdentifierConstant *node_;
};

/* i8 constant */
class IrInt8Constant : public IrExpression {
public:
  IrInt8Constant(AstInt8Constant *node);
  virtual ~IrInt8Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstInt8Constant *node_;
};

/* u8 constant */
class IrUInt8Constant : public IrExpression {
public:
  IrUInt8Constant(AstUInt8Constant *node);
  virtual ~IrUInt8Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstUInt8Constant *node_;
};

/* i16 constant */
class IrInt16Constant : public IrExpression {
public:
  IrInt16Constant(AstInt16Constant *node);
  virtual ~IrInt16Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstInt16Constant *node_;
};

/* u16 constant */
class IrUInt16Constant : public IrExpression {
public:
  IrUInt16Constant(AstUInt16Constant *node);
  virtual ~IrUInt16Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstUInt16Constant *node_;
};

/* i32 constant */
class IrInt32Constant : public IrExpression {
public:
  IrInt32Constant(AstInt32Constant *node);
  virtual ~IrInt32Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstInt32Constant *node_;
};

/* u32 constant */
class IrUInt32Constant : public IrExpression {
public:
  IrUInt32Constant(AstUInt32Constant *node);
  virtual ~IrUInt32Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstUInt32Constant *node_;
};

/* i64 constant */
class IrInt64Constant : public IrExpression {
public:
  IrInt64Constant(AstInt64Constant *node);
  virtual ~IrInt64Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstInt64Constant *node_;
};

/* u64 constant */
class IrUInt64Constant : public IrExpression {
public:
  IrUInt64Constant(AstUInt64Constant *node);
  virtual ~IrUInt64Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstUInt64Constant *node_;
};

/* f32 constant */
class IrFloat32Constant : public IrExpression {
public:
  IrFloat32Constant(AstFloat32Constant *node);
  virtual ~IrFloat32Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstFloat32Constant *node_;
};

/* f64 constant */
class IrFloat64Constant : public IrExpression {
public:
  IrFloat64Constant(AstFloat64Constant *node);
  virtual ~IrFloat64Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstFloat64Constant *node_;
};

/* string constant */
class IrStringConstant : public IrExpression {
public:
  IrStringConstant(AstStringConstant *node);
  virtual ~IrStringConstant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstStringConstant *node_;
};

/* boolean constant */
class IrBooleanConstant : public IrExpression {
public:
  IrBooleanConstant(AstBooleanConstant *node);
  virtual ~IrBooleanConstant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstBooleanConstant *node_;
};

/* call expression */
class IrCallExpression : public IrExpression {
public:
  IrCallExpression(AstCallExpression *node);
  virtual ~IrCallExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstCallExpression *node_;
};

/* unary expression */
class IrUnaryExpression : public IrExpression {
public:
  IrUnaryExpression(AstUnaryExpression *node);
  virtual ~IrUnaryExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstUnaryExpression *node_;
};

/* binary expression */
class IrBinaryExpression : public IrExpression {
public:
  IrBinaryExpression(AstBinaryExpression *node);
  virtual ~IrBinaryExpression();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstBinaryExpression *node_;
  IrExpression *left_;
  IrExpression *right_;
};

/* conditional expression */
class IrConditionalExpression : public IrExpression {
public:
  IrConditionalExpression(AstConditionalExpression *node);
  virtual ~IrConditionalExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstConditionalExpression *node_;
};

/* assignment expression */
class IrAssignmentExpression : public IrExpression {
public:
  IrAssignmentExpression(AstAssignmentExpression *node);
  virtual ~IrAssignmentExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstAssignmentExpression *node_;
};

/* sequel expression */
class IrSequelExpression : public IrExpression {
public:
  IrSequelExpression(AstSequelExpression *node);
  virtual ~IrSequelExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstSequelExpression *node_;
};

/* expression statement */
class IrExpressionStatement : public IrStatement {
public:
  IrExpressionStatement(AstExpressionStatement *node);
  virtual ~IrExpressionStatement();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstExpressionStatement *node_;
  IrExpression *expr_;
};

/* compound statement */
class IrCompoundStatement : public IrStatement {
public:
  IrCompoundStatement(AstCompoundStatement *node);
  virtual ~IrCompoundStatement();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstCompoundStatement *node_;
  IrStatementList *statementList_;
};

/* if statement */
class IrIfStatement : public IrStatement {
public:
  IrIfStatement(AstIfStatement *node);
  virtual ~IrIfStatement();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstIfStatement *node_;
  IrExpression *condition_;
  IrStatement *thens_;
  IrStatement *elses_;
};

/* while statement */
class IrWhileStatement : public IrStatement {
public:
  IrWhileStatement(AstWhileStatement *node);
  virtual ~IrWhileStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstWhileStatement *node_;
};

/* for statement */
class IrForStatement : public IrStatement {
public:
  IrForStatement(AstForStatement *node);
  virtual ~IrForStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstForStatement *node_;
};

/* variable declaration */
class IrVariableDeclaration : public IrDeclaration, public IrLLVMValue {
public:
  IrVariableDeclaration(AstVariableDeclaration *node);
  virtual ~IrVariableDeclaration() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstVariableDeclaration *node_;
};

/* function declaration */
class IrFunctionDeclaration : public IrDeclaration, IrLLVMFunction {
public:
  IrFunctionDeclaration(AstFunctionDeclaration *node);
  virtual ~IrFunctionDeclaration() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Function *codeGen(IrContext *context);

private:
  AstFunctionDeclaration *node_;
  IrFunctionSignatureDeclaration *signature_;
  IrStatement *statement_;
};

/* function signature declaration */
class IrFunctionSignatureDeclaration : public IrDeclaration,
                                       public IrLLVMFunction {
public:
  IrFunctionSignatureDeclaration(AstFunctionSignatureDeclaration *node);
  virtual ~IrFunctionSignatureDeclaration() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Function *codeGen(IrContext *context);

private:
  AstFunctionSignatureDeclaration *node_;
};
