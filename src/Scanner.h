// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Token.h"

class Scanner {
public:
  Scanner();
  virtual ~Scanner();
  yyscan_t &scaninfo();

private:
  yyscan_t scaninfo_;
};
