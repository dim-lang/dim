// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Symbol.h"
#include "config/Header.h"

BuiltinTypeSymbol::BuiltinTypeSymbol(const std::string &name)
    : builtinTypeName_(name) {}

const std::string &BuiltinTypeSymbol::name() const { return builtinTypeName_; }

Type *BuiltinTypeSymbol::type() const { return nullptr; }

std::string BuiltinTypeSymbol::toString() const {
  return fmt::format("[ @BuiltinTypeSymbol builtinTypeName_:{} ]",
                     builtinTypeName_);
}

BuiltinTypeSymbol *BuiltinTypeSymbol::i64Instance() {
  static BuiltinTypeSymbol *i64BuiltinTypeSymbol = new BuiltinTypeSymbol("i64");
  return i64BuiltinTypeSymbol;
}

BuiltinTypeSymbol *BuiltinTypeSymbol::f64Instance() {
  static BuiltinTypeSymbol *f64BuiltinTypeSymbol = new BuiltinTypeSymbol("f64");
  return f64BuiltinTypeSymbol;
}

BuiltinTypeSymbol *BuiltinTypeSymbol::stringInstance() {
  static BuiltinTypeSymbol *stringBuiltinTypeSymbol =
      new BuiltinTypeSymbol("string");
  return stringBuiltinTypeSymbol;
}

BuiltinTypeSymbol *BuiltinTypeSymbol::booleanInstance() {
  static BuiltinTypeSymbol *booleanBuiltinTypeSymbol =
      new BuiltinTypeSymbol("boolean");
  return booleanBuiltinTypeSymbol;
}

BuiltinTypeSymbol *BuiltinTypeSymbol::classInstance() {
  static BuiltinTypeSymbol *classBuiltinTypeSymbol =
      new BuiltinTypeSymbol("class");
  return classBuiltinTypeSymbol;
}

BuiltinTypeSymbol *BuiltinTypeSymbol::funcInstance() {
  static BuiltinTypeSymbol *funcBuiltinTypeSymbol =
      new BuiltinTypeSymbol("func");
  return funcBuiltinTypeSymbol;
}

VariableSymbol::VariableSymbol(const std::string &name, Type *type)
    : variableName_(name), variableType_(type) {}

const std::string &VariableSymbol::name() const { return variableName_; }

Type *VariableSymbol::type() const { return variableType_; }

std::string VariableSymbol::toString() const {
  return fmt::format("[ @VariableSymbol variableName_:{}, variableType_:{} ]",
                     variableName_,
                     variableType_ ? variableType_->name() : "null");
}

FunctionSymbol::FunctionSymbol(const std::string &name, Type *type,
                               Scope *enclosingScope)
    : Scope(name, enclosingScope), functionType_(type) {}

const std::string &FunctionSymbol::name() const { return scopeName_; }

Type *FunctionSymbol::type() const { return functionType_; }

std::string FunctionSymbol::toString() const {
  return fmt::format("[ @FunctionSymbol functionName_:{}, functionType_:{}, "
                     "enclosingScope_:{} ]",
                     name(), functionType_ ? functionType_->name() : "null",
                     enclosingScope_ ? enclosingScope_->toString() : "null");
}
