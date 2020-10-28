// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Label.h"
#include "Ast.h"
#include "fmt/format.h"
#include "infra/Log.h"
#include <algorithm>

#define L_GVAR "$global.variable."
#define L_LVAR "$local.variable."
#define L_FUNC "$function."
#define L_MOD "$module."

Cowstr Label::globalVariable(Ast *ast) {
  LOG_ASSERT(ast, "ast must not null");
  return fmt::format(L_GVAR "{}.{}", ast->name(), ast->location());
}

Cowstr Label::localVariable(Ast *ast) {
  LOG_ASSERT(ast, "ast must not null");
  return fmt::format(L_LVAR "{}.{}", ast->name(), ast->location());
}

Cowstr Label::function(Ast *ast) {
  LOG_ASSERT(ast, "ast must not null");
  return fmt::format(L_FUNC "{}.{}", ast->name(), ast->location());
}

Cowstr Label::modules(Ast *ast) {
  LOG_ASSERT(ast, "ast must not null");
  return fmt::format(L_MOD "{}.{}", ast->name(), ast->location());
}

Label::LabelType Label::what(const Cowstr &label) {
  if (label.startWith(L_GVAR)) {
    return Label::GLOBAL_VARIABLE;
  } else if (label.startWith(L_LVAR)) {
    return Label::LOCAL_VARIABLE;
  } else if (label.startWith(L_FUNC)) {
    return Label::FUNCTION;
  } else if (label.startWith(L_MOD)) {
    return Label::MODULE;
  } else {
    return Label::UNKNOWN;
  }
}
