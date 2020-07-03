// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Exception.h"
#include "SymbolTable.h"
#include "boost/core/noncopyable.hpp"
#include "container/LinkedHashMap.h"
#include "enum.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
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
            VoidExpression,
            // statement
            ExpressionStatement, CompoundStatement, IfStatement, WhileStatement,
            ForStatement, ReturnStatement,
            // definition
            GlobalVariableDefinition, LocalVariableDefinition,
            FunctionDefinition, FunctionSignatureDefinition,
            // list
            ExpressionList, StatementList, DefinitionList,
            // common
            TranslateUnit, Expression, Statement, Definition)

/*================ class ================*/
// interface
class Ir;
class IrConstant;
class IrExpression;
class IrStatement;
class IrDefinition;

// translate unit
class IrTranslateUnit;

// list
class IrExpressionList;
class IrStatementList;
class IrDefinitionList;

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
class IrVoidExpression;

// statement
class IrExpressionStatement;
class IrCompoundStatement;
class IrIfStatement;
class IrWhileStatement;
class IrForStatement;
class IrReturnStatement;

// definition
class IrGlobalVariableDefinition; // global variable
class IrLocalVariableDefinition;  // local variable in function
class IrFunctionDefinition;
class IrFunctionSignatureDefinition;

/*================ definition ================*/

class IrContext : protected boost::noncopyable {
public:
  IrContext(const std::string &moduleName);
  virtual ~IrContext();
  const std::string &moduleName() const;
  llvm::LLVMContext &context();
  const llvm::LLVMContext &context() const;
  llvm::IRBuilder<> &builder();
  const llvm::IRBuilder<> &builder() const;
  llvm::Module *&module();
  const llvm::Module *module() const;
  llvm::legacy::FunctionPassManager *&functionPassManager();
  const llvm::legacy::FunctionPassManager *functionPassManager() const;
  SymbolTable *&symbolTable();
  const SymbolTable *symbolTable() const;

private:
  std::string moduleName_;
  llvm::LLVMContext context_;
  llvm::IRBuilder<> builder_;
  llvm::Module *module_;
  llvm::legacy::FunctionPassManager *fpm_;
  SymbolTable *symbolTable_;
};

class Ir : public Namely, public Stringify, private boost::noncopyable {
public:
  Ir(IrContext *context, const std::string &name);
  virtual ~Ir() = default;
  virtual std::string name() const;
  virtual std::string toString() const = 0;
  virtual IrType type() const = 0;
  virtual llvm::Value *codeGen() = 0;
  virtual void buildSymbol();
  virtual void checkSymbol() const;

  // source code function name translation rule, such as:
  // `format_print` to `shp.ir.format.print`
  // `FormatPrint` to `shp.ir.FormatPrint`
  static std::string toIrName(const std::string &name);

  // source code function name reverse translation rule, such as:
  // `shp.ir.format.print` to `format_print`
  // `shp.ir.FormatPrint` to `FormatPrint`
  static std::string fromIrName(const std::string &name);

protected:
  IrContext *context_;
  std::string name_;
};

class IrExpression : public Ir {
public:
  IrExpression(IrContext *context, const std::string &name);
  virtual ~IrExpression() = default;
  virtual IrType type() const = 0;
  virtual llvm::Value *codeGen() = 0;
};

class IrConstant : public IrExpression {
public:
  IrConstant(IrContext *context, const std::string &name);
  virtual ~IrConstant() = default;
  virtual IrType type() const = 0;
  virtual llvm::Value *codeGen() = 0;
};

class IrStatement : public Ir {
public:
  IrStatement(IrContext *context, const std::string &name);
  virtual ~IrStatement() = default;
  virtual IrType type() const = 0;
  virtual llvm::Value *codeGen() = 0;
};

class IrDefinition : public IrStatement {
public:
  IrDefinition(IrContext *context, const std::string &name);
  virtual ~IrDefinition() = default;
  virtual IrType type() const = 0;
  virtual llvm::Value *codeGen() = 0;
};

namespace detail {

template <class T> class IrList : public Ir {
public:
  IrList(IrContext *context, const std::string &name) : Ir(context, name) {}
  virtual ~IrList() {
    for (int i = 0; i < (int)items_.size(); i++) {
      delete items_[i];
      items_[i] = nullptr;
    }
    items_.clear();
  }
  virtual IrType type() const = 0;
  virtual llvm::Value *codeGen() { return nullptr; }
  virtual std::string toString() const {
    std::stringstream ss;
    ss << fmt::format("[@{} size:{}", stringify(), items_.size());
    if (items_.empty()) {
      ss << "]";
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
    ss << "]";
    return ss.str();
  }
  virtual bool empty() const { return items_.empty(); }
  virtual int size() const { return items_.size(); }
  virtual T *get(int pos) const {
    EX_ASSERT(pos >= 0, "pos {} >= 0");
    EX_ASSERT(items_[pos], "items_[{}] is null", pos);
    return items_[pos];
  }
  virtual void add(T *item) {
    EX_ASSERT(item, "item is null");
    items_.push_back(item);
  }
  virtual void buildSymbol() {
    for (int i = 0; i < items_.size(); i++) {
      items_[i]->buildSymbol();
    }
  }
  virtual void checkSymbol() const {
    for (int i = 0; i < items_.size(); i++) {
      items_[i]->checkSymbol();
    }
  }

protected:
  virtual std::string stringify() const = 0;
  std::vector<T *> items_;
};

} // namespace detail

class IrExpressionList : public detail::IrList<IrExpression> {
public:
  IrExpressionList(IrContext *context);
  virtual ~IrExpressionList() = default;
  virtual IrType type() const;

protected:
  virtual std::string stringify() const;
};

class IrStatementList : public detail::IrList<IrStatement> {
public:
  IrStatementList(IrContext *context);
  virtual ~IrStatementList() = default;
  virtual IrType type() const;

protected:
  virtual std::string stringify() const;
};

class IrDefinitionList : public detail::IrList<IrDefinition> {
public:
  IrDefinitionList(IrContext *context);
  virtual ~IrDefinitionList() = default;
  virtual IrType type() const;

protected:
  virtual std::string stringify() const;
};

/* translate unit */
class IrTranslateUnit : public detail::IrList<IrDefinition> {
public:
  IrTranslateUnit(IrContext *context, AstTranslateUnit *node);
  virtual ~IrTranslateUnit() = default;
  virtual IrType type() const;
  virtual void buildSymbol();
  virtual void checkSymbol() const;

protected:
  virtual std::string stringify() const;

private:
  AstTranslateUnit *node_;
};

/* identifier constant */
class IrIdentifierConstant : public IrConstant {
public:
  IrIdentifierConstant(IrContext *context, AstIdentifierConstant *node);
  virtual ~IrIdentifierConstant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstIdentifierConstant *node_;
  int ssaVersion_;
};

/* i8 constant */
class IrInt8Constant : public IrConstant {
public:
  IrInt8Constant(IrContext *context, AstInt8Constant *node);
  virtual ~IrInt8Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstInt8Constant *node_;
};

/* u8 constant */
class IrUInt8Constant : public IrConstant {
public:
  IrUInt8Constant(IrContext *context, AstUInt8Constant *node);
  virtual ~IrUInt8Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstUInt8Constant *node_;
};

/* i16 constant */
class IrInt16Constant : public IrConstant {
public:
  IrInt16Constant(IrContext *context, AstInt16Constant *node);
  virtual ~IrInt16Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstInt16Constant *node_;
};

/* u16 constant */
class IrUInt16Constant : public IrConstant {
public:
  IrUInt16Constant(IrContext *context, AstUInt16Constant *node);
  virtual ~IrUInt16Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstUInt16Constant *node_;
};

/* i32 constant */
class IrInt32Constant : public IrConstant {
public:
  IrInt32Constant(IrContext *context, AstInt32Constant *node);
  virtual ~IrInt32Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstInt32Constant *node_;
};

/* u32 constant */
class IrUInt32Constant : public IrConstant {
public:
  IrUInt32Constant(IrContext *context, AstUInt32Constant *node);
  virtual ~IrUInt32Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstUInt32Constant *node_;
};

/* i64 constant */
class IrInt64Constant : public IrConstant {
public:
  IrInt64Constant(IrContext *context, AstInt64Constant *node);
  virtual ~IrInt64Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstInt64Constant *node_;
};

/* u64 constant */
class IrUInt64Constant : public IrConstant {
public:
  IrUInt64Constant(IrContext *context, AstUInt64Constant *node);
  virtual ~IrUInt64Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstUInt64Constant *node_;
};

/* f32 constant */
class IrFloat32Constant : public IrConstant {
public:
  IrFloat32Constant(IrContext *context, AstFloat32Constant *node);
  virtual ~IrFloat32Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstFloat32Constant *node_;
};

/* f64 constant */
class IrFloat64Constant : public IrConstant {
public:
  IrFloat64Constant(IrContext *context, AstFloat64Constant *node);
  virtual ~IrFloat64Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstFloat64Constant *node_;
};

/* string constant */
class IrStringConstant : public IrConstant {
public:
  IrStringConstant(IrContext *context, AstStringConstant *node);
  virtual ~IrStringConstant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstStringConstant *node_;
};

/* boolean constant */
class IrBooleanConstant : public IrConstant {
public:
  IrBooleanConstant(IrContext *context, AstBooleanConstant *node);
  virtual ~IrBooleanConstant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstBooleanConstant *node_;
};

/* call expression */
class IrCallExpression : public IrExpression {
public:
  IrCallExpression(IrContext *context, AstCallExpression *node);
  virtual ~IrCallExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstCallExpression *node_;
};

/* unary expression */
class IrUnaryExpression : public IrExpression {
public:
  IrUnaryExpression(IrContext *context, AstUnaryExpression *node);
  virtual ~IrUnaryExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstUnaryExpression *node_;
};

/* binary expression */
class IrBinaryExpression : public IrExpression {
public:
  IrBinaryExpression(IrContext *context, AstBinaryExpression *node);
  virtual ~IrBinaryExpression();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstBinaryExpression *node_;
  IrExpression *left_;
  IrExpression *right_;
};

/* conditional expression */
class IrConditionalExpression : public IrExpression {
public:
  IrConditionalExpression(IrContext *context, AstConditionalExpression *node);
  virtual ~IrConditionalExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstConditionalExpression *node_;
};

/* assignment expression */
class IrAssignmentExpression : public IrExpression {
public:
  IrAssignmentExpression(IrContext *context, AstAssignmentExpression *node);
  virtual ~IrAssignmentExpression();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstAssignmentExpression *node_;
  IrExpression *variable_;
  IrExpression *value_;
};

/* sequel expression */
class IrSequelExpression : public IrExpression {
public:
  IrSequelExpression(IrContext *context, AstSequelExpression *node);
  virtual ~IrSequelExpression();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual IrExpressionList *expressionList();
  virtual llvm::Value *codeGen();

private:
  AstSequelExpression *node_;
  IrExpressionList *expressionList_;
};

class IrVoidExpression : public IrExpression {
public:
  IrVoidExpression(IrContext *context, AstVoidExpression *node);
  virtual ~IrVoidExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstVoidExpression *node_;
};

/* expression statement */
class IrExpressionStatement : public IrStatement {
public:
  IrExpressionStatement(IrContext *context, AstExpressionStatement *node);
  virtual ~IrExpressionStatement();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();
  virtual void buildSymbol();
  virtual void checkSymbol() const;

private:
  AstExpressionStatement *node_;
  IrExpression *expression_;
};

/* compound statement */
class IrCompoundStatement : public IrStatement {
public:
  IrCompoundStatement(IrContext *context, AstCompoundStatement *node);
  virtual ~IrCompoundStatement();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();
  virtual void buildSymbol();
  virtual void checkSymbol() const;

private:
  AstCompoundStatement *node_;
  IrStatementList *statementList_;
};

/* if statement */
class IrIfStatement : public IrStatement {
public:
  IrIfStatement(IrContext *context, AstIfStatement *node);
  virtual ~IrIfStatement();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();
  virtual void buildSymbol();
  virtual void checkSymbol() const;

private:
  AstIfStatement *node_;
  IrExpression *condition_;
  IrStatement *thens_;
  IrStatement *elses_;
};

/* while statement */
class IrWhileStatement : public IrStatement {
public:
  IrWhileStatement(IrContext *context, AstWhileStatement *node);
  virtual ~IrWhileStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();
  virtual void buildSymbol();
  virtual void checkSymbol() const;

private:
  AstWhileStatement *node_;
  IrExpression *condition_;
  IrStatement *statement_;
};

/* for statement */
class IrForStatement : public IrStatement {
public:
  IrForStatement(IrContext *context, AstForStatement *node);
  virtual ~IrForStatement();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();
  virtual void buildSymbol();
  virtual void checkSymbol() const;

private:
  AstForStatement *node_;
  IrStatement *start_;
  IrStatement *step_;
  IrExpression *end_;
  IrStatement *statement_;
};

/* return statement */
class IrReturnStatement : public IrStatement {
public:
  IrReturnStatement(IrContext *context, AstReturnStatement *node);
  virtual ~IrReturnStatement();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();
  virtual void buildSymbol();
  virtual void checkSymbol() const;

private:
  AstReturnStatement *node_;
  IrExpression *expression_;
};

/* variable definition */
class IrVariableDefinition : public IrDefinition {
public:
  IrVariableDefinition(IrContext *context, AstVariableDefinition *node);
  virtual ~IrVariableDefinition() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();
  virtual void buildSymbol();
  virtual void checkSymbol() const;

private:
  AstVariableDefinition *node_;
};

/* global variable */
class IrGlobalVariableDefinition : public IrDefinition {
public:
  IrGlobalVariableDefinition(IrContext *context, AstVariableDefinition *node);
  virtual ~IrGlobalVariableDefinition() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();
  virtual void buildSymbol();
  virtual void checkSymbol() const;

private:
  AstVariableDefinition *node_;
};

/* local variable in function */
class IrLocalVariableDefinition : public IrDefinition {
public:
  IrLocalVariableDefinition(IrContext *context, AstVariableDefinition *node);
  virtual ~IrLocalVariableDefinition() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();
  virtual void buildSymbol();
  virtual void checkSymbol() const;

private:
  AstVariableDefinition *node_;
};

/* function definition */
class IrFunctionDefinition : public IrDefinition {
public:
  IrFunctionDefinition(IrContext *context, AstFunctionDefinition *node);
  virtual ~IrFunctionDefinition() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();
  virtual void buildSymbol();
  virtual void checkSymbol() const;

private:
  AstFunctionDefinition *node_;
  IrFunctionSignatureDefinition *signature_;
  IrStatement *statement_;
};

/* function signature definition */
class IrFunctionSignatureDefinition : public IrDefinition {
public:
  IrFunctionSignatureDefinition(IrContext *context,
                                AstFunctionSignatureDefinition *node);
  virtual ~IrFunctionSignatureDefinition() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen();

private:
  AstFunctionSignatureDefinition *node_;
};
