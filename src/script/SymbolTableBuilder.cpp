// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Hasher.h"
#include "Stringify.h"
#include "config/Header.h"
#include "script/NodeVisitor.h"
#include "script/Symbol.h"
#include "script/SymbolTable.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace fastype {

SymbolTableBuilder::SymbolTableBuilder() {}

void SymbolTableBuilder::travel() {}

std::string SymbolTableBuilder::toString() const {}

void SymbolTableBuilder::visit(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitProgram(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitStatementList(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitVariableDeclaration(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitFunctionDeclaration(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitClassDeclaration(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitCompoundStatement(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitAssignmentStatement(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitEmptyStatement(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitReturnStatement(std::shared_ptr<Ast> node) {}

std::shared_ptr<Ast>
SymbolTableBuilder::visitExpression(std::shared_ptr<Ast> node) {}

std::shared_ptr<Ast>
SymbolTableBuilder::visitBinaryOp(std::shared_ptr<Ast> node) {}

std::shared_ptr<Ast>
SymbolTableBuilder::visitUnaryOp(std::shared_ptr<Ast> node) {}

std::shared_ptr<Ast>
SymbolTableBuilder::visitVariable(std::shared_ptr<Ast> node) {}

} // namespace fastype
