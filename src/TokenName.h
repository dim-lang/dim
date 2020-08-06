// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include <string>

const std::string &tokenName(int token);

const int &tokenValue(const std::string &name);

bool tokenExist(int token);

bool tokenExist(const std::string &name);

#define TOKEN_INVALID 0
