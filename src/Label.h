// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "infra/Cowstr.h"

class Ast;

class Label {
public:
  enum LabelType {
    UNKNOWN = 0,
    GLOBAL_VARIABLE,
    LOCAL_VARIABLE,
    FUNCTION,
    BASIC_BLOCK,
    MODULE,
  };

  /**
   * For global variable: `HelpMessage` (10.2-10.12) returns
   * `nerd.global.variable.HelpMessage.10.2-10.12`
   *
   * For local variable: `x` (1.4-1.5) returns `$local.variable.x.1.4-1.5`
   *
   * For function: `hello_world` (2.7-2.18) returns
   * `nerd.function.hello_world.2.7-2.18`
   *
   * For basic block returns `nerd.basic.block.body.2.19-5.1`
   *
   * For module: `test/case/parse-4.nerd` (1.1-101.28) returns
   * `nerd.module.test.case.parse-4.nerd.1.1-101.28`
   */
  static Cowstr globalVariable(Ast *ast);
  static Cowstr localVariable(Ast *ast);
  static Cowstr function(Ast *ast);
  static Cowstr basicBlock(Ast *ast);
  static Cowstr modules(Ast *ast);

  /**
   * detect label
   */
  static LabelType what(const Cowstr &label);
};
