// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once

class Ast;
class Symtab;
class Tytab;

class Dump {
  static void print(Ast *e);
  static void print(Symtab *e);
  static void print(Tytab *e);
};
