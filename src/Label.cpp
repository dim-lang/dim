// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Label.h"
#include "Ast.h"
#include "fmt/format.h"
#include "infra/Log.h"
#include <algorithm>

#define L_FUNC "nerd.function."
#define L_BLOCK "nerd.basic.block."
#define L_MOD "nerd.module."

Cowstr Label::globalVariable(Ast *ast) {
  LOG_ASSERT(ast, "ast must not null");
  Cowstr r = fmt::format("{}.{}", ast->name(), ast->location());
  LOG_INFO("r:{}", r);
  return r;
}

Cowstr Label::localVariable(Ast *ast) {
  LOG_ASSERT(ast, "ast must not null");
  Cowstr r = fmt::format("{}.{}", ast->name(), ast->location());
  LOG_INFO("r:{}", r);
  return r;
}

Cowstr Label::function(Ast *ast) {
  LOG_ASSERT(ast, "ast must not null");
  Cowstr r = fmt::format(L_FUNC "{}.{}", ast->name(), ast->location());
  LOG_INFO("r:{}", r);
  return r;
}

Cowstr Label::basicBlock(Ast *ast) {
  LOG_ASSERT(ast, "ast must not null");
  Cowstr r = fmt::format(L_BLOCK "{}.{}", ast->name(), ast->location());
  LOG_INFO("r:{}", r);
  return r;
}

Cowstr Label::modules(Ast *ast) {
  LOG_ASSERT(ast, "ast must not null");
  Cowstr r = fmt::format(L_MOD "{}.{}", ast->name(), ast->location());
  LOG_INFO("r:{}", r);
  return r;
}

Label::LabelType Label::what(const Cowstr &label) {
  if (label.startWith(L_GVAR)) {
    return Label::GLOBAL_VARIABLE;
  } else if (label.startWith(L_LVAR)) {
    return Label::LOCAL_VARIABLE;
  } else if (label.startWith(L_FUNC)) {
    return Label::FUNCTION;
  } else if (label.startWith(L_BLOCK)) {
    return Label::BASIC_BLOCK;
  } else if (label.startWith(L_MOD)) {
    return Label::MODULE;
  } else {
    return Label::UNKNOWN;
  }
}
