// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "NodeVisitor.h"

FunctionCallExpressionVisitor::FunctionCallExpressionVisitor(
    std::shared_ptr<Ast> node)
    : node_(node) {}

FunctionCallExpressionVisitor::FunctionCallExpressionVisitor(Ast *node)
    : node_(std::shared_ptr<Ast>(node)) {}

std::shared_ptr<Ast> FunctionCallExpressionVisitor::visit() { return node_; }
