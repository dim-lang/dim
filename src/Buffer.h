// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "yydef.h"

#include "location.hh"
#include <cstdio>
#include <string>

class Buffer {
public:
  /* methods */
  Buffer(const std::string &a_fileName, Scanner *scanner);
  virtual ~Buffer();

  /* members */
  std::string fileName;
  YY_BUFFER_STATE yyBufferState;
  yy::location location;
  FILE *fp;

private:
  Scanner *scanner_;
  void release();
};
